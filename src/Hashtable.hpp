/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Hashtable.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 19:24:35 by pbremond          #+#    #+#             */
/*   Updated: 2025/04/01 23:18:02 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "utility.hpp"
#include "functional.hpp"
#include "iterator.hpp"
#include "type_traits.hpp"

#include <cstddef>
#include <cstring>
#include <memory>

#if __cplusplus < 201103L
# define NOEXCEPT throw()
# define nullptr NULL
#else
# define NOEXCEPT noexcept
#endif

namespace ft { namespace detail {

/*
* Adapter struct that allows to call the Hash function with both key_type and value_type.
* This is used in maps, because value_type is a pair<key_type, mapped_type>.
*/
template <class Key, class ValueType, class Hash>
struct _HashPairAdapter
{
	Hash	hash;

	_HashPairAdapter(Hash hash_func = Hash()) : hash(hash_func) {}

	// Customize this struct according to value_type of the container
	// (in reality, either just key_type or pair<(const) key_type, mapped_type> aka value_type)
	typename Hash::result_type	operator()(Key const& a) 				{ return hash(a); }
	typename Hash::result_type	operator()(Key const& a) const			{ return hash(a); }
	typename Hash::result_type	operator()(ValueType const& a) 			{ return hash(a.first); }
	typename Hash::result_type	operator()(ValueType const& a) const	{ return hash(a.first); }
};

/*
* Adapter struct that allows to call compare key_type with value_type.
* This is useful because in maps, calue_type is a pair<key_type, mapped_type>.
*/
template <class Key, class ValueType, class KeyEqual>
struct _KeyEqualPairAdapter
{
	KeyEqual	key_equal;

	_KeyEqualPairAdapter(KeyEqual keycmp = KeyEqual()) : key_equal(keycmp) {}

	// Customize this struct according to value_type of the container
	// (in reality, just pair<(const) key_type, mapped_type> aka value_type)
	typename KeyEqual::result_type	operator()(Key const& a, Key const& b) 						{ return key_equal(a, b); }
	typename KeyEqual::result_type	operator()(Key const& a, Key const& b) const				{ return key_equal(a, b); }
	typename KeyEqual::result_type	operator()(Key const& a, ValueType const& b) 				{ return key_equal(a, b.first); }
	typename KeyEqual::result_type	operator()(Key const& a, ValueType const& b) const			{ return key_equal(a, b.first); }
	typename KeyEqual::result_type	operator()(ValueType const& a, Key const& b) 				{ return key_equal(a.first, b); }
	typename KeyEqual::result_type	operator()(ValueType const& a, Key const& b) const			{ return key_equal(a.first, b); }
	typename KeyEqual::result_type	operator()(ValueType const& a, ValueType const& b)	 		{ return key_equal(a.first, b.first); }
	typename KeyEqual::result_type	operator()(ValueType const& a, ValueType const& b) const	{ return key_equal(a.first, b.first); }
};

template<
	class Key,
	class ValueType,
	class Hash,		// Must provide operator() overloads for key_type AND value_type, see above
	class KeyEqual, // Must provide operator() overloads for key_type AND value_type, see above
	class Allocator
>
class Hashtable
{
protected:
	// XXX: Is this the best way? I could also inherit in private & tag everything protected...
	template<class Key_, class T_, class Hash_, class Pred_, class Allocator_>
	friend class ft::unordered_map;
	// friend class unordered_set;
	// friend class unordered_multimap;
	// friend class unordered_multiset;

	struct _Node
	{
		_Node		*next;
		ValueType	value;

		_Node() : next(nullptr), value(ValueType()) {}
		_Node(_Node *next_, ValueType const& value_) : next(next_), value(value_) {}
	};

	typedef typename	Allocator::template rebind<_Node>::other				_NodeAllocator;
	typedef typename	Allocator::template rebind<_Node*>::other				_NodePtrAllocator;
	typedef				Hashtable<Key, ValueType, Hash, KeyEqual, Allocator>	_SelfType;

	_Node		**_buckets;
	_Node		*_end;
	size_t		_bucket_count;
	size_t		_element_count;
	float		_max_load_factor;

	Hash		_hash_function;
	KeyEqual	_key_equal;

