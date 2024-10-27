#pragma once

#include "iterator.hpp"
#include <memory>
#include <cstddef>
#include "algorithm.hpp"

#include <cassert>

#if __cplusplus < 201103L
# ifndef nullptr
#  define nullptr NULL
# endif
#endif

namespace ft
{

template <class T, class Allocator = std::allocator<T> >
class list
{
private:
	struct _Node
	{
		_Node	*prev;
		_Node	*next;
		T		data;

		// Goodbye, POD...
		_Node(const T& data_, _Node *prev_ = nullptr, _Node *next_ = nullptr)
			: prev(prev_), next(next_), data(data_) {}
	};

	// Allocator rebind to allocate nodes correctly
	typedef typename Allocator::template rebind<_Node>::other	_Alloc;

	// TODO: Dummy node? Right now front and back are null when list is empty, that fucks up iterators
	_Node	*_front;
	_Node	*_end;		// End "dummy" node, corresponds to end iterator
	_Alloc	_allocator;
	size_t	_size;

	template <class IteratorValueType>
	class _Iterator
	{
	private:
		friend class list;	// Required for iterator operations
		_Node	*_node;

	public:
		typedef std::bidirectional_iterator_tag	iterator_category;
		typedef std::ptrdiff_t					difference_type;
		typedef IteratorValueType				value_type;
		typedef IteratorValueType*				pointer;
		typedef IteratorValueType&				reference;

		// Forward Iterator requirements
		_Iterator() : _node(nullptr) {}
		_Iterator(_Node *node) : _node(node) {}
		_Iterator(_Iterator const& x) : _node(x._node) {}
		~_Iterator() {}
		_Iterator& operator=(_Iterator const& x)	{ _node = x._node; return *this; }

		// Allows iterator -> const_iterator convertion
		inline operator _Iterator<const IteratorValueType>() const { return (this->_node); }

		bool	operator==(_Iterator const& x) const	{ return _node == x._node; }
		bool	operator!=(_Iterator const& x) const	{ return _node != x._node; }

		reference	operator*()		{ return _node->data; }
		pointer		operator->()	{ return &(_node->data); }

		_Iterator& operator++()		{ _node = _node->next; return *this; }
		_Iterator operator++(int)	{ _Iterator tmp = *this; _node = _node->next; return tmp; }

		// Bidirectional Iterator requirements
		// XXX: There are 3 DRs on these BidirectionalIterator requirements.
		// Check that you're ISO compliant and ignore the defects... Yes, really...
		_Iterator& operator--()		{ _node = _node->prev; return *this; }
		_Iterator operator--(int)	{ _Iterator tmp = *this; _node = _node->prev; return tmp; }
	};

	// I fucking miss lambdas, man...
	template <class BinaryPredicate>
	struct not_equal {
		T const& 		value;
		BinaryPredicate	&predicate;
		not_equal(T const& compare_to, BinaryPredicate predicate_)
			: value(compare_to), predicate(predicate_) {}
		inline bool operator()(T const& x) const { return !predicate(x, value); }
	};

	void	_check_list_integrity() const;
	void	_check_list_sorted() const;

public:
	// types:
	typedef typename	Allocator::reference					reference;
	typedef typename	Allocator::const_reference				const_reference;
	typedef				_Iterator<T>							iterator;		// See 23.1
	typedef				_Iterator<T>							const_iterator;	// See 23.1
	typedef				std::size_t								size_type;		// See 23.1
	typedef				std::ptrdiff_t							difference_type;// See 23.1
	typedef				T										value_type;
	typedef				Allocator								allocator_type;
	typedef typename	Allocator::pointer						pointer;
	typedef typename	Allocator::const_pointer				const_pointer;
	typedef				ft::reverse_iterator<iterator>			reverse_iterator;
	typedef				ft::reverse_iterator<const_iterator>	const_reverse_iterator;

	// 23.2.2.1 construct/copy/destroy:
	explicit list(const Allocator& alloc = Allocator());
	explicit list(size_type n, const T& value = T(), const Allocator& alloc = Allocator());
	template <class InputIterator>
	list(InputIterator first, InputIterator last, const Allocator& alloc = Allocator());
	list(const list<T, Allocator>& x);
	~list()
	{
		clear();
		_allocator.deallocate(_end, 1);
	}
	list<T, Allocator>& operator=(const list<T, Allocator>& other);

