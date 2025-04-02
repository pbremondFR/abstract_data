/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stl_unordered_map.tpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 16:58:50 by pbremond          #+#    #+#             */
/*   Updated: 2025/04/02 15:48:45 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "stl_unordered_map.hpp" // TODO: Remove me, just for intellisense

template<class Key, class T, class Hash, class Pred, class Allocator>
ft::unordered_map<Key, T, Hash, Pred, Allocator>::unordered_map(
	size_type n,
	const hasher& hf,
	const key_equal& eql,
	const allocator_type& alloc)
: _ht(n, typename _Hashtable::hasher(hf), typename _Hashtable::key_equal(eql), alloc)
{

}

template<class Key, class T, class Hash, class Pred, class Allocator>
template <class InputIterator>
ft::unordered_map<Key, T, Hash, Pred, Allocator>::unordered_map(
	InputIterator first,
	InputIterator last,
	size_type n,
	const hasher& hf,
	const key_equal& eql,
	const allocator_type& alloc)
: _ht(n, typename _Hashtable::hasher(hf), typename _Hashtable::key_equal(eql), alloc)
{
	insert(first, last);
}

template<class Key, class T, class Hash, class Pred, class Allocator>
ft::unordered_map<Key, T, Hash, Pred, Allocator>::unordered_map(const unordered_map& other)
: _ht(other._ht)
{

}

template<class Key, class T, class Hash, class Pred, class Allocator>
ft::unordered_map<Key, T, Hash, Pred, Allocator>::unordered_map(const Allocator& alloc)
: _ht(10, Hash(), Pred(), alloc)
{

}

template<class Key, class T, class Hash, class Pred, class Allocator>
ft::unordered_map<Key, T, Hash, Pred, Allocator>::unordered_map(const unordered_map& src, const Allocator& alloc)
: _ht(src.bucket_count(), src.hash_function(), src.key_eq(), alloc)
{
	insert(src.begin(), src.end());
}

template<class Key, class T, class Hash, class Pred, class Allocator>
ft::unordered_map<Key, T, Hash, Pred, Allocator>::~unordered_map()
{

}

template<class Key, class T, class Hash, class Pred, class Allocator>
ft::pair<
	typename ft::unordered_map<Key, T, Hash, Pred, Allocator>::iterator,
	bool
>
	ft::unordered_map<Key, T, Hash, Pred, Allocator>::insert(const value_type& obj)
{
	return _ht.insert_unique(obj);
}

template<class Key, class T, class Hash, class Pred, class Allocator>
typename ft::unordered_map<Key, T, Hash, Pred, Allocator>::iterator
	ft::unordered_map<Key, T, Hash, Pred, Allocator>::insert(const_iterator hint, const value_type& obj)
{
	(void)hint;
	return insert(obj).first;
}

template<class Key, class T, class Hash, class Pred, class Allocator>
template <class InputIterator>
typename ft::enable_if<
	!ft::is_fundamental<InputIterator>::value,
	void
>::type	ft::unordered_map<Key, T, Hash, Pred, Allocator>::insert(InputIterator first, InputIterator last)
{
	for (; first != last; ++first)
		insert(*first);
}

template<class Key, class T, class Hash, class Pred, class Allocator>
typename ft::unordered_map<Key, T, Hash, Pred, Allocator>::mapped_type&
	ft::unordered_map<Key, T, Hash, Pred, Allocator>::operator[](const key_type& key)
{
	// first: pointer to node, second: was inserted -> true, already existed -> false
	ft::pair<iterator, bool> insertion = insert(ft::make_pair(key, mapped_type()));
	iterator it = insertion.first;
	return it->second;
}

template<class Key, class T, class Hash, class Pred, class Allocator>
typename ft::unordered_map<Key, T, Hash, Pred, Allocator>::mapped_type&
	ft::unordered_map<Key, T, Hash, Pred, Allocator>::at(const key_type& key)
{
	return const_cast<mapped_type&>(static_cast<const _SelfType*>(this)->at(key));
}

template<class Key, class T, class Hash, class Pred, class Allocator>
typename ft::unordered_map<Key, T, Hash, Pred, Allocator>::mapped_type const&
	ft::unordered_map<Key, T, Hash, Pred, Allocator>::at(const key_type& key) const
{
	const_iterator it = _ht.equal_unique(key);
	if (it == end())
		throw ft::out_of_range("unordered_map::at: out-of-range");
	return it->second;
}

template<class Key, class T, class Hash, class Pred, class Allocator>
typename ft::unordered_map<Key, T, Hash, Pred, Allocator>::size_type
	ft::unordered_map<Key, T, Hash, Pred, Allocator>::bucket_size(size_type n) const
{
	_Node *head = _ht._buckets[n];
	size_type count = 0;
	while (head && head != _ht._end)
	{
		head = head->next;
		++count;
	}
	return count;
}

#ifdef noexcept
# undef noexcept
# undef nullptr
#endif
