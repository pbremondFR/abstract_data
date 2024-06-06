#include <deque>
// TODO: remove std::deque and use our own

namespace ft
{

template <class T, class Container = std::deque<T> >
class stack
{
public:
	typedef typename Container::value_type	value_type;
	typedef typename Container::size_type	size_type;
	typedef Container	container_type;

protected:
	Container c;

public:
	explicit inline stack(const Container& src = Container()) : c(src) {}

	inline bool				empty() const		{ return c.empty(); }
	inline size_type			size() const	{ return c.size(); }
	inline value_type&			top()			{ return c.back(); }
	inline const value_type&	top() const		{ return c.back(); }
	inline void	push(const value_type& x)		{ c.push_back(x); }
	inline void	pop()							{ c.pop_back(); }
};

template <class T, class Container>
inline bool operator==(const stack<T, Container>& x, const stack<T, Container>& y) {
	return x.c == y.c;
}

template <class T, class Container>
inline bool operator< (const stack<T, Container>& x, const stack<T, Container>& y) {
	return x.c < y.c;
}

template <class T, class Container>
inline bool operator!=(const stack<T, Container>& x, const stack<T, Container>& y) {
	return !(x.c == y.c);
}

template <class T, class Container>
inline bool operator> (const stack<T, Container>& x, const stack<T, Container>& y) {
	return y.c < x.c;
}

template <class T, class Container>
inline bool operator>=(const stack<T, Container>& x, const stack<T, Container>& y) {
	return !(x.c < y.c)
}

template <class T, class Container>
inline bool operator<=(const stack<T, Container>& x, const stack<T, Container>& y) {
	return !(x.c > y.c)
}

} // namespace ft
