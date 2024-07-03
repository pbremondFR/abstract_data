/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algorithm.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 22:45:28 by pbremond          #+#    #+#             */
/*   Updated: 2024/07/03 02:51:42 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <functional.hpp>

namespace ft
{

template <class T>
void	swap(T& a, T& b)
{
	T tmp(a);
	a = b;
	b = tmp;
}

template <class ForwardIt1, class ForwardIt2>
void	iter_swap(ForwardIt1 a, ForwardIt2 b)
{
	swap(*a, *b);
}

// TESTME and do everything else I guess
// NOTE: lmao I can't write namespace __detail::heap because that's C++17
namespace __detail { namespace heap
{
	// Iterator is necessarily a random access iterator
	template <class It>	inline It	get_left_child(It pos)	{ return 2 * pos + 1; }
	template <class It>	inline It	get_right_child(It pos)	{ return 2 * pos + 2; }
	template <class It>	inline It	get_parent(It pos)		{ return (pos - 1) / 2; }

	template <class It>	void	sift_down(It begin, It end)
	{
		while (get_left_child(begin) < end)
		{
			// Use Compare instead of operator<() ?
			It child = get_left_child(begin);
			if (child + 1 < end && *child < *(child + 1))
				child = child + 1;
			if (*begin < *child) {
				swap_iter(begin, child);
				begin = child;
			}
			else
				return;
		}
	}

	template <class It> void	sift_up(It begin, It end)
	{
		while (end > begin)
		{
			It parent = get_parent(end);
			if (*parent < *end) {
				swap_iter(parent, end);
				end = parent;
			}
			else
				return;
		}
	}
}} // namespace __detail::heap


template <class RandomIt>
void	make_heap(RandomIt first, RandomIt last)
{
	make_heap(first, last, less<typename RandomIt::value_type>());
}

template <class RandomIt, class Compare>
void	make_heap(RandomIt first, RandomIt last, Compare comp)
{
	using namespace __detail::heap;

	// TODO: That's the wiki version of the algo with std::less, what about general version?
	RandomIt start = parent(last - 1) + 1;
	while (start > first)	// Would comp(first, start) work in all cases?
	{
		--start;
		sift_down(start, last - first);
	}
}

template <class RandomIt>
void	push_heap(RandomIt first, RandomIt last)
{
	push_heap(first, last, less<typename RandomIt::value_type>());
}

template <class RandomIt, class Compare>
void	push_heap(RandomIt first, RandomIt last, Compare comp)
{

}

template <class RandomIt>
void	pop_heap(RandomIt first, RandomIt last)
{
	pop_heap(first, last, less<typename RandomIt::value_type>());
}

template <class RandomIt, class Compare>
void	pop_heap(RandomIt first, RandomIt last, Compare comp)
{

}

template < class InputIt1, class InputIt2 >
bool	equal(InputIt1 first1, InputIt1 last1, InputIt2 first2)
{
	for (; first1 != last1; ++first1, ++first2)
		if (*first1 != *first2)
			return (false);
	return (true);
}

// Why the FUCK was this not implemented until C++14. Fuck you C++98, I'm implementing it.
template < class InputIt1, class InputIt2 >
bool	equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2)
{
	for (; first1 != last1 && first2 != last2; ++first1, ++first2)
		if (*first1 != *first2)
			return (false);
	return (first1 == last1 && first2 == last2);
}

template < class InputIt1, class InputIt2, class BinaryPredicate >
bool	equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, BinaryPredicate p)
{
	for (; first1 != last1; ++first1, ++first2)
		if (!p(first1, first2))
			return (false);
	return (true);
}

template < class InputIt1, class InputIt2, class BinaryPredicate >
bool	equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2,
	BinaryPredicate p)
{
	for (; first1 != last1 && first2 != last2; ++first1, ++first2)
		if (!p(first1, first2))
			return (false);
	return (true);
}

template < class InputIt1, class InputIt2 >
bool	lexicographical_compare(InputIt1 first1, InputIt1 last1,
								InputIt2 first2, InputIt2 last2)
{ // TESTME: different behaviour from the next overload
	while (first1 != last1 && first2 != last2)
	{
		if (*first1 < *first2)
			return (true);
		if (*first1 > *first2)
			return (false);
		++first1;
		++first2;
	}
	return (first1 == last1 && first2 != last2);
}

template < class InputIt1, class InputIt2, class Compare >
bool	lexicographical_compare(InputIt1 first1, InputIt1 last1,
								InputIt2 first2, InputIt2 last2,
								Compare comp)
{ // TESTME: different behaviour from the previous overload
	while (first1 != last1 && first2 != last2 && *first1 == *first2)
	{
		if (comp(*first1, *first2))
			return (true);
		if (comp(*first2, *first1))
			return (false);
		++first1;
		++first2;
	}
	return (first1 == last1 && first2 != last2);
}


} // namespace ft
