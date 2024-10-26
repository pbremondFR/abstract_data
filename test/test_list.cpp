#include <iostream>

#include "list.hpp"
#include <list>

#include <iterator>

#include "StupidClass.hpp"

#ifndef NAMESP
# define NAMESP ft
#else
# define NAMESP std
#endif

#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)
#define A 2

static void	newtest(void)
{
	static int	i(1);

	std::cout << "\e[0;30;47m===============TEST " << i++ << "===============\e[0m"
		<< std::endl;
}

void	tests_list()
{
	std::cout << "Namespace is " STRINGIFY(NAMESP) "\n";
	// {
	// 	try {
	// 		StupidClass coucou;
	// 		StupidClass	foo = StupidClass(StupidClass::be_stupid());
	// 	} catch (std::bad_alloc &) {}
	// 	return;
	// }
	newtest();
	{
		NAMESP::list<int>	foo;
	}
	newtest();
	{
		NAMESP::list<int>	foo;
		foo.push_back(42);
		foo.push_front(21);
	}
	newtest();
	{
		NAMESP::list<StupidClass>	foo;
		foo.push_back(StupidClass());
		foo.push_back(StupidClass());
		foo.push_front(StupidClass());
		foo.push_back(StupidClass());
		foo.push_front(StupidClass());
		foo.push_front(StupidClass());
		foo.push_back(StupidClass());
	}
	newtest();
	{
		NAMESP::list<StupidClass>	foo;
		foo.push_front(StupidClass());
		try {	// XXX: Intentional leak. Why does it not leak with smart pointers though?
			foo.push_front(StupidClass(StupidClass::be_stupid()));
		} catch (std::bad_alloc &) {};
		foo.push_front(StupidClass());
	}
	newtest();
	{
		NAMESP::list<int>	foo;
		foo.push_back(21);
		foo.push_back(42);
		bool test = foo.begin() == foo.end();
		std::cout << std::boolalpha << "test is " << test << '\n';
	}
	newtest();
	{
		int	list_a[] = {1, 3, 5, 7, 9};
		int	list_b[] = {0, 2, 4, 6, 8};
		NAMESP::list<int>	a(list_a, list_a + 5);
		NAMESP::list<int>	b(list_b, list_b + 5);
		a.splice(a.begin(), b, b.begin(), b.end());
		for (NAMESP::list<int>::iterator it = a.begin(); it != a.end(); ++it)
			std::cout << *it << " ";
		std::cout << '\n';
	}
	newtest();
	{
		int	list_a[] = {1, 3, 5, 7, 9};
		int	list_b[] = {0, 2, 4, 6, 8};
		NAMESP::list<int>	a(list_a, list_a + 5);
		NAMESP::list<int>	b(list_b, list_b + 5);

		NAMESP::list<int>::iterator range_end = b.begin();
		range_end++;

		a.splice(a.begin(), b, b.begin(), range_end);
		for (NAMESP::list<int>::iterator it = a.begin(); it != a.end(); ++it)
			std::cout << *it << " ";
		std::cout << '\n';

	}
	newtest();
	{
		int	list_a[] = {1, 3, 5, 7, 9};
		int	list_b[] = {0, 2, 4, 6, 8};
		NAMESP::list<int>	a(list_a, list_a + 5);
		NAMESP::list<int>	b(list_b, list_b + 5);
		a.merge(b);
		for (NAMESP::list<int>::iterator it = a.begin(); it != a.end(); ++it)
			std::cout << *it << " ";
		std::cout << '\n';
	}
	newtest();
	{
		int	list_a[] = {1, 3, 5, 7, 9};
		int	list_b[] = {0, 2, 4, 6, 8};
		NAMESP::list<int>	a(list_a, list_a + 5);
		NAMESP::list<int>	b(list_b, list_b + 5);
		a.merge(b);
		a.reverse();
		for (NAMESP::list<int>::iterator it = a.begin(); it != a.end(); ++it)
			std::cout << "[" << *it << "] ";
		std::cout << '\n';

		for (NAMESP::list<int>::reverse_iterator it = a.rbegin(); it != a.rend(); ++it)
			std::cout << "[" << *it << "] ";
		std::cout << '\n';
	}
	newtest();
	{
		int	list_a[] = {5, 4, 3, 2, 1};
		NAMESP::list<int>	a(list_a, list_a + 5);
		a.sort();
		for (NAMESP::list<int>::iterator it = a.begin(); it != a.end(); ++it)
			std::cout << "[" << *it << "] ";
		std::cout << '\n';
	}
}
