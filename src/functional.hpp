/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functional.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 18:57:53 by pbremond          #+#    #+#             */
/*   Updated: 2024/07/23 23:01:36 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_FUNCTIONAL
#define FT_FUNCTIONAL

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
struct __hash_base
{
	typedef typename	std::size_t	result_type;
	typedef				T			argument_type;
};

template <class T> struct hash {};

template <>
struct hash<bool> : __hash_base<bool>
{
	result_type operator()(argument_type x)	{ return static_cast<result_type>(x); }
};

template <>
struct hash<char> : __hash_base<char>
{
	result_type operator()(argument_type x)	{ return static_cast<result_type>(x); }
};

template <>
struct hash<signed char> : __hash_base<signed char>
{
	result_type operator()(argument_type x)	{ return static_cast<result_type>(x); }
};

template <>
struct hash<unsigned char> : __hash_base<unsigned char>
{
	result_type operator()(argument_type x)	{ return static_cast<result_type>(x); }
};

// template <>
// struct hash<char16_t> : __hash_base<char16_t>
// {
// 	result_type operator()(argument_type x)	{ return static_cast<result_type>(x); }
// };

// template <>
// struct hash<char32_t> : __hash_base<char32_t>
// {
// 	result_type operator()(argument_type x)	{ return static_cast<result_type>(x); }
// };

template <>
struct hash<wchar_t> : __hash_base<wchar_t>
{
	result_type operator()(argument_type x)	{ return static_cast<result_type>(x); }
};

template <>
struct hash<short> : __hash_base<short>
{
	result_type operator()(argument_type x)	{ return static_cast<result_type>(x); }
};

template <>
struct hash<unsigned short> : __hash_base<unsigned short>
{
	result_type operator()(argument_type x)	{ return static_cast<result_type>(x); }
};

template <>
struct hash<int> : __hash_base<int>
{
	result_type operator()(argument_type x)	{ return static_cast<result_type>(x); }
};

template <>
struct hash<unsigned int> : __hash_base<unsigned int>
{
	result_type operator()(argument_type x)	{ return static_cast<result_type>(x); }
};

template <>
struct hash<long> : __hash_base<long>
{
	result_type operator()(argument_type x)	{ return static_cast<result_type>(x); }
};

template <>
struct hash<unsigned long> : __hash_base<unsigned long>
{
	result_type operator()(argument_type x)	{ return static_cast<result_type>(x); }
};

template <>
struct hash<long long> : __hash_base<long long>
{
	result_type operator()(argument_type x)	{ return static_cast<result_type>(x); }
};

template <>
struct hash<unsigned long long> : __hash_base<unsigned long long>
{
	result_type operator()(argument_type x)	{ return static_cast<result_type>(x); }
};

template <>
struct hash<float> : __hash_base<float>
{
	result_type operator()(argument_type x)	{ return static_cast<result_type>(x); }
};

template <>
struct hash<double> : __hash_base<double>
{
	result_type operator()(argument_type x)	{ return static_cast<result_type>(x); }
};

template <>
struct hash<long double> : __hash_base<long double>
{
	result_type operator()(argument_type x)	{ return static_cast<result_type>(x); }
};

template <class T>
struct hash<T*> : __hash_base<T*>
{
	std::size_t operator()(T *x)	{ return reinterpret_cast<std::size_t>(x); }
};


// T* (for any type T)

} // namespace ft

#endif
