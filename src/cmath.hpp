/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmath.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 00:14:53 by pbremond          #+#    #+#             */
/*   Updated: 2025/04/01 00:16:26 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// FIXME: Remove this header and references to the std namespace!
#include <cmath>

namespace ft
{

float	ceil(float num)				{ return std::ceil(num); }
double	ceil(double num)			{ return std::ceil(num); }
long double	ceil(long double num)	{ return std::ceil(num); }

} // namespace ft