	_NodeAllocator		_allocator;
	_NodePtrAllocator	_ptr_allocator;

	// TODO: Stuff for const interator
	template <class U>
	class _Iterator
	{
	private:
		// Hashtable needs to access the internal pointers to use iterators internally
		friend class Hashtable<Key, ValueType, Hash, KeyEqual, Allocator>;
		// _NodePtr has same const-qualifier (or lack of) as template type U (_Node const* vs _Node*).
		// _NodeDoublePtr is the same, but with the bonus of C++ double pointer shenanigans.
		typedef typename conditional<is_const<U>::value, const _Node*,			_Node*>::type	_NodePtr;
		typedef typename conditional<is_const<U>::value, const _Node* const*,	_Node**>::type	_NodeDoublePtr;

		_NodeDoublePtr	_bucket;
		_NodePtr		_node;

	public:
		typedef ft::forward_iterator_tag	iterator_category;
		typedef ptrdiff_t					difference_type;
		typedef U							value_type;
		typedef U*							pointer;
		typedef U&							reference;

		_Iterator(_NodeDoublePtr bucket, _NodePtr entry) : _bucket(bucket), _node(entry) {}
		_Iterator(const _Iterator& other) : _bucket(other._bucket), _node(other._node) {}
		_Iterator& operator=(const _Iterator& other) NOEXCEPT
		{
			this->_bucket = other._bucket;
			this->_node = other._node;
			return *this;
		}
		~_Iterator() {}

		inline operator _Iterator<const U>() const { return _Iterator<const U>(_bucket, _node); }

		reference	operator*() NOEXCEPT	{ return _node->value; }
		pointer		operator->() NOEXCEPT	{ return &_node->value; }
		bool		operator==(_Iterator const& rhs) const NOEXCEPT	{ return _node == rhs._node; }
		bool		operator!=(_Iterator const& rhs) const NOEXCEPT	{ return !operator==(rhs); }

		_Iterator&	operator++() NOEXCEPT
		{
			_node = _node->next;
			while (!_node)
			{
				++_bucket;
				_node = *_bucket;
			}
			return *this;
		}
		_Iterator	operator++(int) NOEXCEPT
		{
			_Iterator tmp(*this);
			this->operator++();
			return tmp;
		}
	};


	template <class U>
	class _LocalIterator
	{
	private:
		friend class Hashtable<Key, ValueType, Hash, KeyEqual, Allocator>;
		typedef typename conditional<is_const<U>::value, const _Node*, _Node*>::type	_NodePtr;
		_NodePtr	_node;

	public:
		typedef typename	_Iterator<U>::iterator_category	iterator_category;
		typedef typename	_Iterator<U>::difference_type	difference_type;
		typedef typename	_Iterator<U>::value_type		value_type;
		typedef typename	_Iterator<U>::pointer			pointer;
		typedef typename	_Iterator<U>::reference			reference;

		_LocalIterator(_NodePtr entry) : _node(entry) {}
		_LocalIterator(const _LocalIterator& other) : _node(other._node) {}
		_LocalIterator& operator=(const _LocalIterator& other) NOEXCEPT
		{
			this->_node = other._node;
			return *this;
		}
		~_LocalIterator() {}

		inline operator _Iterator<const U>() const { return _node; }

		reference	operator*() NOEXCEPT	{ return _node->value; }
		pointer		operator->() NOEXCEPT	{ return &_node->value; }
		bool		operator==(_LocalIterator const& rhs) const NOEXCEPT	{ return _node == rhs._node; }
		bool		operator!=(_LocalIterator const& rhs) const NOEXCEPT	{ return !operator==(rhs); }

		_LocalIterator&	operator++() NOEXCEPT
		{
			_node = _node->next;
		}
		_LocalIterator	operator++(int) NOEXCEPT
		{
			_LocalIterator tmp(*this);
			this->operator++();
			return tmp;
		}
	};

public:
	typedef				Key							key_type;
	typedef				ValueType					value_type;
	typedef				Hash						hasher;
	typedef				KeyEqual					key_equal;
	typedef				Allocator					allocator_type;

	typedef typename	Allocator::size_type		size_type;
	typedef typename	Allocator::difference_type	difference_type;

