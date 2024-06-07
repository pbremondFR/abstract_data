/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exception.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 21:17:20 by pbremond          #+#    #+#             */
/*   Updated: 2024/06/07 21:32:24 by pbremond         ###   ########.fr       */
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
	exception() NOEXCEPT;
	// Let the compiler handle these ones?
	// exception(const exception&) NOEXCEPT;
	// exception& operator=(const exception&) NOEXCEPT;
	virtual ~exception() NOEXCEPT;
	virtual inline const char* what() const NOEXCEPT	{ return "std::exception"; }
};

} // namespace ft

#undef NOEXCEPT

#endif
