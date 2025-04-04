/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.tpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 17:14:34 by pbremond          #+#    #+#             */
/*   Updated: 2025/03/28 15:09:22 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cassert>

// NOTE: Absolutely MORONIC use of std::memmove was in there. You're not in C anymore.
// Classes exist, and their instances cannot just be wildly flailed around.

// TESTME: Some very small capacity discreptancies when testing on Linux. Not remotely
// important, but curious nonetheless.

#include "vector.hpp"

template < class T, class Allocator >
ft::vector<T, Allocator>::vector(const Allocator& alloc): _allocator(alloc)
{
	_array = _allocator.allocate(0);
	_init_size = 0;
	_size = 0;
	_capacity = 0;
}

template < class T, class Allocator >
ft::vector<T, Allocator>::vector(size_type count,
								 const T& value,
								 const Allocator& alloc)
: _allocator(alloc)
{
	_init_size = count;
	_capacity = count;
	_size = count;
	_array = _allocator.allocate(_init_size);
	for (size_type i = 0; i < count; ++i)
		_allocator.construct(_array + i, value);
}

template < class T, class Allocator >
template<class InputIt>
ft::vector<T, Allocator>::vector(InputIt first,
								 InputIt last,
								 const Allocator& alloc,
								 typename enable_if< !is_fundamental<InputIt>::value, int >::type)
: _allocator(alloc)
{
	_init_size = 0;
	_capacity = _init_size;
	_size = 0;
	_array = _allocator.allocate(_init_size);

	this->assign(first, last);
}

template < class T, class Allocator >
ft::vector<T, Allocator>::vector(const ft::vector<T, Allocator>& src)
: _allocator(src._allocator)
{
	_init_size = src._size;
	_capacity = src._size;
	_array = _allocator.allocate(_init_size);
	_size = 0;
	this->assign(src.begin(), src.end());
}

template < class T, class Allocator >
ft::vector<T, Allocator>	&ft::vector<T, Allocator>::operator=(const ft::vector<T, Allocator>& rhs)
{
	if (&rhs == this)
		return (*this);
	for (size_type i = 0; i < _size; ++i)
		_allocator.destroy(_array + i);
	_size = 0;
	if (this->_capacity < rhs._capacity)
	{
		_allocator.deallocate(_array, _init_size);
		_array = _allocator.allocate(rhs._size);
		_capacity = rhs._size;
		_init_size = rhs._size;
	}
	this->assign(rhs.begin(), rhs.end());
	return (*this);
}

template < class T, class Allocator >
ft::vector<T, Allocator>::~vector()
{
	for (size_type i = 0; i < _size; ++i)
		_allocator.destroy(_array + i);
	_allocator.deallocate(_array, _init_size);
}

template < class T, class Allocator >
void	ft::vector<T, Allocator>::assign(size_type count, const T& value)
{
	if (count > _capacity)
		this->reserve(count);
	for (size_type i = 0; i < _size; ++i) // TESTME: Optimize ?
		_allocator.destroy(_array + i);
	for (size_type i = 0; i < count; ++i) {
		_allocator.construct(_array + i, value);
	}
	_size = count;
}

template < class T, class Allocator >
template < class InputIt >
typename ft::enable_if
<
	!ft::is_fundamental<InputIt>::value,
	void
>::type
ft::vector<T, Allocator>::assign(InputIt first, InputIt last)
{
	_do_assign(first, last, typename ft::iterator_traits<InputIt>::iterator_category());
}

template < class T, class Allocator >
template < class InputIt >
void	ft::vector<T, Allocator>::_do_assign(InputIt first, InputIt last,
	input_iterator_tag)
{
	#if VEC_DEBUG_VERBOSE == true
		std::cerr << _BLU"vector: assign: In input iterator specialization"RESET << std::endl;
	#endif

	this->clear();
	for (; first != last; ++first)
		this->push_back(*first);
}

template < class T, class Allocator >
template < class ForwardIt >
void	ft::vector<T, Allocator>::_do_assign(ForwardIt first, ForwardIt last,
	random_access_iterator_tag)
{
	#if VEC_DEBUG_VERBOSE == true
		std::cerr << _BLU"vector: assign: In forward iterator specialization"RESET << std::endl;
	#endif

	difference_type	newSize = ft::distance(first, last);

	if (static_cast<size_type>(newSize) > _capacity)
		this->reserve(newSize);
	for (size_type i = 0; i < _size; ++i)
		_allocator.destroy(_array + i); // TESTME: Optimize?
	size_type i = 0;
	for (ForwardIt itr = first; itr != last; ++itr, ++i)
		_allocator.construct(_array + i, *itr);
	_size = newSize;
}

/* ************************************************************************** */
/* ************************************************************************** */
/* ************************************************************************** */

template < class T, class Allocator >
typename ft::vector<T, Allocator>::reference	ft::vector<T, Allocator>::at(size_type n)
{
	if (n >= _size)
		throw (ft::out_of_range("ft::out_of_range"));
	return (_array[n]);
}

