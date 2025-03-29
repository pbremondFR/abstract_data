#include <iostream>

#include "map.hpp"
#include <map>
#include <string>

namespace ft {
	typedef typename ft::multimap<std::string, std::string>	map_t;
}

namespace std {
	typedef typename std::multimap<string, string>			map_t;
}

void	tests_multimap()
{
	{
		ft::multimap<std::string, std::string>	ft;
		std::multimap<std::string, std::string>	std;

		ft.insert(ft::make_pair("aaaa", "1111"));
		ft.insert(ft::make_pair("aaaa", "2222"));
		ft.insert(ft::make_pair("aaaa", "3333"));
		ft.insert(ft::make_pair("bbbb", "1111"));

		std.insert(std::make_pair("aaaa", "1111"));
		std.insert(std::make_pair("aaaa", "2222"));
		std.insert(std::make_pair("aaaa", "3333"));
		std.insert(std::make_pair("bbbb", "1111"));

		ft.erase("aaaa");
		std.erase("aaaa");

		for (ft::map_t::const_iterator i = ft.begin(); i != ft.end(); ++i)
			std::cout << i->first << " | " << i->second << '\n';
		std::cout << std::endl;
		for (std::map_t::const_iterator i = std.begin(); i != std.end(); ++i)
			std::cout << i->first << " | " << i->second << '\n';
	}

}
