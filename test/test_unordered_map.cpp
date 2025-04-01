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
		ft::map_t	bar;

		foo.empty();
		foo.size();
		foo.max_size();
		foo.begin();
		foo.cbegin();
		foo.end();
		foo.cend();

		foo.insert(ft::make_pair(0, "Hello"));
		foo.insert(foo.begin(), ft::make_pair(1, ", world!"));
		foo.insert(foo.begin(), foo.end());
		foo.erase(foo.find(1));
		foo.erase(0);
		foo.erase(foo.begin(), foo.end());
		foo.clear();
		foo.swap(bar);

		foo.hash_function();
		foo.key_eq();

		foo.find(0);
		foo.count(0);
		foo.equal_range(0);
		foo[0];
		foo.at(0);

		foo.bucket_count();
		foo.max_bucket_count();
		foo.bucket_size(0);
		foo.bucket(0);
		foo.begin(0);
		foo.begin(0);
		foo.end(0);
		foo.end(0);
		foo.cbegin(0);
		foo.cend(0);

	}
	{
		ft::map_t	foo;

		ft::map_t::iterator it = foo.begin();
		ft::map_t::const_iterator cit = foo.cbegin();

		ft::map_t::const_iterator cit2 = it;

		++it;
		cit++;
		++cit2++;

		// it->first = 0;
		it->second = "foo";
	}
}
