#pragma once
#include <iostream>
#include <string>

#include <vector>
#include "vector.hpp"

#include <stack>
#include "stack.hpp"

#include <utility>
#include "utility.hpp"

#ifndef NAMESP
# define NAMESP ft
#endif

template < class T >
void	fillWithAscii(T container)
{
	char	truc = '0';
	for (typename T::iterator itr = container.begin(); itr != container.end(); ++itr)
		*itr = truc++;
}

template < class T >
void	printContainer(std::string const& name, T const& container)
{
	std::cout << name << ": " << std::endl;
	for (typename T::const_iterator itr = container.begin(); itr != container.end(); ++itr)
		std::cout << "- " << *itr << std::endl;
}

template < class T >
void	printMap(std::string const& name, T const& container)
{
	std::cout << name << ": " << std::endl;
	for (typename T::const_iterator itr = container.begin(); itr != container.end(); ++itr)
		std::cout << "- " << itr->first << " | " << itr->second << std::endl;
}

// template <class Key, class T>
// void	print(NAMESP::map<Key, T>& lst)
// {
// 	for (typename NAMESP::map<Key, T>::iterator it = lst.begin(); it != lst.end(); it++)
// 		std::cout << *it << ' ';
// 	std::cout << '\n';
// }

template <class T>
void	fillStackWithRand(std::size_t amount, NAMESP::stack<T>& stack)
{
	for (std::size_t i = 0; i < amount; ++i) {
		stack.push( std::rand() );
	}
}

template <class T>
void	fillVecWithRand(std::size_t amount, NAMESP::vector<T>& vec)
{
	for (std::size_t i = 0; i < amount; ++i) {
		vec.push_back( T(std::rand()) );
	}
}

// template <class Key, class T>
// void	fillMapWithRand(std::size_t amount, NAMESP::map<Key, T>& map)
// {
// 	for (std::size_t i = 0; i < amount; ++i) {
// 		map.insert(NAMESP::make_pair( Key(std::rand()), T() ));
// 	}
// }

#ifdef BONUS
template <class Key>
void	fillSetWithRand(std::size_t amount, NAMESP::set<Key>& set)
{
	for (std::size_t i = 0; i < amount; ++i) {
		set.insert( Key(std::rand()) );
	}
}
#endif
