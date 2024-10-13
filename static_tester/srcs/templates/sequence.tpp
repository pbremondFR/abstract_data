#ifndef MONKEY_TESTER_SEQUENCE_TPP
# define MONKEY_TESTER_SEQUENCE_TPP
# include "../utils/dummy_iterators.hpp"

template <class X, typename T>
void sequence(const T t)
{
	typename X::size_type n = 0;
	dummy_input_iterator<T> i(t), j(t);

	(void)X(n, t);
	X a(n, t);
	const X const_a(n, t);
	(void)a;
	(void)const_a;
	(void)X(i, j);
	X b(i, j);
	(void)b;

	a.insert(a.begin(), t);
	static_assert(std::is_same<decltype(a.insert(a.begin(), t)), typename X::iterator>::value, "Wrong return type for insert(it, value)");
	a.insert(a.begin(), n, t);
	static_assert(std::is_same<decltype(a.insert(a.begin(), n, t)), void>::value, "Wrong return type for insert(it, count, value)");
	a.insert(a.begin(), i, j);
	static_assert(std::is_same<decltype(a.insert(a.begin(), i, j)), void>::value, "Wrong return type for insert(it, range)");
	a.erase(a.begin());
	static_assert(std::is_same<decltype(a.erase(a.begin())), typename X::iterator>::value, "Wrong return type for erase(it)");
	a.erase(a.begin(), a.begin());
	static_assert(std::is_same<decltype(a.erase(a.begin(), a.begin())), typename X::iterator>::value, "Wrong return type for erase(it1, it2)");
}

#endif