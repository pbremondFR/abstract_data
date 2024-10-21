/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterator.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 19:27:37 by pbremond          #+#    #+#             */
/*   Updated: 2024/10/21 00:46:36 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iterator>	// Use for iterator tags

namespace ft
{

// struct input_iterator_tag {};
// struct output_iterator_tag {};
// struct forward_iterator_tag			: public input_iterator_tag {};
// struct bidirectional_iterator_tag	: public forward_iterator_tag {};
// struct random_access_iterator_tag	: public bidirectional_iterator_tag {};

// Subjects states to make our own iterator tags by typedefing the std ones.
typedef	::std::input_iterator_tag			input_iterator_tag;
typedef	::std::output_iterator_tag			output_iterator_tag;
typedef	::std::forward_iterator_tag			forward_iterator_tag;
typedef	::std::bidirectional_iterator_tag	bidirectional_iterator_tag;
typedef	::std::random_access_iterator_tag	random_access_iterator_tag;

template < class _Category, class T, class _Distance = ::std::ptrdiff_t,
	class _Pointer = T*, class _Reference = T& >
struct iterator
{
	typedef T			value_type;
	typedef _Distance	difference_type;
	typedef _Pointer	pointer;
	typedef _Reference	reference;
	typedef _Category	iterator_category;
};

template<class Iterator>
struct iterator_traits
{
	typedef typename Iterator::difference_type		difference_type;
	typedef typename Iterator::value_type			value_type;
	typedef typename Iterator::pointer				pointer;
	typedef typename Iterator::reference			reference;
	typedef typename Iterator::iterator_category	iterator_category;
};

template<class T>
struct iterator_traits<T*>
{
	typedef ::std::ptrdiff_t					difference_type;
	typedef T									value_type;
	typedef T*									pointer;
	typedef T&									reference;
	typedef random_access_iterator_tag			iterator_category;
};

template<class T>
struct iterator_traits<const T*>
{
	typedef ::std::ptrdiff_t					difference_type;
	typedef T									value_type;
	typedef const T*							pointer;
	typedef const T&							reference;
	typedef random_access_iterator_tag			iterator_category;
};

// ============================================================================================== //
// -------------------------------------- reverse_iterator -------------------------------------- //
// ============================================================================================== //

template < class Iterator >
class reverse_iterator
	: public iterator
	<
		typename iterator_traits<Iterator>::iterator_category,
		typename iterator_traits<Iterator>::value_type,
		typename iterator_traits<Iterator>::difference_type,
		typename iterator_traits<Iterator>::pointer,
		typename iterator_traits<Iterator>::reference
	>
{
	protected:
		Iterator	current;

	public:

		// TESTME: Use one, the other, or no version of these?

		// My own version
		// typedef	Iterator	iterator_type;
		// using typename	iterator_traits<Iterator>::iterator_category;
		// using typename	iterator_traits<Iterator>::value_type;
		// using typename	iterator_traits<Iterator>::difference_type;
		// using typename	iterator_traits<Iterator>::pointer;
		// using typename	iterator_traits<Iterator>::reference;

		// ISO paper's version
		typedef Iterator											iterator_type;
		typedef typename iterator_traits<Iterator>::difference_type	difference_type;
		typedef typename iterator_traits<Iterator>::reference		reference;
		typedef typename iterator_traits<Iterator>::pointer			pointer;

		reverse_iterator() : current() {}
		explicit reverse_iterator(iterator_type src) : current(src) {};
		template <class Iter>
		reverse_iterator(reverse_iterator<Iter> const& src) : current(src.base()) {};

		iterator_type	base() const { return (current); }

		reference	operator* () const { return (*prev(current)); }
		pointer		operator->() const { return (&(operator*())); }
		reference	operator[](difference_type n) const { return (*prev(current, n + 1)); }

		reverse_iterator&	operator++()	{ --current; return (*this);					  };
		reverse_iterator	operator++(int)	{ reverse_iterator tmp = *this; --current; return (tmp); };
		reverse_iterator&	operator--()	{ ++current; return (*this);					  };
		reverse_iterator	operator--(int)	{ reverse_iterator tmp = *this; ++current; return (tmp); };

		reverse_iterator	operator+(difference_type n) const { return (reverse_iterator(current - n)); }
		reverse_iterator	operator-(difference_type n) const { return (reverse_iterator(current + n)); }

		reverse_iterator&	operator+=(difference_type n) { current -= n; return (*this); }
		reverse_iterator&	operator-=(difference_type n) { current += n; return (*this); }
};

/* ************************************************************************** */
/*                           OUT-OF-CLASS OPERATORS                           */
/* ************************************************************************** */