	typedef typename	Allocator::pointer			pointer;
	typedef typename	Allocator::const_pointer	const_pointer;
	typedef typename	Allocator::reference		reference;
	typedef typename	Allocator::const_reference	const_reference;

	typedef 			_Iterator<value_type>				iterator;
	typedef 			_Iterator<const value_type>			const_iterator;
	typedef 			_LocalIterator<value_type>			local_iterator;
	typedef 			_LocalIterator<const value_type>		const_local_iterator;

protected:
	_Node*	create_node(_Node *next, value_type const& value)
		{
			_Node *new_node;
			try {
				new_node = _allocator.allocate(1);
				_allocator.construct(new_node, _Node(next, value));
				return new_node;
			}
			catch (...) { // This is stupid but required by the subject, I'd rather catch an exception
				if (new_node)
					_allocator.deallocate(new_node, 1);
				throw;
			}
		}
	/*
	* Allocate a new node and push it at the front of the given list.
	*/
	_Node*	push_node(_Node **list, ValueType const& value)
	{
		_Node *new_node = create_node(*list, value);
		*list = new_node;
		return new_node;
	}
	_Node*	push_node(_Node **list, _Node *new_node)
	{
		new_node->next = *list;
		*list = new_node;
		return new_node;
	}
	void	remove_node(_Node **list, _Node *prev)
	{
		_Node *node = prev ? prev->next : *list;
		if (prev == NULL)
			*list = node->next;
		else
			prev->next = node->next;
		_allocator.destroy(node);
		_allocator.deallocate(node, 1);
	}
	void	remove_node_range(_Node **list, _Node *prev_begin, _Node *end)
	{
		_Node *node = prev_begin ? prev_begin->next : *list;
		while (node && node != end)
		{
			_Node *next = node->next;
			_allocator.destroy(node);
			_allocator.deallocate(node, 1);
			node = next;
		}
		if (prev_begin == NULL)
			*list = end;
		else
			prev_begin->next = end;
	}
	// Append _end node at the end of given bucket.
	// The bucket MUST be a different bucket than the one _end belongs to.
	void	append_end_node(_Node **bucket) NOEXCEPT
	{
		// Put the _end marker at the end of the given bucket
		_Node **last_next = bucket;
		while (*last_next && (*last_next)->next != nullptr)
			last_next = &(*last_next)->next;
		*last_next = _end;
	}

public:
	Hashtable(size_t bucket_count = 10, const Hash& hash = Hash(), const KeyEqual& key_equal = KeyEqual(), const Allocator& alloc = Allocator())
	: _element_count(0), _max_load_factor(1.0f), _hash_function(hash), _key_equal(key_equal), _allocator(alloc), _ptr_allocator(alloc)
	{
		// XXX: Basic exception guarantee
		try {
			_bucket_count = bucket_count > 0 ? bucket_count : 1;
			_buckets = _ptr_allocator.allocate(_bucket_count);
			memset(_buckets, 0, sizeof(*_buckets) * _bucket_count);
			_end = _allocator.allocate(1);
			_end->next = NULL;
			_buckets[_bucket_count - 1] = _end;
		}
		catch (...) {
			if (_buckets)
				_ptr_allocator.deallocate(_buckets, _bucket_count);
			throw;
		}
	}
	Hashtable(const Hashtable& other)
	: _element_count(0), _max_load_factor(other._max_load_factor), _hash_function(other._hash_function), _key_equal(other._key_equal), _allocator(other._allocator), _ptr_allocator(other._ptr_allocator)
	{
		// XXX: Basic exception guarantee
		try {
			_bucket_count = other._bucket_count;
			_buckets = _ptr_allocator.allocate(_bucket_count);
			memset(_buckets, 0, sizeof(*_buckets) * _bucket_count);
			_end = _allocator.allocate(1);
			_end->next = NULL;
			_buckets[_bucket_count - 1] = _end;

			for (size_t i = 0; i < _bucket_count; ++i)
				if (other._buckets[i])
					push_node(&_buckets[i], other._buckets[i]->value);
		}
		catch (...) {
			if (_buckets) {
				clear();
				_ptr_allocator.deallocate(_buckets, _bucket_count);
			}
			if (_end)
				_allocator.deallocate(_end, 1);
			throw;
		}
	}
	Hashtable&	operator=(const Hashtable& other)
	{
		// XXX: Basic exception guarantee
		if (this == &other)
			return *this;
		clear();
		_allocator.deallocate(_end, 1);
		_bucket_count		= other._bucket_count;
		_element_count		= other._element_count;
		_max_load_factor	= other._max_load_factor;
		_hash_function		= other._hash_function;
		_key_equal			= other._key_equal;
		_allocator			= other._allocator;
		_ptr_allocator		= other._ptr_allocator;

		_buckets = _ptr_allocator.allocate(_bucket_count);
		memset(_buckets, 0, sizeof(*_buckets) * _bucket_count);
		_end = _allocator.allocate(1);
		_end->next = nullptr;
		_buckets[_bucket_count - 1] = _end;
		for (size_t i = 0; i < _bucket_count; ++i)
			if (other._buckets[i])
				push_node(&_buckets[i], other._buckets[i]->value);
		return *this;
	}
	~Hashtable()
	{
		clear();
		if (_end)
			_allocator.deallocate(_end, 1);
		if (_buckets)
			_ptr_allocator.deallocate(_buckets, _bucket_count);
	}

