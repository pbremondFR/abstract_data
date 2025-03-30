/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unordered_map.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 16:19:34 by pbremond          #+#    #+#             */
/*   Updated: 2025/03/30 17:28:50 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

namespace ft {
	template<
		class Key,
		class T,
		class Hash,
		class Pred,
		class Allocator
	>
	class unordered_map;
}

#include "stl_unordered_map.hpp"
// #include "stl_unordered_multimap.hpp"
