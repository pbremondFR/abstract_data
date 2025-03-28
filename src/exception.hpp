/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exception.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 21:17:20 by pbremond          #+#    #+#             */
/*   Updated: 2025/03/28 14:27:28 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_EXCEPTION
#define FT_EXCEPTION

#define NOEXCEPT throw()

#include <string>

namespace ft
{

class exception
{
public:
	exception() NOEXCEPT {}
	exception(const exception&) NOEXCEPT {}
	exception& operator=(const exception&) NOEXCEPT { return *this; }
	virtual ~exception() {}
	virtual inline const char* what() const NOEXCEPT	{ return "std::exception"; }
};

class logic_error : public exception
{
public:
	explicit logic_error(const std::string& what_arg) : _what(what_arg) {}
	virtual inline const char* what() const NOEXCEPT	{ return _what.c_str(); }

private:
	std::string	_what;
};

class out_of_range : public logic_error
{
public:
	explicit out_of_range(const std::string& what_arg) : logic_error(what_arg) {}
};

class length_error : public logic_error
{
public:
	explicit length_error(const std::string& what_arg) : logic_error(what_arg) {}
};

} // namespace ft

#undef NOEXCEPT

#endif