template < class T, class Allocator >
typename ft::vector<T, Allocator>::const_reference	ft::vector<T, Allocator>::at(size_type n) const
{
	if (n >= _size)
		throw (ft::out_of_range("ft::out_of_range"));
	return (_array[n]);
}

template < class T, class Allocator >
void	ft::vector<T, Allocator>::reserve(size_type newCapacity)
{
	if (newCapacity <= _capacity)
		return ;
	if (newCapacity > _allocator.max_size())
	{ // Pointless preproc directives to mimic current OS error messages.
		#ifdef __linux__
			throw (ft::length_error("vector::reserve"));
		#else
			throw (ft::length_error("allocator<T>::allocate(size_t n) 'n' exceeds maximum supported size"));
		#endif
	}

	T	*newArray = _allocator.allocate(newCapacity);
	for (size_type i = 0; i < _size; ++i) {
		_allocator.construct(newArray + i, _array[i]);
	}
	for (size_type i = 0; i < _size; ++i) {
		_allocator.destroy(_array + i);
	}
	_allocator.deallocate(_array, _init_size);
	_init_size = newCapacity;
	_capacity = newCapacity;
	_array = newArray;
}

/* ************************************************************************** */
/* ************************************************************************** */
/* ************************************************************************** */

template < class T, class Allocator >
void	ft::vector<T, Allocator>::clear()
{
	for (size_type i = 0; i < _size; ++i)
		_allocator.destroy(_array + i);
	_size = 0;
}

template < class T, class Allocator >
typename ft::vector<T, Allocator>::iterator	ft::vector<T, Allocator>::insert(iterator pos,
	const T& value)
{
	if (_size + 1 > _capacity)
	{
		size_type	pos_index = pos - begin();
		this->_doubleCapacity();
		pos = _array + pos_index;
	}
	if (pos == end())
		_allocator.construct(pos.operator->(), value);
	else
	{
		pointer src = pos.operator->();
		pointer dest = src + 1;
		_moveElements(src, dest, end() - pos,
			ft::integral_constant< bool, _hasSwapMethod::value>());
		*pos = value;
	}
	++_size;
	return (pos);
}

template < class T, class Allocator >
void	ft::vector<T, Allocator>::insert(iterator pos, size_type count, const T& value)
{
	if (count == 0)
		return ;
	if (_size + count > _capacity)
	{
		size_type	pos_index = pos - begin();
		_reserveOrDouble(_size + count);
		pos = _array + pos_index;
	}
	pointer src = pos.operator->();
	pointer dest = src + count;
	_moveElements(src, dest, end() - pos,
		ft::integral_constant< bool, _hasSwapMethod::value>());

	for (size_type i = 0; i < count; ++i, ++pos)
	{
		if (pos.operator->() < _array + _size)
			*pos = value;
		else
			_allocator.construct(pos.operator->(), value);
	}
	_size += count;
}

template < class T, class Allocator >
template < class InputIt >
typename ft::enable_if
<
	!ft::is_fundamental<InputIt>::value,
	void
>::type
ft::vector<T, Allocator>::insert(iterator pos, InputIt first, InputIt last)
{
	_do_insert(pos, first, last, typename ft::iterator_traits<InputIt>::iterator_category());
}

template < class T, class Allocator >
template < class InputIt >
void	ft::vector<T, Allocator>::_do_insert(iterator pos, InputIt first, InputIt last,
	input_iterator_tag)
{
	#if VEC_DEBUG_VERBOSE == true
		std::cerr << _BLU"vector: insert: In input iterator specialization"RESET << std::endl;
	#endif

	size_type pos_index = pos - begin();
	for (; first != last; ++first)
	{
		this->insert(iterator(_array + pos_index), *first);
		++pos_index;
	}
}

template < class T, class Allocator >
template < class ForwardIt >
void	ft::vector<T, Allocator>::_do_insert(iterator pos, ForwardIt first, ForwardIt last,
	random_access_iterator_tag)
{
	#if VEC_DEBUG_VERBOSE == true
		std::cerr << _BLU"vector: insert: In random access iterator specialization"RESET << std::endl;
	#endif

	difference_type	count = ft::distance(first, last);
	if (_size + count > _capacity)
	{
		size_type	pos_index = pos - begin();
		_reserveOrDouble(_size + count);
		pos = _array + pos_index;
	}
	pointer src = pos.operator->();
	pointer dest = src + count;
	_moveElements(src, dest, end() - pos,
		ft::integral_constant< bool, _hasSwapMethod::value >());

	for (; first != last; ++first, ++pos)
	{
		if (pos.operator->() < _array + _size)
			*pos = *first;
		else
			_allocator.construct(pos.operator->(), *first);
	}
	_size += count;
}

