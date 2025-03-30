/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stl_unordered_map.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 16:20:09 by pbremond          #+#    #+#             */
/*   Updated: 2025/03/30 23:51:44 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Hashtable.hpp"
#include "utility.hpp"
#include "functional.hpp"
#include "exception.hpp"
#include <string>
#include <cstddef>

#if __cplusplus < 201103L
# define NOEXCEPT throw()
# define nullptr NULL
#else
# define NOEXCEPT noexcept
#endif

namespace ft {
template<
	class Key,
	class T,
	class Hash = ft::hash<Key>,
	class Pred = ft::equal_to<Key>,
	class Allocator = std::allocator<ft::pair<const Key, T> >
>
class unordered_map
{
private:
	typedef typename	ft::detail::Hashtable<
		Key,
		ft::pair<Key, T>,
		ft::detail::_HashPairAdapter<Key, ft::pair<Key, T>, Hash>,
		ft::detail::_KeyEqualPairAdapter<Key, ft::pair<Key, T>, ft::equal_to<Key> >,
		Allocator
	>	_Hashtable;

	typedef typename _Hashtable::_Node								_Node;
	typedef			 unordered_map<Key, T, Hash, Pred, Allocator>	_SelfType;

	_Hashtable	_ht;

public:
	// types
	typedef				Key								key_type;
	typedef				ft::pair<const Key, T>			value_type;
	typedef				T								mapped_type;
	typedef				Hash							hasher;
	typedef				Pred							key_equal;
	typedef				Allocator						allocator_type;
	typedef typename	allocator_type::pointer			pointer;
	typedef typename	allocator_type::const_pointer	const_pointer;
	typedef typename	allocator_type::reference		reference;
	typedef typename	allocator_type::const_reference	const_reference;
	typedef				size_t							size_type;
	typedef				ptrdiff_t						difference_type;
	typedef typename	_Hashtable::_Iterator			iterator;
	typedef typename	_Hashtable::_Iterator			const_iterator;
	typedef typename	_Hashtable::_LocalIterator		local_iterator;
	typedef typename	_Hashtable::_LocalIterator		const_local_iterator;

	// construct/destroy/copy
	explicit unordered_map(size_type n = 10 , const hasher& hf = hasher(), const key_equal& eql = key_equal(), const allocator_type& a = allocator_type());
	template <class InputIterator>
	unordered_map(InputIterator f, InputIterator l, size_type n = 10 , const hasher& hf = hasher(), const key_equal& eql = key_equal(), const allocator_type& a = allocator_type());
	unordered_map(const unordered_map&);
	explicit unordered_map(const Allocator&);
	unordered_map(const unordered_map&, const Allocator&);
	~unordered_map();
	unordered_map& operator=(const unordered_map&);
	allocator_type get_allocator() const NOEXCEPT;

	// size and capacity
	bool		empty() const NOEXCEPT		{ return _ht.size() == 0; };
	size_type	size() const NOEXCEPT		{ return _ht.size(); };
	size_type	max_size() const NOEXCEPT	{ return _ht.max_size(); };

	// iterators
	iterator		begin() NOEXCEPT			{ return const_cast<iterator>(static_cast<const _SelfType*>(this)->begin()); } // Meyer's pattern
	const_iterator	begin() const NOEXCEPT		{ return this->cbegin(); }
	iterator		end() NOEXCEPT				{ return _ht._end; }
	const_iterator	end() const NOEXCEPT		{ return _ht._end; }
	const_iterator	cbegin() const NOEXCEPT;
	const_iterator	cend() const NOEXCEPT		{ return _ht._end; }

	// modifiers
	pair<iterator, bool>	insert(const value_type& obj);
	iterator 				insert(const_iterator hint, const value_type& obj);
	template <class InputIterator>
	void		insert(InputIterator first, InputIterator last);
	iterator	erase(const_iterator position);
	size_type	erase(const key_type& k)	{ return _ht.erase_unique(k); }
	iterator	erase(const_iterator first, const_iterator last);
	void 		clear() NOEXCEPT			{ _ht.clear(); }
	void 		swap(unordered_map &other)	{ ft::swap(_ht, other.ht); }

	// observers
	hasher		hash_function() const	{ return _ht._hash_function.hash; }
	key_equal	key_eq() const			{ return _ht._key_equal.key_equal; }

	// lookup
	iterator		find(const key_type& k);
	const_iterator	find(const key_type& k) const;
	size_type		count(const key_type& k) const	{ return _ht.has(k) ? 1 : 0; }
	ft::pair<iterator, iterator>				equal_range(const key_type& k)
	{
		typedef ft::pair<iterator, iterator>	return_type;
		return const_cast<return_type>(static_cast<const _SelfType*>(this)->equal_range());
	}
	ft::pair<const_iterator, const_iterator>	equal_range(const key_type& k) const
	{
		ft::pair<_Node*, _Node*> range = _ht.equal_range();
		return ft::make_pair(const_iterator(range.first), const_iterator(range.second));
	}
	mapped_type&		operator[](const key_type& k);
	mapped_type&		at(const key_type& k);
	const mapped_type&	at(const key_type& k) const;

	// bucket interface
	size_type				bucket_count() const NOEXCEPT		{ return _ht._bucket_count; }
	size_type				max_bucket_count() const NOEXCEPT	{ return _ht.max_bucket_count(); }
	size_type				bucket_size(size_type n) const;
	size_type				bucket(const key_type& k) const		{ return _ht._hash_function(k) % _ht._bucket_count; }
	local_iterator			begin(size_type n)			{ return local_iterator(_ht._buckets[n]); }
	const_local_iterator	begin(size_type n) const	{ return const_local_iterator(_ht._buckets[n]); }
	local_iterator			end(size_type n)			{ return n == _ht._bucket_count - 1 ? _ht._end : nullptr; }
	const_local_iterator	end(size_type n) const		{ return n == _ht._bucket_count - 1 ? _ht._end : nullptr; }
	const_local_iterator	cbegin(size_type n) const	{ return const_local_iterator(_ht._buckets[n]); }
	const_local_iterator	cend(size_type n) const		{ return n == _ht._bucket_count - 1 ? _ht._end : nullptr; }

	// hash policy
	float	load_factor() const NOEXCEPT;
	float	max_load_factor() const NOEXCEPT	{ return _ht.get_max_load_factor(); }
	void	max_load_factor(float z)			{ _ht.set_max_load_factor(z); }
	void	rehash(size_type n);
	void	reserve(size_type n);
};

template <class Key, class T, class Hash, class Pred, class Alloc>
void swap(unordered_map<Key, T, Hash, Pred, Alloc>& x, unordered_map<Key, T, Hash, Pred, Alloc>& y);

template <class Key, class T, class Hash, class Pred, class Alloc>
bool operator==(const unordered_map<Key, T, Hash, Pred, Alloc>& a, const unordered_map<Key, T, Hash, Pred, Alloc>& b);

template <class Key, class T, class Hash, class Pred, class Alloc>
bool operator!=(const unordered_map<Key, T, Hash, Pred, Alloc>& a, const unordered_map<Key, T, Hash, Pred, Alloc>& b);

} // namespace ft

#include "stl_unordered_map.tpp"