	allocator_type	get_allocator() const NOEXCEPT		{ return _allocator; }
	allocator_type	get_ptr_allocator() const NOEXCEPT	{ return _ptr_allocator; }

	size_type	get_bucket_count() const NOEXCEPT	{ return _bucket_count; }
	size_type	max_bucket_count() const NOEXCEPT	{ return _ptr_allocator.max_size(); }

	float	get_max_load_factor() const NOEXCEPT		{ return _max_load_factor; }
	void	set_max_load_factor(float n) NOEXCEPT		{ _max_load_factor = n; }
	float	get_load_factor() const NOEXCEPT			{ return _element_count / static_cast<float>(_bucket_count); }
	float	get_load_factor(size_type n) const NOEXCEPT	{ return n / static_cast<float>(_bucket_count); }

	size_type	size() const NOEXCEPT		{ return _element_count; }
	size_type	max_size() const NOEXCEPT	{ return _allocator.max_size(); }

	iterator	begin() NOEXCEPT
	{
		_Node **head = _buckets;
		while (*head == nullptr && head < _buckets + _bucket_count)
			++head;
		return iterator(head, *head);
	}
	const_iterator	begin() const NOEXCEPT	{ return cbegin(); }
	const_iterator	cbegin() const NOEXCEPT
	{
		_Node **head = _buckets;
		while (*head == nullptr && head < _buckets + _bucket_count)
			++head;
		return const_iterator(head, *head);
	}
	iterator		end() NOEXCEPT			{ return iterator(_buckets + _bucket_count - 1, _end); }
	const_iterator	end() const NOEXCEPT	{ return cend(); }
	const_iterator	cend() const NOEXCEPT	{ return const_iterator(_buckets + _bucket_count - 1, _end); }

	void	swap(Hashtable &other) NOEXCEPT
	{
		ft::swap(_buckets,			other._buckets);
		ft::swap(_end,				other._end);
		ft::swap(_bucket_count,		other._bucket_count);
		ft::swap(_element_count,	other._element_count);
		ft::swap(_max_load_factor,	other._max_load_factor);
		ft::swap(_hash_function,	other._hash_function);
		ft::swap(_key_equal,		other._key_equal);
		ft::swap(_allocator,		other._allocator);
		ft::swap(_ptr_allocator,	other._ptr_allocator);
	}

	bool		has(key_type const& key) const	{ return equal_unique(key) != end(); }

	size_type	count(key_type const& key) const
	{
		_Node *node = equal_unique(key);
		size_type n = 0;
		while (node && node != _end && _key_equal(node->value, key))
		{
			node = node->next;
			++n;
		}
		return n;
	}

