#include "list.hpp"

using namespace ft;

template <class T, class Allocator>
list<T, Allocator>::list(size_type n, const T& value = T(), const Allocator& alloc = Allocator())
	: _front(nullptr), _end(nullptr), _allocator(alloc), _size(0)
{
	// TESTME: Exception guarantee?
	// push_back holds the string exception guarantee, and the object is correctly initialized.
	// Thus, if constructor throws, the state of the object should be the last valid state, i.e.
	// before the push_back call that threw.
	// XXX: In C++, throwing from a constructor does not call the destructor. BUT the destructor
	// will still be called when the object is destructed (out-of-scope or manually...)
	_end = _allocator.allocate(1);
	_end->prev = nullptr;
	_end->next = nullptr;
	_front = _end;
	for (size_t i = 0; i < n; ++i)
		push_back(value);
}

template <class T, class Allocator>
template <class InputIterator>
list<T, Allocator>::list(InputIterator first, InputIterator last, const Allocator& alloc = Allocator())
	: _front(nullptr), _end(nullptr), _allocator(alloc), _size(0)
{
	for (; first != last; ++first)
		push_back(*first);
}

template <class T, class Allocator>
list<T, Allocator>::list(const list<T, Allocator>& x)
	: _front(nullptr), _end(nullptr), _allocator(x._allocator), _size(0)
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
		new_node->prev = _end->prev;
		new_node->next = _end;
		_end->prev = new_node;
		_front = _size == 0 ? new_node : _front;	// If size == 0
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
	_Node *popped = _end->prev;
	// XXX: UB when list is empty, YAAAAAAYYYYYY
	if (popped->prev)
		popped->prev->next = _end;
	_end = popped->prev;
	_front = _size == 1 ? _end : _front;	// If removing last node
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
		_front = position == begin() ? new_node : _front;	// If inserting at the front
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
	::ft::swap(_end,		other._end);
	::ft::swap(_allocator,	other._allocator);
	::ft::swap(_size,		other._size);
}

template <class T, class Allocator>
void	list<T, Allocator>::clear()
{
	_Node *head = _front;
	while (head != _end)
	{
		_Node *prev = head;
		head = head->next;
		_allocator.destroy(prev->data);
		_allocator.deallocate(prev, 1);
		--_size;
	}
}

// Descriptions of function semantics contain the following elements (as appropriate):148)
// — Requires: the preconditions for calling the function
// — Effects: the actions performed by the function
// — Postconditions: the observable results established by the function
// — Returns: a description of the value(s) returned by the function
// — Throws: any exceptions thrown by the function, and the conditions that would cause the exception
// — Complexity: the time and/or space complexity of the function

template <class T, class Allocator>
void	list<T, Allocator>::splice(iterator position, list<T, Allocator> &other)
{
	if (other.empty())
		return;

	_Node *insert_after = position._node->prev;	// Insert splice after this node...
	_Node *insert_before = position._node;		// ...and before this node
	_Node *other_last = other._end->prev;		// Last node of other list

	if (insert_before)
		insert_after->next = other._front;
	else
		_front = other._front;
	other._front->prev = insert_after;

	insert_before->prev = other_last;
	other_last->next = insert_before;

	other._end->prev = nullptr;
	other._front = other._end;
	other._size = 0;
}

template <class T, class Allocator>
void	list<T, Allocator>::splice(iterator position, list<T, Allocator> &other, iterator target)
{
	if (other.empty())
		return;

	_Node *insert_after = position._node->prev;	// Insert splice after this node...
	_Node *insert_before = position._node;		// ...and before this node

	insert_after->next = target._node;
	if (insert_before)
		insert_before->prev = target._node;
	else
		_front = target._node;	// If inserting at beginning of the list

	if (target != other.begin())
		target._node->prev->next = target._node->next;
	else
		other._front = target._node->next;
	target._node->next->prev = target._node->prev;

	target._node->prev = insert_after;
	target._node->next = insert_before;
	--other._size;
}


template <class T, class Allocator>
void	list<T, Allocator>::splice(iterator position, list<T, Allocator> &other, iterator range_begin,
	iterator range_end)
{
	if (other.empty())
		return;

	_Node *other_before = range_begin._node->prev;	// Node from other list that's before range_begin
	if (range_begin == other.begin())
		other._front = range_end._node;
	else
		other_before->next = range_end._node;
	range_end._node->prev = other_before;

	_Node *insert_after = position._node->prev;	// Insert splice after this node...
	_Node *insert_before = position._node;		// ...and before this node
	_Node *range_last = range_end._node->prev;

	insert_after->next = range_begin._node;
	range_begin._node->prev = insert_after;

	insert_before->prev = range_last;
	range_last->next = insert_before;
}

// TODO: Replace this implementation with a remove_if(std::identity) or something like that?
template <class T, class Allocator>
void	list<T, Allocator>::remove(const T& value)
{
	_Node *head = _front;

	while (*head->next != _end)
	{
		if (head->next->data == value)
		{
			_Node *to_delete = head->next;
			head->next = to_delete->next;
			head->next->prev = head;
			_allocator.destroy(to_delete->data);
			_allocator.deallocate(to_delete, 1);
		}
		head = head->next;
	}
}

template <class T, class Allocator>
template <class Predicate>
void	list<T, Allocator>::remove_if(Predicate pred)
{
	_Node *head = _front;

	while (*head->next != _end)
	{
		if (pred(head->next->data))
		{
			_Node *to_delete = head->next;
			head->next = to_delete->next;
			head->next->prev = head;
			_allocator.destroy(to_delete->data);
			_allocator.deallocate(to_delete, 1);
		}
		head = head->next;
	}
}
