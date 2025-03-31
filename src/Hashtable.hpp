/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Hashtable.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 19:24:35 by pbremond          #+#    #+#             */
/*   Updated: 2025/04/01 00:18:27 by pbremond         ###   ########.fr       */
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

template <class Key, class ValueType, class KeyEqual>
struct _KeyEqualPairAdapter
{
	KeyEqual	key_equal;

	_KeyEqualPairAdapter(KeyEqual keycmp = KeyEqual()) : key_equal(keycmp) {}

	// Customize this struct according to value_type of the container
	// (in reality, either just key_type or pair<(const) key_type, mapped_type> aka value_type)
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
		friend class Hashtable<Key, ValueType, Hash, KeyEqual, Allocator>;
		// typedef typename	ft::remove_const<U>::type	U_constless;
		_Node	**_bucket;
		_Node	*_node;

	public:
		typedef ft::forward_iterator_tag	iterator_category;
		typedef ptrdiff_t					difference_type;
		typedef U							value_type;
		typedef U*							pointer;
		typedef U&							reference;

		_Iterator(_Node **bucket, _Node *entry) : _bucket(bucket), _node(entry) {}
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
		U	*_node;

	public:
		typedef typename	_Iterator<U>::iterator_category	iterator_category;
		typedef typename	_Iterator<U>::difference_type	difference_type;
		typedef typename	_Iterator<U>::value_type		value_type;
		typedef typename	_Iterator<U>::pointer			pointer;
		typedef typename	_Iterator<U>::reference			reference;

		_LocalIterator(U *entry) : _node(entry) {}
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

public:
	Hashtable(size_t bucket_count = 10, const Hash& hash = Hash(), const KeyEqual& key_equal = KeyEqual(), const Allocator& alloc = Allocator())
	: _element_count(0), _max_load_factor(1.0f), _hash_function(hash), _key_equal(key_equal), _allocator(alloc), _ptr_allocator(alloc)
	{
		// XXX: Strong exception guarantee
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
	~Hashtable()
	{
		// clear();
		_ptr_allocator.deallocate(_buckets, _bucket_count);
	}

	allocator_type	get_allocator() const NOEXCEPT		{ return _allocator; }
	allocator_type	get_ptr_allocator() const NOEXCEPT	{ return _ptr_allocator; }

	size_type	get_bucket_count() const NOEXCEPT	{ return _bucket_count; }
	size_type	max_bucket_count() const NOEXCEPT	{ return _ptr_allocator.max_size(); }

	float	get_max_load_factor() const NOEXCEPT	{ return _max_load_factor; }
	void	set_max_load_factor(float n) NOEXCEPT	{ _max_load_factor = n; }
	float	get_load_factor() const NOEXCEPT		{ return _ht._element_count / static_cast<float>(_ht._bucket_count); }

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

	// TODO
	void	rehash(size_type n) { (void)n; }
	void	clear() NOEXCEPT {}

	ft::pair<iterator, bool> insert_unique(const value_type& value)
	{
		if (static_cast<float>(_element_count + 1) / _bucket_count > _max_load_factor)
			rehash(_bucket_count * 2); // TODO: Fibonacci thing

		size_t idx = _hash_function(value) % _bucket_count;
		_Node* node = _buckets[idx];
		while (node && node != _end)
		{
			if (_key_equal(node->value, value))
				return ft::make_pair(iterator(_buckets + idx, node), false); // Key already exists
			node = node->next;
		}
		// Exception guarantee: No modifications before this point
		_Node *new_node = push_node(&_buckets[idx], value);
		++_element_count;
		return ft::make_pair(iterator(_buckets + idx, new_node), true);
	}

	iterator	insert_equal(const value_type& value)
	{
		if (static_cast<float>(_element_count + 1) / _bucket_count > _max_load_factor)
			rehash(_bucket_count * 2); // TODO: Fibonacci thing

		_Node *new_node = create_node(NULL, value);
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
		const_iterator it = static_cast<const _SelfType*>(this)->equal_unique(key);
		_Node *bucket = const_cast<_Node*>(it._node);
		return iterator(it._bucket, bucket);
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
		ft::pair<const_iterator, const_iterator> range = static_cast<const _SelfType*>(this)->equal_range(key);
		return ft::make_pair(
			iterator(range.first._bucket, const_cast<_Node*>(range.first._node)),
			iterator(range.second._bucket, const_cast<_Node*>(range.second._node))
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