	// Rehashes table but does not need to walk the linked list to push new nodes in
	void	rehash_unique(size_type n)
	{
		if ( n < static_cast<size_type>(size() / get_max_load_factor()) )
			return;

		_Node **new_bucket_list = nullptr;
		try {
			new_bucket_list = _ptr_allocator.allocate(n);
			memset(new_bucket_list, 0, n * sizeof(*new_bucket_list));
			iterator it = begin();
			// Do it node-by-node because rehashing might individuqlly  change the bucket index of
			// nodes in the old bucket
			while (it._node != _end)
			{
				iterator next = it;
				++next;
				// Just push the node at the front of the bucket
				size_type new_idx = _hash_function(*it) % n;
				it._node->next = new_bucket_list[new_idx];
				new_bucket_list[new_idx] = it._node;
				it = next;
			}
			// Put the _end marker at the end of the last bucket
			append_end_node(new_bucket_list + n - 1);

			_ptr_allocator.deallocate(_buckets, _bucket_count);
			_buckets = new_bucket_list;
			_bucket_count = n;
		}
		catch (...) {
			throw;
		}
	}
	// Rehashes table but keeps identical nodes (according to KeyEq) next to eachother
	void	rehash_equal(size_type n)
	{
		if ( n < static_cast<size_type>(size() / get_max_load_factor()) )
			return;

		_Node **new_bucket_list = nullptr;
		try {
			new_bucket_list = _ptr_allocator.allocate(n);
			memset(new_bucket_list, 0, n * sizeof(*new_bucket_list));
			iterator it = begin();
			while (it._node != _end)
			{
				iterator next = it;
				++next;
				size_type new_idx = _hash_function(*it) % n;

				// Find the correct insertion spot
				_Node **prev_next = new_bucket_list + new_idx;
				while (*prev_next && !_key_equal((*prev_next)->value, *it))
					prev_next = &(*prev_next)->next;
				it._node->next = *prev_next ? (*prev_next)->next : nullptr;
				*prev_next = it._node;
			}
			// Put the _end marker at the end of the last bucket
			append_end_node(new_bucket_list + n - 1);

			_ptr_allocator.deallocate(_buckets, _bucket_count);
			_buckets = new_bucket_list;
			_bucket_count = n;
		}
		catch (...) {
			throw;
		}
	}
	void	clear() NOEXCEPT
	{
		for (size_t i = 0; i + 1 < _bucket_count; ++i)
			remove_node_range(_buckets + i, nullptr, nullptr);
		remove_node_range(_buckets + _bucket_count - 1, nullptr, _end);
		_element_count = 0;
	}

	ft::pair<iterator, bool> insert_unique(const value_type& value)
	{
		// Strong exception guarantee: No modifications before this point
		_Node *new_node = create_node(nullptr, value);

		// Check for rehash after creating node to avoid rehashing if allocation/construction throws
		if (get_load_factor(_element_count + 1) > _max_load_factor)
			rehash_unique(_bucket_count * 2); // TODO: Fibonacci thing

		size_t idx = _hash_function(value) % _bucket_count;
		_Node* node = _buckets[idx];
		while (node && node != _end)
		{
			if (_key_equal(node->value, value))
				return ft::make_pair(iterator(_buckets + idx, node), false); // Key already exists
			node = node->next;
		}
		push_node(&_buckets[idx], new_node);
		++_element_count;
		return ft::make_pair(iterator(_buckets + idx, new_node), true);
	}

	iterator	insert_equal(const value_type& value)
	{
		// Strong exception guarantee: No modifications before this point
		_Node *new_node = create_node(NULL, value);

		// Check for rehash after creating node to avoid rehashing if allocation/construction throws
		if (get_load_factor(_element_count + 1) > _max_load_factor)
			rehash_equal(_bucket_count * 2); // TODO: Fibonacci thing

		size_t idx = _hash_function(value) % _bucket_count;
		_Node* node = _buckets[idx];
		if (!node || node == _end)
		{
			new_node->next = _buckets[idx];
			_buckets[idx] = new_node;
		}
		else
		{
			while (node && node->next != _end && !_key_equal(node->value, value))
				node = node->next;
			new_node->next = node->next;
			node->next = new_node;
		}
		++_element_count;
		return iterator(_buckets + idx, new_node);
	}

