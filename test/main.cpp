/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 16:44:17 by pbremond          #+#    #+#             */
/*   Updated: 2025/03/29 16:17:34 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>

#include <vector>
#include "vector.hpp"

#include <stack>
#include "stack.hpp"

#include <utility>
#include "utility.hpp"

#include <map>
#include "map.hpp"

#ifdef BONUS
# include <set>
# include "set.hpp"
#endif

#include <list>
#include <cstdlib>

#include "ansi_color.h"
// #include "optional.hpp"

#ifndef NAMESP
# define NAMESP ft
#else
# define NAMESP std
#endif

static void	newtest(void)
{
	static int	i(1);

	std::cout << "\e[0;30;47m===============TEST " << i++ << "===============\e[0m"
		<< std::endl;
}

#include "utils.tpp"

void	lmartin_main(void);	// Never forget
void	tests_list();
void	tests_multimap();

int	main(void)
{
	std::srand(1337);

	// tests_list();
	// return 0;
	tests_multimap();
	return 0;

	newtest();
	try
	{
		NAMESP::vector<char>	test(9, 'a');
		NAMESP::vector<char>	test2(8);
		std::cout << "Capacity: " << test.capacity() << std::endl;
		test.assign(test.begin(), test.end());
		std::cout << "Capacity: " << test2.capacity() << std::endl;
		// test2.push_back('b');
		std::cout << "Capacity: " << test2.capacity() << std::endl;
		test2.insert(test2.begin(), '0');
		std::cout << "Capacity: " << test2.capacity() << std::endl;
		// std::cout << "prout " << sizeof(char) << std::endl;
		// test.reserve(ULLONG_MAX);
		// std::iterator_traits
	}
	catch (std::exception &e)
	{
		std::cout << "Exception caught: " << e.what() << std::endl;
	}
	newtest();
	try
	{
		std::vector<char>	test(9, 'a');
		ft::vector<char>	test2(8);
		std::cout << "Capacity: " << test.capacity() << std::endl;
		test2.assign(test.begin(), test.end());
		std::cout << "Capacity: " << test2.capacity() << std::endl;
		// test2.push_back('b');
		std::cout << "Capacity: " << test2.capacity() << std::endl;
		// test.insert(test2.begin(), '0');
		test.insert(test.begin(), '0');
		test2.insert(test2.begin(), '0');
		std::cout << "Capacity: " << test2.capacity() << std::endl;
		// std::cout << "prout " << sizeof(char) << std::endl;
		// test.reserve(ULLONG_MAX);
	}
	catch (std::exception &e)
	{
		std::cout << "Exception caught: " << e.what() << std::endl;
	}
	newtest();
	try
	{
		NAMESP::vector<char>	test(8, '0');

		char	truc = 'a';
		for (NAMESP::vector<char>::iterator itr = test.begin(); itr != test.end(); ++itr) {
			*itr = truc++;
			std::cout << *itr << std::endl;
		}
		std::cout << "---------------------------------" << std::endl;
		for (NAMESP::vector<char>::const_iterator itr = test.end() - 1; itr != test.begin() - 1; --itr)
			std::cout << (*itr) << std::endl;
		std::cout << "---------------------------------" << std::endl;
		test.push_back('j');
		test.insert(test.end() - 1, 'i');
		test.insert(test.begin(), '0');
		std::cout << "capacity: " << test.capacity() << std::endl;
		for (NAMESP::vector<char>::const_iterator itr = test.begin(); itr != test.end(); ++itr)
			std::cout << *itr << std::endl;
		std::cout << "begin: " << *test.begin() << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << "Exception caught: " << e.what() << std::endl;
	}
	newtest();
	try
	{
		NAMESP::vector<char>	test(8, '-');
		NAMESP::vector<char>	test2;

		char	truc = '0';
		for (NAMESP::vector<char>::iterator itr = test.begin(); itr != test.end(); ++itr)
			std::cout << (*itr = truc++) << std::endl;
		test2.assign(test.begin(), test.end());
		std::cout << "------------" << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << "Exception caught: " << e.what() << std::endl;
	}
	newtest();
	try
	{
		std::string				truc = "0123456789";
		NAMESP::vector<char>	test(truc.begin(), truc.end());

		fillWithAscii(test);
		NAMESP::vector<char>::iterator	itr1 = test.begin();
		// ft::vector<char>::iterator	itr2 = itr1 + 5;
		NAMESP::vector<char>::iterator	itr2 = 5 + itr1;
		std::cout << "itr1: " << *itr1 << '\n'
			<< "itr2: " << *itr2 << std::endl;

	}
	catch(const std::exception& e)
	{
		std::cerr << "Exception caught: " << e.what() << std::endl;
	}
	newtest();
	try
	{
		NAMESP::vector<char>	test1(3, 'a');
		NAMESP::vector<char>	test2(3, 'b');

		printContainer("test1", test1);
		printContainer("test2", test2);
		std::swap(test1, test2);
		// using std::swap;
		// swap(test1, test2);
		printContainer("test1", test1);
		printContainer("test2", test2);
	}
	catch(const std::exception& e)
	{
		std::cerr << "Exception caught: " << e.what() << std::endl;
	}
	newtest();
	try
	{
		NAMESP::vector<char>	test1(10, 'o');
		NAMESP::vector<char>	test2(test1);

		test1.erase(test1.begin() + 1, test1.end() - 1);
		printContainer("test1", test1);
	}
	catch(const std::exception& e)
	{
		std::cerr << "Exception caught: " << e.what() << std::endl;
	}
	newtest();
	try
	{
		NAMESP::pair<int, char>		pair1('a', 'a');
		NAMESP::pair<int, char>		pair2(pair1);

		std::cout << "First: " << pair1.first << '\n'
			<< "Second: " << pair1.second << std::endl;
		std::cout << (pair1 == pair2 ? "TRUE" : "FALSE") << std::endl;
		pair2.first++;
		std::cout << (pair1 == pair2 ? "TRUE" : "FALSE") << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << "Exception caught: " << e.what() << std::endl;
	}
	// newtest();
	// {
	// 	ft::optional<int>	opt(21);
	// 	std::cout << "Has value: " << opt.has_value() << std::endl;
	// 	std::cout << "value_or(-42): " << opt.value_or(-42) << std::endl;
	// 	opt = ft::nullopt;
	// 	std::cout << "Has value: " << opt.has_value() << std::endl;
	// 	opt.reset();
	// 	std::cout << "Has value: " << opt.has_value() << std::endl;
	// }
	// newtest();
	// {
	// 	ft::optional<std::string>	opt("Hello world");
	// 	std::cout << "Has value: " << opt.has_value() << std::endl;
	// 	std::cout << "value_or(\"merde\"): " << opt.value_or("merde") << std::endl;
	// 	opt = ft::nullopt;
	// 	std::cout << "Has value: " << opt.has_value() << std::endl;
	// 	opt.reset();
	// 	std::cout << "Has value: " << opt.has_value() << std::endl;
	// }
	newtest();
	{
		NAMESP::map<int, char>	test;
		test.insert(NAMESP::make_pair(42, 'a'));
		NAMESP::map<int, char>::iterator it = test.begin();
		std::cout << (*it).second << std::endl;
		std::cout << it.operator->() << std::endl;
		std::cout << it.operator*().first << std::endl;
		it->second = 'b';
		std::cout << it->second << std::endl;
		// it->first = 'b';
	}
	newtest();
	{
		NAMESP::map<int, std::string>	test;
		test.insert(NAMESP::make_pair(30, "30"));
		test.insert(NAMESP::make_pair(20, "20"));
		test.insert(NAMESP::make_pair(70, "70"));
		test.insert(NAMESP::make_pair(10, "10"));
		test.insert(NAMESP::make_pair(50, "50"));
		test.insert(NAMESP::make_pair(40, "40"));
		test.insert(NAMESP::make_pair(60, "60"));

		printMap("map", test);
	}
	newtest();
	{
		NAMESP::map<int, std::string>	test;
		test.insert(NAMESP::make_pair(10, "10"));
		test.insert(NAMESP::make_pair(5, "5"));
		test.insert(NAMESP::make_pair(15, "15"));
		test.insert(NAMESP::make_pair(2, "2"));
		test.insert(NAMESP::make_pair(7, "7"));
		test.insert(NAMESP::make_pair(6, "6"));
		test.insert(NAMESP::make_pair(8, "8"));

		printMap("map - test", test);
		// test.debug_printByLevel();

		// test.debug_leftRotate(5);

		printMap("map - test", test);
		// test.debug_printByLevel();

		// test.debug_rightRotate(7);

		printMap("map - test", test);
		// test.debug_printByLevel();
	}
	newtest();
	{
		NAMESP::map<int, std::string>	test;
		test.insert(NAMESP::make_pair(10, "10"));
		test.insert(NAMESP::make_pair(5, "5"));
		test.insert(NAMESP::make_pair(15, "15"));
		test.insert(NAMESP::make_pair(2, "2"));
		test.insert(NAMESP::make_pair(7, "7"));
		test.insert(NAMESP::make_pair(6, "6"));
		test.insert(NAMESP::make_pair(8, "8"));
		test.insert(NAMESP::make_pair(1, "1"));
		test.insert(NAMESP::make_pair(0, "0"));

		printMap("map - test", test);
		// test.debug_printByLevel();
		// test.debug_printFamily(10);
		// test.debug_printFamily(15);
	}
	newtest();
	{
		NAMESP::map<int, std::string>	test;
		test.insert(NAMESP::make_pair(10, "10"));
		test.insert(NAMESP::make_pair(5, "5"));
		test.insert(NAMESP::make_pair(15, "15"));
		test.insert(NAMESP::make_pair(2, "2"));
		test.insert(NAMESP::make_pair(7, "7"));
		test.insert(NAMESP::make_pair(6, "6"));
		test.insert(NAMESP::make_pair(8, "8"));
		test.insert(NAMESP::make_pair(1, "1"));
		printMap("map - test", test);
		std::cout << test.insert(test.find(1), NAMESP::make_pair(0, "0"))->first << std::endl;

		printMap("map - test", test);
		// test.debug_printByLevel();
		// test.debug_printFamily(10);
		// test.debug_printFamily(15);
	}
	newtest();
	{
		NAMESP::map<int, int>	test;
		std::cout << "With size 0, expecting begin() == end()\n"
			<< (test.begin() == test.end() ?
				_GRN "PASSED: begin() == end()" RESET
				: _RED "FAILED: begin() != end()" RESET)
			<< std::endl;
	}
	newtest();
	{
		NAMESP::map<int, int>	test;
		test.insert(NAMESP::make_pair(42, 42));
		NAMESP::map<int, int>::iterator	it = test.end();
		it--;
		std::cout << it->first << " | " << it->second << std::endl;
	}
	newtest();
	try
	{
		NAMESP::vector<std::string>	test;
		test.push_back("A");
		test.push_back("BB");
		test.push_back("CCC");
		test.push_back("DDDD");

		test.erase(test.begin() + 2);
		// test1.erase(test1.begin() + 1, test1.end() - 1);
		printContainer("test", test);
	}
	catch(const std::exception& e)
	{
		std::cerr << "Exception caught: " << e.what() << std::endl;
	}
	// newtest();
	// {
	// 	lmartin_main();
	// }
	newtest();
	{
		NAMESP::map<char, int>	test;

		test['a'];
		test['b'];
		test['c'];
		test['d'];
		test['e'];
		test['f'];
		test['h'];

		test.insert(test.find('f'), NAMESP::make_pair('g', 1337));
		printMap("test", test);
	}
	newtest();
	{
		std::string	input1 = "ABCDEFGHIJ";
		std::string input2 = "KLMNOPQRST";
		std::list<NAMESP::pair<char, char> >	inputA;
		std::list<NAMESP::pair<char, char> >	inputB;
		NAMESP::map<char, char>	test1;
		NAMESP::map<char, char>	test2;

		for (std::string::iterator it = input1.begin(); it != input1.end(); ++it)
			inputA.push_back(NAMESP::make_pair(*it, *it));
		for (std::string::iterator it = input2.begin(); it != input2.end(); ++it)
			inputB.push_back(NAMESP::make_pair(*it, *it));
		test1.insert(inputA.begin(), inputA.end());
		test2.insert(inputB.begin(), inputB.end());

		printMap("Test 1", test1);
		printMap("Test 2", test2);
		std::swap(test1, test2);
		printMap("Test 1", test1);
		printMap("Test 2", test2);
	}
	newtest();
	{ // Enable vector debugging information to see the results !
		#if VEC_DEBUG_VERBOSE == false
			std::cout << "Enable vector debugging information to see results of this test." << std::endl;
		#endif
		NAMESP::vector<std::string>	testOptiSFINAE;
		testOptiSFINAE.push_back("CECI EST UN TEST MDR");
		testOptiSFINAE.insert(testOptiSFINAE.begin(), "CECI EN EST UN AUTRE LOL");
	}
	const int size = 1e6;
	newtest();
	{

		{
		NAMESP::map<int, char>	mapperf;
		fillMapWithRand(size, mapperf);
		std::cout << "Filled map, " << size - mapperf.size() << " elements failed to insert" << std::endl;
		}
		{
		#ifdef BONUS
		NAMESP::set<int>	setperf;
		fillSetWithRand(size, setperf);
		std::cout << "Filled set, " << size - setperf.size() << " elements failed to insert" << std::endl;
		#endif
		}
		{
		NAMESP::vector<int>	vecperf;
		fillVecWithRand(size, vecperf);
		std::cout << "Filled vector, " << size - vecperf.size() << " elements failed to insert" << std::endl;
		}
		{
		NAMESP::stack<int>	stackperf;
		fillStackWithRand(size, stackperf);
		std::cout << "Filled stack, " << size - stackperf.size() << " elements failed to insert" << std::endl;
		}
	}
	newtest();
	{

		{
		NAMESP::map<int, std::string>	mapperf;
		for (std::size_t i = 0; i < size; ++i)
			mapperf.insert(NAMESP::make_pair(std::rand(), "According to all known laws of aviation, there is no way that a bee should be able to fly. Its wings are too small to get its fat little body off the ground. The bee, of course, flies anyways. Because bees don't care what humans think is impossible."));
		std::cout << "Filled map, " << size - mapperf.size() << " elements failed to insert" << std::endl;
		}
		{
		NAMESP::vector<std::string>	vecperf;
		for (std::size_t i = 0; i < size; ++i)
			vecperf.push_back("According to all known laws of aviation, there is no way that a bee should be able to fly. Its wings are too small to get its fat little body off the ground. The bee, of course, flies anyways. Because bees don't care what humans think is impossible.");
		std::cout << "Filled vector, " << size - vecperf.size() << " elements failed to insert" << std::endl;
		}
		{
		NAMESP::stack<std::string>	stackperf;
		for (std::size_t i = 0; i < size; ++i)
			stackperf.push("According to all known laws of aviation, there is no way that a bee should be able to fly. Its wings are too small to get its fat little body off the ground. The bee, of course, flies anyways. Because bees don't care what humans think is impossible.");
		std::cout << "Filled stack, " << size - stackperf.size() << " elements failed to insert" << std::endl;
		}
	}
	newtest();
	{
		NAMESP::map<int, char>	mapperf;
		for (std::size_t i = 0; i < size; ++i) {
			mapperf.insert(NAMESP::make_pair(i, 'A'));
		}
		std::cout << "Filled map, " << size - mapperf.size() << " elements failed to insert" << std::endl;
	}
	// newtest();
	// {
	// 	NAMESP::vector<int>	vecperf;
	// 	for (std::size_t i = 0; i < 1e5; ++i) {
	// 		vecperf.insert( vecperf.begin(), i );
	// 	}
	// 	std::cout << "Filled vector, " << 1e5 - vecperf.size() << " elements failed to insert" << std::endl;
	// }
	// newtest();
	// {
	// 	NAMESP::vector<std::string>	vecperf;
	// 	for (std::size_t i = 0; i < 1e5; ++i) {
	// 		vecperf.insert( vecperf.begin(), "According to all known laws of aviation, there is no way that a bee should be able to fly. Its wings are too small to get its fat little body off the ground. The bee, of course, flies anyways. Because bees don't care what humans think is impossible." );
	// 	}
	// 	std::cout << "Filled vector, " << 1e5 - vecperf.size() << " elements failed to insert" << std::endl;
	// }
	newtest();
	{
		NAMESP::vector<std::string>	test;
		test.insert(test.begin(), "A");
		test.insert(test.begin(), "BB");
		test.insert(test.begin(), "CCC");
		test.insert(test.begin(), "DDDD");
		test.insert(test.begin(), "EEEEE");
		printContainer("test", test);
	}
	return (0);
}
