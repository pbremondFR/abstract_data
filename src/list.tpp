#include "list.hpp"
#include "functional.hpp"
#include <cassert>

namespace ft
{

#ifdef _DEBUG
/*
 * Checks that all of the list's links are valid. Aborts with an assertion otherwise.
 */
template <class T, class Allocator>
void	list<T, Allocator>::_check_list_integrity() const
{
	assert(_front->prev == nullptr);
	assert(_end->next == nullptr);
	size_t size = 0;
	for (_Node const *head = _front; head; head = head->next)
	{
		if (head->next)
			assert(head->next->prev == head);
		if (head->prev)
			assert(head->prev->next == head);
		++size;
	}
	assert(--size == _size);
	if (size > 0)
		assert(_front != _end);
	else
		assert(_front == _end);
}

/*
 * Checks if the list is correctly sorted. Aborts with an assertion if it's not the case.
 */
template <class T, class Allocator>
void	list<T, Allocator>::_check_list_integrity() const
{
	for (_Node *head = _front; head->next != _end; head = head->next)
		assert(head->data <= head->next->data);
}
#else
/* In release mode, these are no-ops and should be optimized away. */
template <class T, class Allocator>
void	list<T, Allocator>::_check_list_integrity() const
{}

template <class T, class Allocator>
void	list<T, Allocator>::_check_list_sorted() const
{}
#endif

template <class T, class Allocator>
list<T, Allocator>::list(const Allocator& alloc)
		: _front(nullptr), _end(nullptr), _allocator(alloc), _size(0)
{
	_end = _allocator.allocate(1);	// XXX: Don't construct, it's just a dummy. Can I do that?
	_end->prev = nullptr;
	_end->next = nullptr;
	_front = _end;
}

template <class T, class Allocator>
list<T, Allocator>::list(size_type n, const T& value, const Allocator& alloc)
	: _front(nullptr), _end(nullptr), _allocator(alloc), _size(0)
{
	// TESTME: Exception guarantee?
	// push_back holds the string exception guarantee, and the object is correctly initialized.
	// Thus, if constructor throws, the state of the object should be the last valid state, i.e.
	// before the push_back call that threw.
	// XXX: In C++, throwing from a constructor does not call the destructor. BUT the destructor
	// will still be called when the object is destructed (out-of-scope or manually...)
	_end = _allocator.allocate(1);	// XXX: Don't construct, it's just a dummy. Can I do that?
	_end->prev = nullptr;
	_end->next = nullptr;
	_front = _end;
	for (size_t i = 0; i < n; ++i)
		push_back(value);
}

template <class T, class Allocator>
template <class InputIterator>
list<T, Allocator>::list(InputIterator first, InputIterator last, const Allocator& alloc)
	: _front(nullptr), _end(nullptr), _allocator(alloc), _size(0)
{
	_end = _allocator.allocate(1);	// XXX: Don't construct, it's just a dummy. Can I do that?
	_end->prev = nullptr;
	_end->next = nullptr;
	_front = _end;
	for (; first != last; ++first)
		push_back(*first);
}

template <class T, class Allocator>
list<T, Allocator>::list(const list<T, Allocator>& x)
	: _front(nullptr), _end(nullptr), _allocator(x._allocator), _size(0)
{
	_end = _allocator.allocate(1);	// XXX: Don't construct, it's just a dummy. Can I do that?
	_end->prev = nullptr;
	_end->next = nullptr;
	_front = _end;
	for (const_iterator i = x.begin(); i != x.end(); ++i)
		push_back(*i);
}

template <class T, class Allocator>
void	list<T, Allocator>::resize(size_type sz, T value)
{
	// XXX: Standard doesn't require exception guarantee. I choose to apply Strong Exception Guarantee.
	if (sz > _size)
	{
		insert(end(), sz - _size, value);
	}
	else if (sz < _size)
	{
		iterator start = begin();
		ft::advance(start, sz);
		erase(start, end());
	}
	// Else do nothing
}

template <class T, class Allocator>
list<T, Allocator>& list<T, Allocator>::operator=(const list<T, Allocator>& other)
{
	// XXX: Basic Exception Guarantee: a rollback to the state before function call is not required
	clear();
	// TESTME: Is allocator change OK here? Should I free _end before changing the allocator and reallocate it afterwards?
	this->_allocator = other._allocator;
	for (const_iterator i = other.begin(); i != other.end(); ++i)
		push_back(*i);
	return *this;
}

template <class T, class Allocator>
void	list<T, Allocator>::push_front(const T& value)
{
	_check_list_integrity();
	_Node *new_node = _allocator.allocate(1);
	try
	{
		_allocator.construct(new_node, _Node(value));
		new_node->prev = nullptr;
		new_node->next = _front;
		_front->prev = new_node;
		_front = new_node;
		++_size;
		_check_list_integrity();
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
	_check_list_integrity();
	_Node *popped = _front;
	// XXX: UB when list is empty, YAAAAAAYYYYYY
	_front = popped->next;
	_front->prev = nullptr;
	--_size;
	_allocator.destroy(popped);
	_allocator.deallocate(popped, 1);
	_check_list_integrity();
}

template <class T, class Allocator>
void	list<T, Allocator>::push_back(const T& value)
{
	_check_list_integrity();
	_Node *new_node = _allocator.allocate(1);
	try
	{
		_allocator.construct(new_node, _Node(value));
		new_node->prev = _end->prev;
		new_node->next = _end;
		if (_end->prev)
			_end->prev->next = new_node;
		_end->prev = new_node;
		_front = _size == 0 ? new_node : _front;	// If size == 0
		++_size;
		_check_list_integrity();
	}
	catch(const std::exception&)	// Strong exception guarantee
	{	// Don't destroy object, because exception was thrown in its constructor
		_allocator.deallocate(new_node, 1);
		throw;
	}
}


template <class T, class Allocator>
void	list<T, Allocator>::pop_back()
{
	_check_list_integrity();
	_Node *popped = _end->prev;
	// XXX: UB when list is empty, YAAAAAAYYYYYY
	if (popped->prev)
		popped->prev->next = _end;
	_end->prev = popped->prev;
	_front = _size == 1 ? _end : _front;	// If removing last node
	--_size;
	_allocator.destroy(popped);
	_allocator.deallocate(popped, 1);
	_check_list_integrity();
}

template <class T, class Allocator>
typename list<T, Allocator>::iterator
	list<T, Allocator>::insert(iterator position, const T& value)
{
	_check_list_integrity();
	_Node *new_node = _allocator.allocate(1);
	try
	{
		_allocator.construct(new_node, _Node(value));
		new_node->prev = position._node->prev;
		new_node->next = position._node;
		position._node->prev = new_node;
		if (new_node->prev)
			new_node->prev->next = new_node;
		else
			_front = new_node;
		++_size;
	}
	catch(const std::exception&)	// Strong exception guarantee
	{
		_allocator.deallocate(new_node, 1);
		throw;
	}
	_check_list_integrity();
	return iterator(new_node);
}

template <class T, class Allocator>
void	list<T, Allocator>::insert(iterator position, size_type n, const T& value)
{
	_check_list_integrity();
	list<T, Allocator> sublist(n, value, _allocator);
	sublist._check_list_integrity();
	this->splice(position, sublist);
	_check_list_integrity();
}

template <class T, class Allocator>
template <class InputIt>
void	list<T, Allocator>::insert(iterator position, InputIt first, InputIt last)
{
	_check_list_integrity();
	if (first == last)
		return;
	list<T, Allocator> sublist(first, last, _allocator);
	sublist._check_list_integrity();
	this->splice(position, sublist);
	_check_list_integrity();
}

template <class T, class Allocator>
typename list<T, Allocator>::iterator
	list<T, Allocator>::erase(iterator position)
{
	_check_list_integrity();
	_Node *to_erase = position._node;

	if (to_erase == _front)
		_front = to_erase->next;
	else
		to_erase->prev->next = to_erase->next;
	to_erase->next->prev = to_erase->prev;

	iterator retval(to_erase->next);
	_allocator.destroy(to_erase);
	_allocator.deallocate(to_erase, 1);
	--this->_size;
	_check_list_integrity();
	return retval;
}

template <class T, class Allocator>
typename list<T, Allocator>::iterator
	list<T, Allocator>::erase(iterator range_begin, iterator range_end)
{
	_check_list_integrity();
	if (range_begin == range_end)
		return range_end;

	_Node *first_node = range_begin._node;
	_Node *last_node = range_end._node->prev;

	if (first_node == _front)
		_front = last_node->next;
	else
		first_node->prev->next = last_node->next;
	last_node->next->prev = first_node->prev;

	// iterator retval(last_node->next);

	first_node->prev = nullptr;
	last_node->next = nullptr;
	while (first_node != nullptr)
	{
		_Node *to_delete = first_node;
		first_node = first_node->next;
		_allocator.destroy(to_delete);
		_allocator.deallocate(to_delete, 1);
		--this->_size;
	}
	_check_list_integrity();
	return range_end;
}

template <class T, class Allocator>
void	list<T, Allocator>::swap(list<T, Allocator> &other)
{
	_check_list_integrity();
	ft::swap(_front,		other._front);
	ft::swap(_end,			other._end);
	ft::swap(_allocator,	other._allocator);
	ft::swap(_size,			other._size);
	_check_list_integrity();
}

template <class T, class Allocator>
void	list<T, Allocator>::clear()
{
	_check_list_integrity();
	_Node *head = _front;
	while (head != _end)
	{
		_Node *prev = head;
		head = head->next;
		_allocator.destroy(prev);
		_allocator.deallocate(prev, 1);
	}
	_end->prev = nullptr;
	_front = _end;
	_size = 0;
	_check_list_integrity();
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
	_check_list_integrity();
	if (other.empty())
		return;

	_Node *insert_after = position._node->prev;	// Insert splice after this node...
	_Node *insert_before = position._node;		// ...and before this node
	_Node *other_last = other._end->prev;		// Last node of other list

	if (position != begin())	// insert_before/after are not null if size > 0
		insert_after->next = other._front;
	else
		_front = other._front;
	other._front->prev = insert_after;

	insert_before->prev = other_last;
	other_last->next = insert_before;

	other._end->prev = nullptr;
	other._front = other._end;
	this->_size += other._size;
	other._size = 0;
	_check_list_integrity();
}

template <class T, class Allocator>
void	list<T, Allocator>::splice(iterator position, list<T, Allocator> &other, iterator target)
{
	_check_list_integrity();
	if (other.empty())
		return;

	_Node *insert_after = position._node->prev;	// Insert splice after this node...
	_Node *insert_before = position._node;		// ...and before this node

	if (position != begin())
		insert_after->next = target._node;
	else
		_front = target._node;	// If inserting at beginning of the list
	insert_before->prev = target._node;

	if (target != other.begin())
		target._node->prev->next = target._node->next;
	else
		other._front = target._node->next;
	target._node->next->prev = target._node->prev;

	target._node->prev = insert_after;
	target._node->next = insert_before;
	++this->_size;
	--other._size;
	_check_list_integrity();
}


template <class T, class Allocator>
void	list<T, Allocator>::splice(iterator position, list<T, Allocator> &other, iterator range_begin,
	iterator range_end)
{
	_check_list_integrity();
	if (other.empty() || range_begin == range_end)
		return;

	_Node *insert_after = position._node->prev;	// Insert splice after this node...
	_Node *insert_before = position._node;		// ...and before this node
	_Node *range_last = range_end._node->prev;

	_Node *other_before = range_begin._node->prev;	// Node from other list that's before range_begin
	if (range_begin == other.begin())	// If we're slicing from beginning of other list
		other._front = range_end._node;
	else
		other_before->next = range_end._node;
	range_end._node->prev = other_before;

	if (position != this->begin())
		insert_after->next = range_begin._node;
	else
		_front = range_begin._node;
	range_begin._node->prev = insert_after;

	insert_before->prev = range_last;
	range_last->next = insert_before;

	if (&other != this)	// Complexity: Constant time if &x == this; otherwise, linear time
	{
		size_t num_spliced = ft::distance(range_begin, iterator(range_last->next));
		other._size -= num_spliced;
		this->_size += num_spliced;
	}
	_check_list_integrity();
}

// TODO: Replace this implementation with a remove_if(std::identity) or something like that?
template <class T, class Allocator>
void	list<T, Allocator>::remove(const T& value)
{
	_check_list_integrity();
	for (iterator it = begin(); it != end();)
	{
		if (*it == value)
		{
			iterator to_delete = it;
			++it;
			erase(to_delete);
		}
		else
			++it;
	}
	_check_list_integrity();
}

template <class T, class Allocator>
template <class Predicate>
void	list<T, Allocator>::remove_if(Predicate pred)
{
	_check_list_integrity();
	for (iterator it = begin(); it != end();)
	{
		if (pred(*it))
		{
			iterator to_delete = it;
			++it;
			erase(to_delete);
		}
		else
			++it;
	}
	_check_list_integrity();
}

template <class T, class Allocator>
void	list<T, Allocator>::unique()
{
	unique(ft::equal_to<T>());
}

template <class T, class Allocator>
template <class BinaryPredicate>
void	list<T, Allocator>::unique(BinaryPredicate is_equal)
{
	_check_list_integrity();
	if (_size < 2)
		return;

	_Node *head = _front;
	while (head != _end)
	{
		_Node *range_end = head->next;
		while (range_end != _end && is_equal(range_end->data, head->data))
			range_end = range_end->next;
		if (head->next != range_end)
			erase(iterator(head->next), iterator(range_end));
		head = range_end;
	}
	_check_list_integrity();
}

template <class T, class Allocator>
void	list<T, Allocator>::merge(list<T, Allocator>& other)
{
	merge(other, less<T>());
}

template <class T, class Allocator>
template <class Compare>
void	list<T, Allocator>::merge(list<T, Allocator>& other, Compare comp)
{
	_check_list_integrity();
	if (this->empty())
	{
		this->splice(this->begin(), other);
		_check_list_integrity();
		return;
	}
	// Standard allows us to assume both lists are sorted
	_Node *head = _front;
	while (other.size() > 0)
	{
		// Find place to insert front of other list (first element greater than other.front)
		while (head != _end && comp(head->data, other.front()))
			head = head->next;
		// Find out the range of the other list to insert (all elements smaller than head)
		_Node *range_end = other._front->next;
		if (head != _end)
		{
			while (range_end != other._end && comp(range_end->data, head->data))
				range_end = range_end->next;
		}
		else	// If head is end, don't dereference it! Just insert everything else
			range_end = other._end;
		// Perform a splice-insert of that section
		this->splice(iterator(head), other, other.begin(), iterator(range_end));
	}
	_check_list_integrity();
	_check_list_sorted();
}

template <class T, class Allocator>
void	list<T, Allocator>::sort()
{
	sort(less<T>());
}

template <class T, class Allocator>
template <class Compare>
void	list<T, Allocator>::sort(Compare comp)
{
	_check_list_integrity();
	// Only Basic Exception Guarantee
	// XXX: Standard specifies "approximately O(N log(N))"
	if (_size <= 2)
	{
		if (_size == 2 && comp(back(), front()))
			reverse();
		return;
	}

	iterator middle = begin();
	ft::advance(middle, _size / 2);
	list<T, Allocator> a, b;
	a.splice(a.begin(), *this, this->begin(), middle);
	b.splice(b.begin(), *this);
	a.sort(comp);
	b.sort(comp);
	a.merge(b, comp);
	this->splice(this->begin(), a);
	_check_list_integrity();
	_check_list_sorted();
}

template <class T, class Allocator>
void	list<T, Allocator>::reverse()
{
	_check_list_integrity();
	if (_size <= 1)
		return;

	_Node *prev = nullptr;
	_Node *future_last = _front;
	_Node *head = _front;
	while (head != _end)
	{
		prev = head->prev;
		head->prev = head->next;
		head->next = prev;
		head = head->prev;
	}
	_front = head->prev;
	_front->prev = nullptr;

	_end->prev = future_last;
	future_last->next = _end;
	_check_list_integrity();
}

} // namespace ft