template < class T, class Allocator >
typename ft::vector<T, Allocator>::iterator	ft::vector<T, Allocator>::erase(iterator pos)
{
	pointer src = (pos + 1).operator->();
	pointer dest = pos.operator->();
	_moveElements(src, dest, end() - src,
		ft::integral_constant< bool, _hasSwapMethod::value >());

	_allocator.destroy(_array + --_size);
	if (pos > end())
		return (end());
	else
		return (pos);
}

template < class T, class Allocator >
typename ft::vector<T, Allocator>::iterator	ft::vector<T, Allocator>::erase(iterator first, iterator last)
{
	difference_type	n_erased = ft::distance(first, last);

	pointer src = last.operator->();
	pointer dest = first.operator->();
	_moveElements(src, dest, end() - src,
		ft::integral_constant< bool, _hasSwapMethod::value >());

	for (difference_type i = end() - last; dest + i < _array + _size; ++i)
		_allocator.destroy(dest + i);
	_size -= n_erased;
	return (first);
}

template < class T, class Allocator >
void	ft::vector<T, Allocator>::push_back(const T& value)
{
	if (_size + 1 > _capacity)
		this->_doubleCapacity();
	_allocator.construct(_array + _size++, value);
}

template < class T, class Allocator >
void	ft::vector<T, Allocator>::pop_back()
{
	_allocator.destroy(_array + --_size);
}

template < class T, class Allocator >
void	ft::vector<T, Allocator>::resize(size_type count, T value)
{
	if (_size < count)
	{
		if (_capacity < count)
			_reserveOrDouble(count);
		for (size_type i = _size; i < count; ++i)
			_allocator.construct(_array + i, value);
		_size = count;
	}
	else if (count < _size)
	{
		this->erase(this->begin() + count, this->end());
	}
}

template < class T, class Allocator >
void	ft::vector<T, Allocator>::swap(vector& other)
{
	#if VEC_DEBUG_VERBOSE == true
		std::cerr << DEBUG_TAG<<__BLU("inside ft::vector swap method") << std::endl;
	#endif
	std::swap(this->_allocator, other._allocator);
	std::swap(this->_init_size, other._init_size);
	std::swap(this->_array, other._array);
	std::swap(this->_capacity, other._capacity);
	std::swap(this->_size, other._size);
}

// namespace ft
// {
// 	// using std::swap;
// 	template< class T, class Alloc >
// 	void	swap(ft::vector<T, Alloc>& lhs, ft::vector<T, Alloc>& rhs)
// 	{
// 		std::cout << "DEBUG: SWAP IS SPECIALIZED (IN FT)" << std::endl;
// 		lhs.swap(rhs);
// 	}
// }

// NOTE: Apparently this is the wrong way to do it, orthodox way is commented out above.
// However, only with that solution can I specialize std::swap for both these cases:
// - `using std::swap; swap(a, b);`
// - `std::swap(a, b);`
namespace std
{
	// using std::swap;
	template< class T, class Alloc >
	void	swap(ft::vector<T, Alloc>& lhs, ft::vector<T, Alloc>& rhs)
	{
		#if VEC_DEBUG_VERBOSE == true
			std::cerr << DEBUG_TAG<<__BLU("specialized std::swap<ft::vector> was called") << std::endl;
		#endif
		lhs.swap(rhs);
	}
}

/* ************************************************************************** */
/* ************************************************************************** */
/* ************************************************************************** */

template < class T, class Allocator >
void	ft::vector<T, Allocator>::_doubleCapacity()
{
	if (_capacity == 0)
		this->reserve(1);
	else
		this->reserve(_capacity * 2);
}

template < class T, class Allocator >
void	ft::vector<T, Allocator>::_moveElements(pointer src, pointer dest, size_type n, ft::false_type)
{
	#if VEC_DEBUG_VERBOSE == true
		std::cerr << __BBLU("DEBUG: ")<<__RED("vector: In unspecialized moveBackward") << std::endl;
	#endif
	if (dest > src)
	{
		for (size_type i = n; i > 0;)
		{
			--i;
			if (dest + i < _array + _size)
				dest[i] = src[i];
			else
				_allocator.construct(dest + i, src[i]);
		}
	}
	else
	{
		for (difference_type i = 0; i < end() - src; ++i)
			dest[i] = src[i];
	}
}

template < class T, class Allocator >
void	ft::vector<T, Allocator>::_moveElements(pointer src, pointer dest, size_type n, ft::true_type)
{
	#if VEC_DEBUG_VERBOSE == true
		std::cerr << DEBUG_TAG<<__UGRN("vector:")<<__GRN(" In swap specialized moveBackward")
			<< std::endl;
	#endif
	if (dest > src)
	{
		for (size_type i = n; i > 0;)
		{
			--i;
			if (dest + i < _array + _size)
				dest[i].swap(src[i]);
			else
				_allocator.construct(dest + i, src[i]);
		}
	}
	else
	{
		for (difference_type i = 0; i < end() - src; ++i)
			dest[i].swap(src[i]);
	}
}
