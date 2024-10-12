#include "StupidClass.hpp"
#include <memory>

class SmartClass
{
private:
	std::unique_ptr<int>	one;
	std::unique_ptr<int>	two;

public:
	struct be_stupid {};
	SmartClass()
	{
		one = std::make_unique<int>(42);
		two = std::make_unique<int>(42);
	}
	explicit SmartClass(be_stupid)
	{
		one = std::make_unique<int>(42);
		throw std::bad_alloc();
		// two = std::make_unique<int>(42);
	}
};

#include <list>

int	main()
{
	{
		SmartClass	foo(SmartClass::be_stupid);
	}
	{
		std::list<SmartClass>	foo;
		foo.push_back(SmartClass());
		try {
			foo.push_back(SmartClass(SmartClass::be_stupid()));
		} catch (std::bad_alloc &) {}
		foo.push_back(SmartClass());
	}
}
