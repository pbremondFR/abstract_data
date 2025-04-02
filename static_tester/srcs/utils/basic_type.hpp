#ifndef MONKEY_TESTER_BASIC_TYPE_HPP
# define MONKEY_TESTER_BASIC_TYPE_HPP

#include "functional.hpp"

class basic_type
{
public:
	basic_type() {}
	basic_type(const basic_type &) {}
	basic_type &operator=(const basic_type &) {return *this;}
	struct compare
	{
		compare() {}
		compare(const compare &) {}
		bool operator()(const basic_type &, const basic_type &) const {return false;}
	};
	struct unary_predicate
	{
		unary_predicate() {}
		unary_predicate(const unary_predicate &) {}
		bool operator()(const basic_type &) const {return false;}
	};
};

namespace std
{
	template <>
	struct hash<basic_type>
	{
		std::size_t operator()(const basic_type&) const	{ return 0; }
	};
}

namespace ft
{
	template <>
	struct hash<basic_type>
	{
		std::size_t operator()(const basic_type&) const	{ return 0; }
	};
}

#endif
