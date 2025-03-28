/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_traits.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 06:44:44 by pbremond          #+#    #+#             */
/*   Updated: 2025/03/28 16:49:51 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "iterator.hpp"

namespace ft
{

template < bool B, class T = void >
struct enable_if
{
};

template < class T >
struct enable_if<true, T>
{
	typedef	T	type;
};

template < class T, T v >
struct integral_constant {
	static const	T	value = v;

	typedef	T						value_type;
	typedef	integral_constant<T,v>	type;

	operator T() const { return (v); }
};

typedef integral_constant<bool, true>	true_type;
typedef integral_constant<bool, false>	false_type;

template < class T, class U >
struct is_same : false_type
{
};

template < class T >
struct is_same<T, T> : true_type
{
};

// is_integral declaration and specialisation
template <class T>	struct is_integral						: public false_type {};
template <>			struct is_integral<bool>				: public true_type {};
template <>			struct is_integral<char>				: public true_type {};
template <>			struct is_integral<unsigned char>		: public true_type {};
template <>			struct is_integral<wchar_t>				: public true_type {};
// NOTE: These do not exist until C11
// template <>			struct is_integral<char16_t>			: public true_type {};
// template <>			struct is_integral<char32_t>			: public true_type {};
template <>			struct is_integral<short>				: public true_type {};
template <>			struct is_integral<unsigned short>		: public true_type {};
template <>			struct is_integral<int>					: public true_type {};
template <>			struct is_integral<unsigned int>		: public true_type {};
template <>			struct is_integral<long>				: public true_type {};
template <>			struct is_integral<unsigned long>		: public true_type {};
template <>			struct is_integral<long long>			: public true_type {};
template <>			struct is_integral<unsigned long long>	: public true_type {};
template <>			struct is_integral<__int128_t>			: public true_type {};
template <>			struct is_integral<__uint128_t>			: public true_type {};

// is_floating_point declaration and specialisation
template <class T>	struct is_floating_point				: public false_type {};
template <>			struct is_floating_point<float>			: public true_type {};
template <>			struct is_floating_point<double>		: public true_type {};
template <>			struct is_floating_point<long double>	: public true_type {};

template < class T >
struct is_fundamental
	: integral_constant
	<
		bool,
		is_integral<T>::value
		|| is_floating_point<T>::value
	>
{};

template < class I >
struct is_input_iterator
	: integral_constant
	<
		bool,
		is_same<typename I::iterator_category, input_iterator_tag>::value
		|| is_same<typename I::iterator_category, forward_iterator_tag>::value
		|| is_same<typename I::iterator_category, bidirectional_iterator_tag>::value
		|| is_same<typename I::iterator_category, random_access_iterator_tag>::value
	>
{};

template < class I >
struct is_output_iterator
	: integral_constant
	<
		bool,
		is_same<typename I::iterator_category, output_iterator_tag>::value
		|| is_same<typename I::iterator_category, forward_iterator_tag>::value
		|| is_same<typename I::iterator_category, bidirectional_iterator_tag>::value
		|| is_same<typename I::iterator_category, random_access_iterator_tag>::value
	>
{};

template < class I >
struct is_forward_iterator
	: integral_constant
	<
		bool,
		is_same<typename I::iterator_category, forward_iterator_tag>::value
		|| is_same<typename I::iterator_category, bidirectional_iterator_tag>::value
		|| is_same<typename I::iterator_category, random_access_iterator_tag>::value
	>
{};

}
