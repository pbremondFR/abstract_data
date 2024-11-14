#pragma once
#include <memory>
#include "iterator.hpp"
#include "algorithm.hpp"

#if __cplusplus < 201103L
# ifndef nullptr
#  define nullptr NULL
# endif
#endif

namespace ft
{

template <class T, class Allocator = std::allocator<T> >
class deque
{
	class _DequeIterator
	{
	private:
		typedef 	iterator_traits<T*>		traits;

	protected:
		deque			*_parent;
		std::size_t		_index;

	public:
		typedef				_DequeIterator		this_type;

		typedef typename	traits::difference_type		difference_type;
		typedef typename	traits::value_type			value_type;
		typedef typename	traits::pointer				pointer;
		typedef typename	traits::reference			reference;
		typedef typename	traits::iterator_category	iterator_category;

		_DequeIterator(deque *parent = nullptr, std::size_t index = 0) : _parent(parent), _index(index) {}
		// template <class Iter>
		// _DequeIterator(_DequeIterator<Iter> const& src) : _parent(src._parent), _index(src._index) {}
		// TESTME: Is copy & assign between deque<T>::iterator and deque<U>::iterator required?
		this_type&	operator=(_DequeIterator const& src) { _parent = src._parent; _index = src._index; }

		reference	operator* () const					{ return _parent->_raw_at(_index); }
		pointer		operator->() const					{ return &_parent->_raw_at(_index); }
		reference	operator[](difference_type n) const { return _parent->_raw_at(_index + n); }

		this_type&	operator++()	{ ++_index; return *this;					   };
		this_type	operator++(int)	{ this_type tmp = *this; ++_index; return tmp; };
		this_type&	operator--()	{ --_index; return *this;					   };
		this_type	operator--(int)	{ this_type tmp = *this; --_index; return tmp; };

		this_type&	operator+=(difference_type n)	{ _index += n; return *this; };
		this_type&	operator-=(difference_type n)	{ _index -= n; return *this; };

		this_type	operator+(difference_type rhs) const	 { return this_type(_index + rhs); };
		this_type	operator-(difference_type rhs) const	 { return this_type(_index - rhs); };
		difference_type		operator-(this_type delta) const { return difference_type(_index - delta._index); };
	};
	friend class _DequeIterator;

public:
	// types:
	typedef typename	Allocator::reference					reference;
	typedef typename	Allocator::const_reference				const_reference;
	// TODO: iterators
	typedef				class _DequeIterator					iterator; 		// See 23.1
	typedef				class _DequeIterator					const_iterator;	// See 23.1
	typedef				std::size_t								size_type;		// See 23.1
	typedef				std::ptrdiff_t							difference_type;// See 23.1
	typedef				T										value_type;
	typedef				Allocator								allocator_type;
	typedef typename	Allocator::pointer						pointer;
	typedef typename	Allocator::const_pointer				const_pointer;
	typedef				std::reverse_iterator<iterator>			reverse_iterator;
	typedef				std::reverse_iterator<const_iterator>	const_reverse_iterator;

private:
	static const size_type	INIT_MAP_LEN = 3;
	// Number of elements per block (Either enough for 4096 bytes or 16 elements, whichever is greater)
	static const size_type	ELEMS_PER_BLOCK = (sizeof(T) * 16 < 4096 ? 4096 / sizeof(T) : 16);
	static const size_type	BLOCK_SIZE = ELEMS_PER_BLOCK * sizeof(T);

	typedef typename Allocator::template rebind<pointer>::other	_Pointer_allocator;

	// Having two allocators is necessary! Containers are required to allocate everything
	// through an allocator interface. Can't use new!
	_Pointer_allocator	_ptr_alloc;
	allocator_type		_value_alloc;

	value_type		**_map;
	size_type		_map_size;
	size_type		_front_idx;
	size_type		_end_idx;

	inline pointer 			_get_block(size_t index)		{ return _map[index / ELEMS_PER_BLOCK]; }
	inline const_pointer	_get_block(size_t index) const	{ return _map[index / ELEMS_PER_BLOCK]; }
	inline void		_set_block(size_t index, pointer val)	{ _map[index / ELEMS_PER_BLOCK] = val; }
	// Get reference to element at given index in map, without taking into account _front_index
	inline reference		_raw_at(size_t idx)			{ return _map[idx / ELEMS_PER_BLOCK][idx % ELEMS_PER_BLOCK]; }
	inline const_reference	_raw_at(size_t idx) const	{ return _map[idx / ELEMS_PER_BLOCK][idx % ELEMS_PER_BLOCK]; }

