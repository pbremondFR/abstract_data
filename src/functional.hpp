/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functional.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 18:57:53 by pbremond          #+#    #+#             */
/*   Updated: 2025/04/03 19:43:05 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_FUNCTIONAL
#define FT_FUNCTIONAL

#include <cstddef>
#include <string>

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

namespace detail
{
	template <class T>
	struct __hash_base
	{
		typedef typename	std::size_t	result_type;
		typedef				T			argument_type;
	};
} // namespace detail


template <class T> struct hash {};

#define HASH_SPECIALIZE(_Tp) \
	template <> \
	struct hash<_Tp> : detail::__hash_base<_Tp> \
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

#undef HASH_SPECIALIZE

// T* (for any type T)
template <class T>
struct hash<T*> : detail::__hash_base<T*>
{
	std::size_t operator()(T *x) const throw()	{ return reinterpret_cast<std::size_t>(x); }
};

// Some pretty basic and probably bad LCG-based hashing algorithm
template <>
struct hash<std::string> : detail::__hash_base<std::string>
{
	std::size_t	operator()(std::string const& s) const throw()
	{
		const std::size_t a = 12345;
		const std::size_t m = 999999999;
		size_t result = 0;
		for (size_t i = 0; i < s.length(); ++i)
			result += (a * i + s[i]) % m;
		return result;
	}
};

} // namespace ft

#endif
