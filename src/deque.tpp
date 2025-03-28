#include "deque.hpp"
#include "cassert"
#include <stdexcept>


namespace ft
{

template <class T, class Allocator>
deque<T, Allocator>::deque(Allocator const& alloc)
	: _ptr_alloc(alloc), _value_alloc(alloc), _map(nullptr), _map_size(INIT_MAP_LEN),
		_front_idx(INIT_MAP_LEN / 2), _end_idx(_front_idx)
{
	try
	{
		_map = _ptr_alloc.allocate(_map_size);
		_set_block(_front_idx, _value_alloc.allocate(ELEMS_PER_BLOCK));
	}
	catch(const std::exception&)
	{
		if (_get_block(_front_idx) != nullptr)
			_value_alloc.deallocate(_get_block(_front_idx), ELEMS_PER_BLOCK);
		if (_map != nullptr)
			_ptr_alloc.deallocate(_map, _map_size);
	}
}

template <class T, class Allocator>
deque<T, Allocator>::~deque()
{
	// this->clear();
	// _value_alloc.deallocate(_get_block(_front_idx), ELEMS_PER_BLOCK);
	// _ptr_alloc.deallocate(_map, _map_size);
}

template <class T, class Allocator>
void	deque<T, Allocator>::resize(size_type new_size, T value)
{
	if (new_size > size())
		insert(end(), new_size - size(), value);
	else if (new_size < size())
		erase(begin() + new_size, end());
	// else do nothing
}

template <class T, class Allocator>
typename deque<T, Allocator>::reference
	deque<T, Allocator>::operator[](size_type idx)
{
	idx += _front_idx;
	return _map[idx / BLOCK_SIZE][idx % BLOCK_SIZE];
}

template <class T, class Allocator>
typename deque<T, Allocator>::const_reference
	deque<T, Allocator>::operator[](size_type idx) const
{
	idx += _front_idx;
	return _map[idx / BLOCK_SIZE][idx % BLOCK_SIZE];
}

template <class T, class Allocator>
typename deque<T, Allocator>::reference
	deque<T, Allocator>::at(size_type idx)
{
	// Covers empty case because front == end when empty, in which case idx will be
	if (idx < _front_idx || idx >= _end_idx)
		throw std::out_of_range("deque::at: out_of_range");
	else
		return operator[](idx);
}

template <class T, class Allocator>
typename deque<T, Allocator>::const_reference
	deque<T, Allocator>::at(size_type idx) const
{
	if (idx < _front_idx || idx >= _end_idx)
		throw std::out_of_range("deque::at: out_of_range");
	else
		return operator[](idx);
}

template <class T, class Allocator>
typename deque<T, Allocator>::iterator
	deque<T, Allocator>::insert(iterator position, const T& x)
{
	return begin();
}


template <class T, class Allocator>
void	deque<T, Allocator>::pop_front()
{
	_value_alloc.destroy(&_raw_at(_front_idx));
	++_front_idx;
	// TODO: shrink map alloc if necessary?
}

template <class T, class Allocator>
void	deque<T, Allocator>::pop_back()
{
	_value_alloc.destroy(&_raw_at(_end_idx - 1));
	--_end_idx;
	// TODO: shrink map alloc if necessary?
}

template <class T, class Allocator>
void	deque<T, Allocator>::swap(deque<T, Allocator> &other)
{
	ft::swap(_ptr_alloc,	other._ptr_alloc);
	ft::swap(_value_alloc,	other._value_alloc);
	ft::swap(_map,			other._map);
	ft::swap(_map_size,		other._map_size);
	ft::swap(_front_idx,	other._front_idx);
	ft::swap(_end_idx,		other._end_idx);
}

}	// namespace ft
