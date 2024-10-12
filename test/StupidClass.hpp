#pragma once
#include <stdint.h>	// lmao, cstdint is C++11...
#include <cstddef>
#include <stdexcept>
#include <iostream>

/*
 * This class is very stupid. Indeed, it manually manages its resources instead of
 * using RAII, and it fails one of its allocs if you tell it to. Damned!
 * Nonetheless, it is conform to the standard and doesn't leak resources by itself.
 * Use it to make sure containers don't leak.
 */
class StupidClass
{
private:
	int	*one;
	int	*two;

public:
	struct be_stupid {};

	StupidClass() : one(NULL), two(NULL)
	{
		std::cout << "Constructor\n";
		one = new int[42];
		two = new int[42];
	}
	StupidClass(const StupidClass &other) : one(NULL), two(NULL)
	{
		std::cout << "Copy Constructor\n";
		if (other.one)
			one = new int[42];
		if (other.two)
			two = new int[42];
	}
	StupidClass& operator=(StupidClass const& other)
	{
		std::cout << "Copy operator\n";
		delete[] one;
		delete[] two;
		if (other.one)
			one = new int[42];
		if (other.two)
			two = new int[42];
		return *this;
	}
	explicit StupidClass(be_stupid) : one(NULL), two(NULL)
	{
		std::cout << "Stupid Constructor\n";
		one = new int[42];
		throw std::bad_alloc();
		// two = new int[42];	// Unreachable
	}
	~StupidClass()
	{
		std::cout << "Destructor\n";
		delete[] one;
		delete[] two;
	}
	void	bar() const {
		std::cout << one << " - " << two << '\n';
	}
};
