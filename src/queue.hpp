// I'd love to use #pragma once, but since it's tEcHnIcAlLy not standardized today,
// much less in C++98...
#ifndef FT_STACK
#define FT_STACK

#include <deque>
// TODO: remove std::deque and use our own

namespace ft
{

template <class T, class Container = std::deque<T> >
class queue {
public:
	typedef typename	Container::value_type	value_type;
	typedef typename	Container::size_type	size_type;
	typedef				Container				container_type;

protected:
	Container c;

public:
	explicit inline queue(const Container& src = Container()) : c(src) {}

	inline bool					empty() const	{ return c.empty(); }
	inline size_type			size() const	{ return c.size(); }
	inline value_type&			front()			{ return c.front(); }
	inline const value_type&	front() const	{ return c.front(); }
	inline value_type&			back()			{ return c.back(); }
	inline const value_type&	back() const	{ return c.back(); }
	inline void	push(const value_type& x)		{ c.push_back(x); }
	inline void	pop()							{ c.pop_front(); }
};

template <class T, class Container>
inline bool operator==(const queue<T, Container>& x, const queue<T, Container>& y) {
	return x.c == y.c;
}

template <class T, class Container>
inline bool operator< (const queue<T, Container>& x, const queue<T, Container>& y) {
	return x.c < y.c;
}

template <class T, class Container>
inline bool operator!=(const queue<T, Container>& x, const queue<T, Container>& y) {
	return !(x.c == y.c);
}

template <class T, class Container>
inline bool operator> (const queue<T, Container>& x, const queue<T, Container>& y) {
	return y.c < x.c;
}

template <class T, class Container>
inline bool operator>=(const queue<T, Container>& x, const queue<T, Container>& y) {
	return !(x.c < y.c)
}

template <class T, class Container>
inline bool operator<=(const queue<T, Container>& x, const queue<T, Container>& y) {
	return !(x.c > y.c)
}

}	// namespace ft

#endif