template <class Iterator>
reverse_iterator<Iterator> operator+(typename reverse_iterator<Iterator>::difference_type n,
	reverse_iterator<Iterator> const& rev_it)
{
	return (rev_it + n);
}

template <class Iter1, class Iter2>
typename reverse_iterator<Iter1>::difference_type
	operator- ( reverse_iterator<Iter1> const& lhs,
				reverse_iterator<Iter2> const& rhs)
{
	return (rhs.base() - lhs.base());
}

/* ************************************************************************** */
/*                            RELATIONAL OPERATORS                            */
/* ************************************************************************** */

template <class Iter1, class Iter2>
bool operator==(const reverse_iterator<Iter1>& lhs,
				const reverse_iterator<Iter2>& rhs)
{
	return (lhs.base() == rhs.base());
}

template <class Iter1, class Iter2>
bool operator!=(const reverse_iterator<Iter1>& lhs,
				const reverse_iterator<Iter2>& rhs)
{
	return (lhs.base() != rhs.base());
}

template <class Iter1, class Iter2>
bool operator< (const reverse_iterator<Iter1>& lhs,
				const reverse_iterator<Iter2>& rhs)
{
	return (lhs.base() > rhs.base());
}

template <class Iter1, class Iter2>
bool operator> (const reverse_iterator<Iter1>& lhs,
				const reverse_iterator<Iter2>& rhs)
{
	return (lhs.base() < rhs.base());
}

template <class Iter1, class Iter2>
bool operator<=(const reverse_iterator<Iter1>& lhs,
				const reverse_iterator<Iter2>& rhs)
{
	return (lhs.base() >= rhs.base());
}

template <class Iter1, class Iter2>
bool operator>=(const reverse_iterator<Iter1>& lhs,
				const reverse_iterator<Iter2>& rhs)
{
	return (lhs.base() <= rhs.base());
}

// ============================================================================================== //
// ---------------------------------------- FT::DISTANCE ---------------------------------------- //
// ============================================================================================== //

namespace __detail
{

	template<class It>
	static typename iterator_traits<It>::difference_type
		do_distance(It first, It last, input_iterator_tag)
	{
		typename iterator_traits<It>::difference_type retval = 0;

		for (; first != last; ++first, ++retval)
			;
		return (retval);
	}

	template<class It>
	static typename iterator_traits<It>::difference_type
		do_distance(It first, It last, random_access_iterator_tag)
	{
		return (last - first);
	}

} // namespace __detail


template<class It>
typename iterator_traits<It>::difference_type
	distance(It first, It last)
{
	return __detail::do_distance(first, last, typename iterator_traits<It>::iterator_category());
}

// ============================================================================================== //
// ---------------------------------------- FT::ADVANCE ----------------------------------------- //
// ============================================================================================== //

namespace __detail
{

	template<class InputIt>
	static void	do_advance(InputIt& it, typename iterator_traits<InputIt>::difference_type n,
							input_iterator_tag)
	{
		while (n-- > 0)
			++it;
	}

	template<class BidirIt>
	static void	do_advance(BidirIt& it, typename iterator_traits<BidirIt>::difference_type n,
							bidirectional_iterator_tag)
	{
		while (n > 0) {
			--n;
			++it;
		}
		while (n < 0) {
			++n;
			--it;
		}
	}

	template<class RandomIt>
	static void	do_advance(RandomIt& it, typename iterator_traits<RandomIt>::difference_type n,
							random_access_iterator_tag)
	{
		it += n;
	}

} // namespace __detail

template<class It, class Distance>
void advance(It& it, Distance n)
{
	__detail::do_advance(it, typename iterator_traits<It>::difference_type(n),
		typename iterator_traits<It>::iterator_category());
}

// ============================================================================================== //
// ------------------------------------------ FT::PREV ------------------------------------------ //
// ============================================================================================== //

template<class BidirIt>
BidirIt	prev(BidirIt it, typename iterator_traits<BidirIt>::difference_type n = 1)
{
	ft::advance(it, -n);
	return it;
}

}
