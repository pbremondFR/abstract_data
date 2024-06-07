#include <vector>
#include <functional>
#include <functional.hpp>

namespace ft {

// TODO: replace std members
template <class T, class Container = std::vector<T>, class Compare = less<typename Container::value_type> >
class priority_queue {
public:
	typedef typename	Container::value_type	value_type;
	typedef typename	Container::size_type	size_type;
	typedef				Container				container_type;

protected:
	Container c;
	Compare comp;

public:
	explicit inline priority_queue(const Compare& x = Compare(), const Container& cont = Container()) : c(cont), comp(x)
	{
		std::make_heap(c.begin(), c.end(), comp);
	}

	template <class InputIterator>
	inline priority_queue(InputIterator first, InputIterator last, const Compare& x = Compare(), const Container& cont = Container())
	: c(cont), comp(x)
	{
		c.insert(c.end(), first, last);
		std::make_heap(c.begin(), c.end(), comp);
	}

	bool				empty() const	{ return c.empty(); }
	size_type			size() const	{ return c.size(); }
	const value_type& 	top() const		{ return c.front(); }
	inline void	push(const value_type& x)
	{
		c.push_back(x);
		std::push_heap(c.begin(), c.end(), comp);
	}
	inline void	pop()
	{
		std::pop_heap(c.begin(), c.end(), comp);
		c.pop_back();
	}
};

// no equality is provided

}	// namespace ft
