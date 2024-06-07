/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functional.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 18:57:53 by pbremond          #+#    #+#             */
/*   Updated: 2024/06/07 18:58:26 by pbremond         ###   ########.fr       */
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

} // namespace ft

#endif
