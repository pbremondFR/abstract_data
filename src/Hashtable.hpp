/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Hashtable.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 19:24:35 by pbremond          #+#    #+#             */
/*   Updated: 2025/03/30 15:43:04 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "utility.hpp"
#include "functional.hpp"
#include "iterator.hpp"

#include <cstdint>
#include <memory>
#include <unordered_set>

namespace ft { namespace detail {

template <class Key, class ValueType, class KeyEqual>
struct _KeyEqualForPair
{
	KeyEqual	key_equal;

	_KeyEqualForPair(KeyEqual keycmp = KeyEqual()) : key_equal(keycmp) {}

	// Customize this struct according to value_type of the container
	// (in reality, either just key_type or pair<(const) key_type, mapped_type> aka value_type)
	bool	operator()(Key const& a, Key const& b)				{ return key_equal(a, b); }
	bool	operator()(Key const& a, ValueType const& b)		{ return key_equal(a, b.first); }
	bool	operator()(ValueType const& a, Key const& b)		{ return key_equal(a.first, b); }
	bool	operator()(ValueType const& a, ValueType const& b)	{ return key_equal(a.first, b.first); }
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
private:
	struct _Node
	{
		_Node		*next;
		ValueType	value;

		static _Node *create(_Node *next, value_type const& value)
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
	};

	typedef typename Allocator::template rebind<_Node>::other	_NodeAllocator;
	typedef typename Allocator::template rebind<_Node*>::other	_NodePtrAllocator;

	_Node		**_buckets;
	_Node		*_end;
	size_t		_bucket_count;
	size_t		_element_count;
	float		_max_load_factor;

	Hash		_hash_function;
	KeyEqual	_key_equal;

	_NodeAllocator		_allocator;
	_NodePtrAllocator	_ptr_allocator;

	class _Iterator
	{
	private:
		_Node	**_bucket_list;
		_Node	*_bucket;

	public:
		typedef ft::forward_iterator_tag	iterator_category;
		typedef ptrdiff_t					difference_type;
		typedef ValueType					value_type;
		typedef ValueType*					pointer;
		typedef ValueType&					reference;

		_Iterator(_Node **buckets, _Node *entry) : _bucket_list(buckets), _bucket(entry) {}
		_Iterator(const _Iterator& other) : _bucket_list(other._bucket_list), _bucket(other._bucket) {}
		_Iterator& operator=(const _Iterator& other)
		{
			this->_bucket_list = other._bucket_list;
			this->_bucket = other._bucket;
			return *this;
		}
		~_Iterator() {}

		reference	operator*()		{ return _bucket->value; }
		pointer		operator->()	{ return &_bucket->value; }

		_Iterator&	operator++()
		{
			_bucket = _bucket->next;
			while (!_bucket)
			{
				++_bucket_list;
				_bucket = *_bucket_list;
			}
		}
		_Iterator	operator++(int)
		{
			_Iterator tmp(*this);
			this->operator++();
			return tmp;
		}
	};

	/*
	* Allocate a new node and push it at the front of the given list.
	*/
	_Node*	push_node(_Node **list, ValueType const& value)
	{
		_Node *new_node = _Node::create(*list, value);
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
	typedef Key						key_type;
	typedef ValueType				value_type;
	typedef Hash					hasher;
	typedef KeyEqual				key_equal;
	typedef Allocator				allocator_type;

	typedef typename Allocator::size_type		size_type;
	typedef typename Allocator::difference_type	difference_type;

	typedef typename Allocator::pointer			pointer;
	typedef typename Allocator::const_pointer	const_pointer;
	typedef typename Allocator::reference		reference;
	typedef typename Allocator::const_reference	const_reference;

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
			_bucket_count[_bucket_count - 1] = _end;
		}
		catch (...) {
			if (_buckets)
				_ptr_allocator.deallocate(_buckets, _bucket_count);
			throw;
		}
	}
	~Hashtable()
	{
		clear();
		_ptr_allocator.deallocate(_buckets, _bucket_count);
	}

	std::pair<_Node*, bool> insert_unique(const value_type& value)
	{
		if (static_cast<float>(_element_count + 1) / _bucket_count > _max_load_factor)
			rehash(_bucket_count * 2); // TODO: Fibonacci thing

		size_t idx = _hash_function(value) % _bucket_count;
		_Node* node = _buckets[idx];
		while (node && node != _end)
		{
			if (_key_equal(node->value, value))
				return std::make_pair(node, false); // Key already exists
			node = node->next;
		}
		// Exception guarantee: No modifications before this point
		_Node *new_node = push_node(&_buckets[idx], value);
		++_element_count;
		return std::make_pair(new_node, true);
	}

	_Node*	insert_equal(const value_type& value)
	{
		if (static_cast<float>(_element_count + 1) / _bucket_count > _max_load_factor)
			rehash(_bucket_count * 2); // TODO: Fibonacci thing

		_Node *new_node = _Node::create(NULL, value);
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
		return new_node;
	}

	/*
	* Returns a pointer to the first node matching key, or NULL if none is found.
	* The returned node is always valid (cannot be _end).
	*/
	_Node*	equal_unique(key_type const& key)
	{
		size_t idx = _hash_function(key) % _bucket_count;
		_Node *node = _buckets[idx];
		while (node && node != _end)
		{
			if (_key_equal(key, node->value))
				return node;
			node = node->next;
		}
		return NULL;
	}

	/*
	* Returns a pair pointers to the range matching given key in format [begin, end)
	* If the range does not exist, returns {NULL, NULL}. The second pointer may be
	* the _end node.
	*/
	ft::pair<_Node*, _Node*>	equal_range(key_type const& key)
	{
		_Node *range_begin = equal_unique(key);
		if (!range_begin)
			return ft::make_pair(NULL, NULL);
		_Node *range_end = range_begin->next;
		while (range_end && range_end != _end && _key_equal(range_end->value, key))
			range_end = range_end->next;
		return ft::make_pair(range_begin, range_end);
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
			if (_key_equal(head, key))
			{
				remove_node(&_buckets[idx], prev);
				return 1;
			}
			prev = head;
			head = head->next;
		}
		return 0;
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
};

}}
