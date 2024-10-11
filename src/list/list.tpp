#include "list.hpp"

using namespace ft;

template <class T, class Allocator>
list<T, Allocator>::list(size_type n, const T& value = T(), const Allocator& alloc = Allocator())
	: _front(nullptr), _back(nullptr), _allocator(alloc), _size(0)
{
	// TESTME: Exception guarantee?
	// push_back holds the string exception guarantee, and the object is correctly initialized.
	// Thus, if constructor throws, the state of the object should be the last valid state, i.e.
	// before the push_back call that threw.
	// XXX: In C++, throwing from a constructor does not call the destructor. BUT the destructor
	// will still be called when the object is destructed (out-of-scope or manually...)

	for (size_t i = 0; i < n; ++i)
		push_back(value);
}
template <class T, class Allocator>
template <class InputIterator>
list<T, Allocator>::list(InputIterator first, InputIterator last, const Allocator& alloc = Allocator())
	: _front(nullptr), _back(nullptr), _allocator(alloc), _size(0)
{
	for (; first != last; ++first)
		push_back(*first);
}

template <class T, class Allocator>
list<T, Allocator>::list(const list<T, Allocator>& x)
	: _front(nullptr), _back(nullptr), _allocator(x._allocator), _size(0)
{
	for (const_iterator i = x.begin(); i != x.end(); ++i)
		push_back(*i);
}

template <class T, class Allocator>
void	list<T, Allocator>::resize(size_type sz, T value = T())
{
	// XXX: Standard doesn't require exception guarantee. I choose to apply Strong Exception Guarantee.
	if (sz > _size)
	{
		insert(end(), sz - _size, value);
	}
	else if (sz < _size)
	{
		iterator start = begin();
		advance(start, sz);
		erase(start, end());
	}
	// Else do nothing
}

template <class T, class Allocator>
list<T, Allocator>& list<T, Allocator>::operator=(const list<T, Allocator>& other)
{
	// XXX: Basic Exception Guarantee: a rollback to the state before function call is not required
	clear();
	this->_allocator = other._allocator;
	for (const_iterator i = other.begin(); i != other.end(); ++i)
		push_back(*i);
}

template <class T, class Allocator>
void	list<T, Allocator>::push_front(const T& value)
{
	_Node *new_node = _allocator.allocate(1);
	try
	{
		_allocator.construct(new_node->data, value);
		new_node->prev = nullptr;
		new_node->next = _front;
		if (_front)	// XXX: Linus Torvalds' talk on "code taste", need to watch it
			_front->prev = new_node;
		_front = new_node;
		++_size;
	}
	catch(const std::exception&)	// Strong exception guarantee
	{
		_allocator.deallocate(new_node, 1);
		throw;
	}
}

template <class T, class Allocator>
void	list<T, Allocator>::pop_front()
{
	_Node *popped = _front;
	// XXX: UB when list is empty, YAAAAAAYYYYYY
	_front = popped->next;
	_front->prev = nullptr;
	--_size;
	_allocator.destroy(popped->data);
	_allocator.deallocate(popped, 1);
}

template <class T, class Allocator>
void	list<T, Allocator>::push_back(const T& value)
{
	_Node *new_node = _allocator.allocate(1);
	try
	{
		_allocator.construct(new_node->data, value);
		new_node->prev = _back;
		new_node->next = nullptr;
		_back->next = new_node;
		_back = new_node;
		++_size;
	}
	catch(const std::exception&)	// Strong exception guarantee
	{
		_allocator.deallocate(new_node, 1);
		throw;
	}
}


template <class T, class Allocator>
void	list<T, Allocator>::pop_back()
{
	_Node *popped = _back;
	// XXX: UB when list is empty, YAAAAAAYYYYYY
	_back = popped->prev;
	_back->next = nullptr;
	--_size;
	_allocator.destroy(popped->data);
	_allocator.deallocate(popped, 1);
}

template <class T, class Allocator>
list<T, Allocator>::iterator	list<T, Allocator>::insert(iterator position, const T& value)
{
	_Node *new_node = _allocator.allocate(1);
	try
	{
		_allocator.construct(new_node->data, value);
		new_node->prev = position._node->prev;
		new_node->next = position._node;
		position._node->prev = new_node;
		++_size;
	}
	catch(const std::exception&)	// Strong exception guarantee
	{
		_allocator.deallocate(new_node, 1);
		throw;
	}
	return iterator(new_node);
}

template <class T, class Allocator>
void	list<T, Allocator>::insert(iterator position, size_type n, const T& value)
{
	list<T, Allocator> sublist(n, value, _allocator);
	this->splice(position, sublist);

	// _Node *sublist = _allocator.allocate(1);
	// sublist->prev = nullptr;
	// sublist->next = nullptr;
	// try
	// {
	// 	// ...
	// }
	// catch(const std::exception&)
	// {
	// 	_Node *head = sublist;
	// 	while (head->next)	// Destroy AND deallocate all sublist except last node...
	// 	{
	// 		_Node *prev = head;
	// 		head = head->next;
	// 		_allocator.destroy(prev->data);
	// 		_allocator.deallocate(prev, 1);
	// 	}
	// 	// ...Last node is just deallocated, not destroyed because constructor threw
	// 	_allocator.deallocate(head, 1);
	// }

}

template <class T, class Allocator>
template <class InputIt>
void	list<T, Allocator>::insert(iterator position, InputIt first, InputIt last)
{
	list<T, Allocator> sublist(first, last, _allocator);
	this->splice(position, sublist);
}

template <class T, class Allocator>
void	list<T, Allocator>::swap(list<T, Allocator> &other)
{
	::ft::swap(_front,		other._front);
	::ft::swap(_back,		other._back);
	::ft::swap(_allocator,	other._allocator);
	::ft::swap(_size,		other._size);
}

template <class T, class Allocator>
void	list<T, Allocator>::clear()
{
	_Node *head = _front;
	while (head)
	{
		_Node *prev = head;
		head = head->next;
		_allocator.destroy(prev->data);
		_allocator.deallocate(prev, 1);
		--_size;
	}
}

template <class T, class Allocator>
void	list<T, Allocator>::splice(iterator position, list<T, Allocator> &other)
{

}
