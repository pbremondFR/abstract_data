#include <iostream>

#include "unordered_map.hpp"
#include <string>

namespace ft {
	typedef typename ft::unordered_map<int, std::string>	map_t;
}


void	tests_multimap()
{
	{
		ft::map_t	foo;

		foo.hash_function();
	}

}
