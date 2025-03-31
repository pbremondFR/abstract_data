/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functional.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 18:57:53 by pbremond          #+#    #+#             */
/*   Updated: 2025/03/31 21:27:49 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_FUNCTIONAL
#define FT_FUNCTIONAL

#include <cstddef>

namespace ft
{

// Deprecated since C++11 but well..
template <class Arg, class Result>
struct unary_function
{
	typedef	Arg		argument_type;
	typedef	Result	result_type;
};

template <class Arg1, class Arg2, class Result>
struct binary_function
{
	typedef	Arg1	first_argument_type;
	typedef	Arg2	second_argument_type;
	typedef	Result	result_type;
};

template <class T>
struct less : binary_function<T, T, bool>
{
	inline bool	operator()(const T& x, const T& y) const { return x < y; }
};

template <class T>
struct equal_to : binary_function<T, T, bool>
{
	inline bool	operator()(const T& x, const T& y) const { return x == y; }
};

template <class T>
struct __hash_base
{
	typedef typename	std::size_t	result_type;
	typedef				T			argument_type;
};

template <class T> struct hash {};

#define HASH_SPECIALIZE(_Tp) \
	template <> \
	struct hash<_Tp> : __hash_base<_Tp> \
	{ \
		result_type operator()(argument_type x)	const throw() { return static_cast<result_type>(x); } \
	};


HASH_SPECIALIZE(bool)
HASH_SPECIALIZE(char)
HASH_SPECIALIZE(signed char)
HASH_SPECIALIZE(unsigned char)
HASH_SPECIALIZE(wchar_t)
HASH_SPECIALIZE(short)
HASH_SPECIALIZE(unsigned short)
HASH_SPECIALIZE(int)
HASH_SPECIALIZE(unsigned int)
HASH_SPECIALIZE(long)
HASH_SPECIALIZE(unsigned long)
HASH_SPECIALIZE(long long)
HASH_SPECIALIZE(unsigned long long)
HASH_SPECIALIZE(float)
HASH_SPECIALIZE(double)
HASH_SPECIALIZE(long double)

// T* (for any type T)
template <class T>
struct hash<T*> : __hash_base<T*>
{
	std::size_t operator()(T *x) const throw()	{ return reinterpret_cast<std::size_t>(x); }
};

} // namespace ft

#endif
