/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stl_unordered_map.tpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 16:58:50 by pbremond          #+#    #+#             */
/*   Updated: 2025/03/31 00:43:06 by pbremond         ###   ########.fr       */
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
: _ht(n, hf, eql, alloc)
{

}

template<class Key, class T, class Hash, class Pred, class Allocator>
typename ft::unordered_map<Key, T, Hash, Pred, Allocator>::iterator
	ft::unordered_map<Key, T, Hash, Pred, Allocator>::begin() NOEXCEPT
{
	_Node **head = _ht._buckets;
	while (*head == nullptr)
		++head;
	return iterator(_ht._buckets, *head);
}

template<class Key, class T, class Hash, class Pred, class Allocator>
typename ft::unordered_map<Key, T, Hash, Pred, Allocator>::const_iterator
	ft::unordered_map<Key, T, Hash, Pred, Allocator>::cbegin() const NOEXCEPT
{
	_Node **head = _ht._buckets;
	while (*head == nullptr)
		++head;
	return const_iterator(_ht._buckets, static_cast<const _Node*>(*head));
}

template<class Key, class T, class Hash, class Pred, class Allocator>
ft::pair<
	typename ft::unordered_map<Key, T, Hash, Pred, Allocator>::iterator,
	bool
>
	ft::unordered_map<Key, T, Hash, Pred, Allocator>::insert(const value_type& obj)
{
	// first: pointer to node, second: was inserted -> true, already existed -> false
	ft::pair<_Node*, bool> insertion = _ht.insert_unique(obj);
	return ft::make_pair(iterator(_ht._buckets, insertion.first), insertion.second);
}

template<class Key, class T, class Hash, class Pred, class Allocator>
typename ft::unordered_map<Key, T, Hash, Pred, Allocator>::iterator
	ft::unordered_map<Key, T, Hash, Pred, Allocator>::find(const key_type& key)
{
	const_cast<iterator&>(static_cast<const _SelfType*>(this)->find(key));
}

template<class Key, class T, class Hash, class Pred, class Allocator>
typename ft::unordered_map<Key, T, Hash, Pred, Allocator>::const_iterator
	ft::unordered_map<Key, T, Hash, Pred, Allocator>::find(const key_type& key) const
{
	_Node *node = _ht.equal_unique(key);
	return node ? const_iterator(_ht._buckets, node) : _ht._end;
}

template<class Key, class T, class Hash, class Pred, class Allocator>
typename ft::unordered_map<Key, T, Hash, Pred, Allocator>::mapped_type&
	ft::unordered_map<Key, T, Hash, Pred, Allocator>::operator[](const key_type& key)
{
	// first: pointer to node, second: was inserted -> true, already existed -> false
	ft::pair<_Node*, bool> insertion = _ht.insert_unique(ft::make_pair(key, mapped_type()));
	// Cast to add back the const in the pair thing
	return static_cast<mapped_type&>(insertion.first->value);
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
	_Node *node = _ht.equal_unique(key);
	if (node == nullptr)
		throw ft::out_of_range("unordered_map::at: out-of-range");
	return static_cast<mapped_type const&>(node->value);
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