	/*
	* Returns a pointer to the first node matching key, or NULL if none is found.
	* The returned node is always valid (cannot be _end).
	*/
	const_iterator	equal_unique(key_type const& key) const
	{
		size_t idx = _hash_function(key) % _bucket_count;
		_Node *node = _buckets[idx];
		while (node && node != _end)
		{
			if (_key_equal(key, node->value))
				return const_iterator(_buckets + idx, node);
			node = node->next;
		}
		return end();
	}
	iterator	equal_unique(key_type const& key)
	{
		// Using Meyer's pattern to avoid repeating code twice
		const_iterator it = static_cast<const _SelfType*>(this)->equal_unique(key);
		_Node *node = const_cast<_Node*>(it._node);
		_Node **bucket = const_cast<_Node**>(it._bucket);
		return iterator(bucket, node);
	}

	/*
	* Returns a pair pointers to the range matching given key in format [begin, end)
	* If the range does not exist, returns {NULL, NULL}. The second pointer may be
	* the _end node.
	*/
	ft::pair<const_iterator, const_iterator>	equal_range(key_type const& key) const
	{
		const_iterator range_begin = equal_unique(key);
		if (range_begin == end())
			return ft::make_pair(range_begin, range_begin);
		const_iterator range_end = range_begin;
		++range_end;
		while (range_end != end())
			++range_end;
		return ft::make_pair(range_begin, range_end);
	}
	ft::pair<iterator, iterator>	equal_range(key_type const& key)
	{
		// Using Meyer's pattern to avoid repeating code twice
		ft::pair<const_iterator, const_iterator> range = static_cast<const _SelfType*>(this)->equal_range(key);
		return ft::make_pair(
			iterator(const_cast<_Node**>(range.first._bucket), const_cast<_Node*>(range.first._node)),
			iterator(const_cast<_Node**>(range.second._bucket), const_cast<_Node*>(range.second._node))
		);
	}

	/*
	* Erase a single node. Returns 1 if node was erased, 0 otherwise.
	*/
	size_type	erase_unique(key_type const& key)
	{
		size_type idx = _hash_function(key) % _bucket_count;
		_Node *head = _buckets[idx];
		_Node *prev = NULL;
		while (head && head != _end)
		{
			if (_key_equal(head->value, key))
			{
				remove_node(&_buckets[idx], prev);
				return 1;
			}
			prev = head;
			head = head->next;
		}
		return 0;
	}
	iterator	erase_unique(iterator pos)
	{
		// This is not invalidated because no rehash is triggered
		iterator next = pos;
		++next;

		// If iterator is the first in the bucket, previous node is null, otherwise find the previous node.
		_Node *prev = nullptr;
		if (pos._bucket != pos._node)
		{
			prev = *pos._bucket;
			while (prev->next != pos._node)
				prev = prev->next;
		}
		remove_node(pos._bucket, prev);
		return next;
	}

	/*
	* Erases all nodes matching given key. Returns the amount of nodes erased.
	*/
	size_type	erase_range(key_type const& key)
	{
		size_type idx = _hash_function(key) % _bucket_count;
		_Node *head = _buckets[idx];
		_Node *prev_begin = NULL;
		while (head && head != _end)
		{
			if (_key_equal(head, key))
				break;
			prev_begin = head;
			head = head->next;
		}
		size_type count = 0;
		while (head && head != _end && _key_equal(head->value, key))
		{
			head = head->next;
			++count;
		}
		if (count > 0)
			remove_node_range(&_buckets[idx], prev_begin, head);
		return count;
	}
	iterator	erase_range(iterator first, iterator last)
	{
		// This is not invalidated because no rehash is triggered
		iterator after_last = last;
		++after_last;

		while (first != last)
		{
			_Node *prev = nullptr;
			// If iterator is the first in the bucket, previous node is null, otherwise find the previous node.
			if (first._bucket != first._node)
			{
				prev = *first._bucket;
				while (prev->next != first._node)
					prev = prev->next;
			}
			_Node *head = first._node;
			while (head && head != last._node)
			{
				remove_node(first._bucket, prev);
				prev = head;
				head = head->next;
			}
			first._node = prev;
			++first;
		}
		return after_last;
	}
};

}}

#if __cplusplus < 201103L
# undef nullptr
#endif
#undef NOEXCEPT
