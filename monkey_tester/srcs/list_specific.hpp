#ifndef MONKEY_TESTER_LIST_SPECIFIC_HPP
#define MONKEY_TESTER_LIST_SPECIFIC_HPP
# include "common.hpp"
# include "utils/input_iterator.hpp"


namespace list_specific
{
	template <class C>
	void	merge(C &a, C &b)
	{
		FNAME
		std::cout << "a.sort(); C c = b; c.sort(); a.merge(c)" << std::endl;
		C c = b;
		a.sort();
		c.sort();
		a.merge(c);
		std::cout << "c:" << std::endl;
		monkey::print(c);
	}

	template <class C>
	void	splice(C &a, C &b)
	{
		FNAME
		typename C::iterator pos = monkey::get_random_it(a.begin(), a.end());
		std::cout << "a.splice(pos, b) with pos = ";
		monkey::print_it(a, pos);
		std::cout << std::endl;
		a.splice(pos, b);
	}

	template <class C>
	void	splice_range(C &a, C &b)
	{
		if (b.empty())
			return;
		FNAME
		typename C::iterator pos = monkey::get_random_it(a.begin(), a.end());
		typename C::iterator first = monkey::get_random_it(b.begin(), monkey::prev(b.end()));
		typename C::iterator last = monkey::get_random_it(first, b.end());
		std::cout << "a.splice(pos, first, last) with pos = ";
		monkey::print_it(a, pos);
		std::cout << ", first = ";
		monkey::print_it(b, first);
		std::cout << ", last = ";
		monkey::print_it(b, last);
		std::cout << std::endl;
		a.splice(pos, b, first, last);
	}

	template <class C>
	void	splice_it(C &a, C &b)
	{
		FNAME
		if (b.empty())
			return;
		typename C::iterator pos = monkey::get_random_it(a.begin(), a.end());
		typename C::iterator it = monkey::get_random_it(b.begin(), monkey::prev(b.end()));
		std::cout << "a.splice(pos, it) with pos = ";
		monkey::print_it(a, pos);
		std::cout << ", it = ";
		monkey::print_it(b, it);
		std::cout << std::endl;
		a.splice(pos, b, it);
	}

	template <class C>
	void	remove(C &a, C &)
	{
		FNAME
		typename C::value_type value((monkey::get_value()));
		std::cout << "a.remove(value) with value = [" << value << "]" << std::endl;
		a.remove(value);
	}

	template <class C>
	void	remove_if(C &a, C &)
	{
		FNAME
		typename C::value_type value((monkey::get_value()));
		std::cout << "a.remove_if(std::bind1st(std::less<std::string>(), value)) with value = [" << value << "]" << std::endl;
		a.remove_if(std::bind1st(std::less<std::string>(), value));
	}

	template <class C>
	void	reverse(C &a, C &)
	{
		FNAME
		std::cout << "a.reverse()" << std::endl;
		a.reverse();
	}

	template <class C>
	void	sort(C &a, C &)
	{
		FNAME
		std::cout << "a.sort()" << std::endl;
		a.sort();
	}

	template <class C>
	void	unique(C &a, C &)
	{
		FNAME
		std::cout << "a.unique()" << std::endl;
		a.unique();
	}

	BUILD_TABLE(
			merge<C>,
			splice<C>,
			splice_range<C>,
			splice_it<C>,
			remove<C>,
			remove_if<C>,
			reverse<C>,
			sort<C>,
			unique<C>
	)
}

#endif