	// XXX: Strong Exception Guarantee mandated by standard's effects
	template <class InputIterator>	// TODO: SFINAE?
	void	assign(InputIterator first, InputIterator last)	{ clear(); insert(begin(), first, last); }
	void	assign(size_type n, const T& value)				{ clear(); insert(begin(), n, value); }
	allocator_type	get_allocator() const					{ return _allocator; }

	// iterators:
	iterator				begin()			{ return iterator(_front); }
	const_iterator			begin() const	{ return iterator(_front); }
	iterator				end()			{ return iterator(_end); }
	const_iterator			end() const		{ return iterator(_end); }
	reverse_iterator		rbegin()		{ return reverse_iterator(_end); }
	const_reverse_iterator	rbegin() const	{ return reverse_iterator(_end); }
	reverse_iterator		rend()			{ return reverse_iterator(_front); }
	const_reverse_iterator	rend() const	{ return reverse_iterator(_front); }

	// 23.2.2.2 capacity:
	bool		empty() const		{ return _size == 0; }
	size_type	size() const		{ return _size; }
	size_type	max_size() const	{ return _allocator.max_size(); }
	void		resize(size_type sz, T c = T());

	// element access:
	// XXX: LMAO, UB if list is empty. Isn't that so funny?!?!?!?!
	reference		front()			{ assert(_size); return _front->data; }
	const_reference	front() const	{ assert(_size); return _front->data; }
	reference		back()			{ assert(_size); return _end->prev->data; }
	const_reference	back() const	{ assert(_size); return _end->prev->data; }

	// 23.2.2.3 modifiers:
	void		push_front(const T& x);
	void		pop_front();
	void		push_back(const T& x);
	void		pop_back();
	iterator	insert(iterator position, const T& x);
	void		insert(iterator position, size_type n, const T& x);
	template <class InputIt>
	void		insert(iterator position, InputIt first, InputIt last);
	iterator	erase(iterator position);
	iterator	erase(iterator position, iterator last);
	void		swap(list<T,Allocator> &other);
	void		clear();

	// 23.2.2.4 list operations:
	void	splice(iterator position, list<T,Allocator>& x);
	void	splice(iterator position, list<T,Allocator>& x, iterator i);
	void	splice(iterator position, list<T,Allocator>& x, iterator first, iterator last);
	void	remove(const T& value);
	template <class Predicate>
	void	remove_if(Predicate pred);
	void	unique();
	template <class BinaryPredicate>
	void	unique(BinaryPredicate binary_pred);
	void	merge(list<T,Allocator>& x);
	template <class Compare>
	void	merge(list<T,Allocator>& x, Compare comp);
	void	sort();
	template <class Compare>
	void	sort(Compare comp);
	void	reverse();
};

template <class T, class Allocator>
bool operator==(const list<T,Allocator>& x, const list<T,Allocator>& y)
{
	return x.size() == y.size() && ft::equal(x.begin(), x.end(), y.begin());
}

template <class T, class Allocator>
bool operator< (const list<T,Allocator>& x, const list<T,Allocator>& y)
{
	return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

template <class T, class Allocator>
bool operator!=(const list<T,Allocator>& x, const list<T,Allocator>& y)
{
	return !(x == y);
}

template <class T, class Allocator>
bool operator> (const list<T,Allocator>& x, const list<T,Allocator>& y)
{
	return y < x;
}

template <class T, class Allocator>
bool operator>=(const list<T,Allocator>& x, const list<T,Allocator>& y)
{
	return !(x < y);
}

template <class T, class Allocator>
bool operator<=(const list<T,Allocator>& x, const list<T,Allocator>& y)
{
	return !(x > y);
}

// specialized algorithms:
template <class T, class Allocator>
void swap(list<T,Allocator>& x, list<T,Allocator>& y)
{
	x.swap(y);
}

}	// namespace ft

#include "list.tpp"
