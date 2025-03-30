#include <iostream>

#include "unordered_map.hpp"
#include <string>

namespace ft {
	typedef ft::unordered_map<int, std::string>	map_t;
}


void	tests_multimap()
{
	{
		ft::map_t	foo;

		foo.hash_function();
		foo.key_eq();
		foo.empty();
		foo.size();
		foo.max_size();
		foo.begin();
		foo.cbegin();
		foo.end();
		foo.cend();
		foo.insert(ft::make_pair(0, "Hello"));
	}

}
