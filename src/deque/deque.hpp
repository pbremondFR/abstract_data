#include <memory>

namespace ft
{

template <class T, class Allocator = std::allocator<T> >
class deque {
public:
	// types:
	typedef typename	Allocator::reference					reference;
	typedef typename	Allocator::const_reference				const_reference;
	// TODO: iterators
	typedef				void									iterator; 		// See 23.1
	typedef				void									const_iterator;	// See 23.1
	typedef				void									size_type;		// See 23.1
	typedef				void									difference_type;// See 23.1
	typedef				T										value_type;
	typedef				Allocator								allocator_type;
	typedef typename	Allocator::pointer						pointer;
	typedef typename	Allocator::const_pointer				const_pointer;
	typedef				std::reverse_iterator<iterator>			reverse_iterator;
	typedef				std::reverse_iterator<const_iterator>	const_reverse_iterator;

	// 23.2.1.1 construct/copy/destroy:
	explicit deque(const Allocator& = Allocator());
	explicit deque(size_type n, const T& value = T(), const Allocator& = Allocator());

	template <class InputIterator>
	deque(InputIterator first, InputIterator last, const Allocator& = Allocator());
	deque(const deque<T,Allocator>& x);

	~deque();

	deque<T,Allocator>& operator=(const deque<T,Allocator>& x);

	template <class InputIterator>
	void assign(InputIterator first, InputIterator last);
	void assign(size_type n, const T& t);

	allocator_type get_allocator() const;

	// iterators:
	iterator				begin();
	const_iterator			begin() const;
	iterator				end();
	const_iterator			end() const;
	reverse_iterator		rbegin();
	const_reverse_iterator	rbegin() const;
	reverse_iterator		rend();
	const_reverse_iterator	rend() const;

	// 23.2.1.2 capacity:
	size_type	size() const;
	size_type	max_size() const;
	void		resize(size_type sz, T c = T());
	bool		empty() const;

	// element access:
	reference		operator[](size_type n);
	const_reference	operator[](size_type n) const;
	reference		at(size_type n);
	const_reference	at(size_type n) const;
	reference		front();
	const_reference	front() const;
	reference		back();
	const_reference	back() const;

	// 23.2.1.3 modifiers:
	void		push_front(const T& x);
	void		push_back(const T& x);
	iterator	insert(iterator position, const T& x);
	void		insert(iterator position, size_type n, const T& x);
	template <class InputIterator>
	void		insert (iterator position, InputIterator first, InputIterator last);
	void		pop_front();
	void		pop_back();
	iterator	erase(iterator position);
	iterator	erase(iterator first, iterator last);
	void		swap(deque<T,Allocator>&);
	void		clear();
};

template <class T, class Allocator>
bool operator==(const deque<T,Allocator>& x, const deque<T,Allocator>& y);

template <class T, class Allocator>
bool operator< (const deque<T,Allocator>& x, const deque<T,Allocator>& y);

template <class T, class Allocator>
bool operator!=(const deque<T,Allocator>& x, const deque<T,Allocator>& y);

template <class T, class Allocator>
bool operator> (const deque<T,Allocator>& x, const deque<T,Allocator>& y);

template <class T, class Allocator>
bool operator>=(const deque<T,Allocator>& x, const deque<T,Allocator>& y);

template <class T, class Allocator>
bool operator<=(const deque<T,Allocator>& x, const deque<T,Allocator>& y);

// specialized algorithms:
template <class T, class Allocator>
void swap(deque<T,Allocator>& x, deque<T,Allocator>& y);

} // namespace
