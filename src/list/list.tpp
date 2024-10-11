#include "list.hpp"

using namespace ft;

template <class T, class Allocator>
void	list<T, Allocator>::push_front(const T& value)
{
	_Node *new_node = _allocator.allocate(1);
	_allocator.construct(new_node->data, value);
	new_node->prev = nullptr;
	new_node->next = _front;
	if (_front)	// XXX: Linus Torvalds' talk on "code taste", need to watch it
		_front->prev = new_node;
	_front = new_node;
}

template <class T, class Allocator>
void	list<T, Allocator>::pop_front()
{
	_Node *popped = _front;
	// XXX: UB when list is empty, YAAAAAAYYYYYY
	_front = popped->next;
	_front->prev = nullptr;
	_allocator.destroy(popped->data);
	_allocator.deallocate(popped, 1);
}

template <class T, class Allocator>
void	list<T, Allocator>::push_back(const T& value)
{
	_Node *new_node = _allocator.allocate(1);
	_allocator.construct(new_node->data, value);
	new_node->prev = _back;
	new_node->next = nullptr;
	_back->next = new_node;
	_back = new_node;
}


template <class T, class Allocator>
void	list<T, Allocator>::pop_back()
{
	_Node *popped = _back;
	// XXX: UB when list is empty, YAAAAAAYYYYYY
	_back = popped->prev;
	_back->next = nullptr;
	_allocator.destroy(popped->data);
	_allocator.deallocate(popped, 1);
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
	}
}
