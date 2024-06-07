/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stdexcept.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 21:27:33 by pbremond          #+#    #+#             */
/*   Updated: 2024/06/07 21:32:21 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_STDEXCEPT
#define FT_STDEXCEPT

#include <exception.hpp>
#include <stdexcept>
// std::logic_error

#define NOEXCEPT throw()

namespace ft
{

class logic_error : public exception
{
private:
	::std::string	_msg;

public:
	logic_error() NOEXCEPT;
	explicit logic_error(const ::std::string& what_arg) : _msg(what_arg) {}
	virtual ~logic_error() NOEXCEPT {}
	virtual inline const char* what() const NOEXCEPT	{ return _msg.c_str(); }
};

} // namespace ft

#undef NOEXCEPT

#endif