	inline size_type	_capacity() const	{ return _map_size * ELEMS_PER_BLOCK; }

public:
	// 23.2.1.1 construct/copy/destroy:
	explicit deque(const Allocator& = Allocator());
	explicit deque(size_type n, const T& value = T(), const Allocator& = Allocator());

	// TODO: enable_if
	template <class InputIterator>
	deque(InputIterator first, InputIterator last, const Allocator& = Allocator());
	deque(const deque<T,Allocator>& x);

	~deque();

	deque<T,Allocator>& operator=(const deque<T,Allocator>& x);

	template <class InputIterator>
	void assign(InputIterator first, InputIterator last)	{ clear(); insert(begin(), first, last); }
	void assign(size_type n, const T& t)					{ clear(); insert(begin(), n, t); }

	allocator_type get_allocator() const	{ return _value_alloc; }

	// iterators:
	iterator				begin()			{ return iterator(this, _front_idx); }
	const_iterator			begin() const	{ return iterator(this, _front_idx); }
	iterator				end()			{ return iterator(this, _end_idx); }
	const_iterator			end() const		{ return iterator(this, _end_idx); }
	reverse_iterator		rbegin()		{ return reverse_iterator(end()); }
	const_reverse_iterator	rbegin() const	{ return reverse_iterator(end()); }
	reverse_iterator		rend()			{ return reverse_iterator(begin()); }
	const_reverse_iterator	rend() const	{ return reverse_iterator(begin()); }

	// 23.2.1.2 capacity:
	size_type	size() const		{ return _end_idx - _front_idx; }
	size_type	max_size() const	{ return _value_alloc.max_size(); }
	void		resize(size_type sz, T c = T());
	bool		empty() const		{ return _end_idx == _front_idx; }

	// element access:
	reference		operator[](size_type n);
	const_reference	operator[](size_type n) const;
	reference		at(size_type n);
	const_reference	at(size_type n) const;
	// XXX: Lmao, UB when empty, cool!
	reference		front()			{ return _raw_at(_front_idx); }
	const_reference	front() const	{ return _raw_at(_front_idx); }
	reference		back()			{ return _raw_at(_end_idx - 1); }
	const_reference	back() const	{ return _raw_at(_end_idx - 1); }

	// 23.2.1.3 modifiers:
	void		push_front(const T& x)	{ insert(begin(), x); }
	void		push_back(const T& x)	{ insert(end(), x); }
	iterator	insert(iterator position, const T& x);
	void		insert(iterator position, size_type n, const T& x);
	template <class InputIterator>
	void		insert (iterator position, InputIterator first, InputIterator last);
	void		pop_front();
	void		pop_back();
	iterator	erase(iterator position);
	iterator	erase(iterator first, iterator last);
	void		swap(deque<T,Allocator>&);
	void		clear()		{ erase(begin(), end()); }
};

template <class T, class Allocator>
bool operator==(const deque<T,Allocator>& x, const deque<T,Allocator>& y)
{
	return x.size() == y.size() && ft::equal(x.begin(), x.end(), y.begin());
}

template <class T, class Allocator>
bool operator< (const deque<T,Allocator>& x, const deque<T,Allocator>& y)
{
	return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

template <class T, class Allocator>
bool operator!=(const deque<T,Allocator>& x, const deque<T,Allocator>& y)
{
	return !(x == y);
}

template <class T, class Allocator>
bool operator> (const deque<T,Allocator>& x, const deque<T,Allocator>& y)
{
	return y < x;
}

template <class T, class Allocator>
bool operator>=(const deque<T,Allocator>& x, const deque<T,Allocator>& y)
{
	return !(x < y);
}

template <class T, class Allocator>
bool operator<=(const deque<T,Allocator>& x, const deque<T,Allocator>& y)
{
	return !(x > y);
}
// specialized algorithms:
template <class T, class Allocator>
void swap(deque<T,Allocator>& x, deque<T,Allocator>& y)
{
	x.swap(y);
}

} // namespace ft

#include "deque.tpp"
