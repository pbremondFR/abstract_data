/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stl_set.tpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 16:58:33 by pbremond          #+#    #+#             */
/*   Updated: 2025/03/29 18:15:18 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "stl_set.hpp"

#ifndef STL_SET_HPP
# error "You should never include this file! Instead, include set.hpp"
#endif

#if SET_DEBUG_VERBOSE == true
template <class Key, class Compare, class Allocator>
std::ostream&	ft::set<Key, Compare, Allocator>::logstream = std::cerr;
#endif

// ============================================================================================== //
// ------------------------------------------ ITERATOR ------------------------------------------ //
// ============================================================================================== //

template <class Key, class Compare, class Allocator>
template <class U>
typename ft::set<Key, Compare, Allocator>::template __map_iterator<U>&
	ft::set<Key, Compare, Allocator>::__map_iterator<U>::operator++()
{
	if (_node->right != NULL) // NOTE: Keep it like this. It allows to get to end leaf
	{
		_node = _node->right;
		while (_node->left != NULL)
			_node = _node->left;
	}
	else
	{ // No right child, go back up until you go over a left child
		_Node_t	*parent = _node->parent;
		while (parent != NULL && _node == parent->right) {
			_node = _node->parent;
			parent = parent->parent;
		}
		_node = parent;
	}
	return (*this);
}

template <class Key, class Compare, class Allocator>
template <class U>
typename ft::set<Key, Compare, Allocator>::template __map_iterator<U>
	ft::set<Key, Compare, Allocator>::__map_iterator<U>::operator++(int)
{
	__map_iterator<U>	tmp(*this);
	this->operator++();
	return (tmp);
}

template <class Key, class Compare, class Allocator>
template <class U>
typename ft::set<Key, Compare, Allocator>::template __map_iterator<U>&
	ft::set<Key, Compare, Allocator>::__map_iterator<U>::operator--()
{
	if (_node->left != NULL)
	{
		_node = _node->left;
		while (_node->right != NULL)
			_node = _node->right;
	}
	else
	{
		_Node_t	*parent = _node->parent;
		while (parent != NULL && _node == parent->left) {
			_node = _node->parent;
			parent = parent->parent;
		}
		_node = parent;
	}
	return (*this);
}

template <class Key, class Compare, class Allocator>
template <class U>
typename ft::set<Key, Compare, Allocator>::template __map_iterator<U>
	ft::set<Key, Compare, Allocator>::__map_iterator<U>::operator--(int)
{
	__map_iterator<U>	tmp(*this);
	this->operator--();
	return (tmp);
}

// ============================================================================================== //
// --------------------------------------- PUBLIC METHODS --------------------------------------- //
// ============================================================================================== //

template <class Key, class Compare, class Allocator>
ft::set<Key, Compare, Allocator>::set(Compare const& comp, Allocator const& alloc)
	: _compare(comp), _allocator(alloc), _endLeaf(_allocator.allocate(1)), _root(_endLeaf),
		_dummy(_allocator.allocate(1)), _size(0)
{
	_endLeaf->colour = _Node_t::BLACK;
	_endLeaf->parent = NULL;
	_endLeaf->left = NULL;
	_endLeaf->right = NULL;

	_dummy->colour = _Node_t::BLACK;
	_dummy->parent = NULL;
	_dummy->left = NULL;
	_dummy->right = NULL;
}

template <class Key, class Compare, class Allocator>
template <class InputIt>
ft::set<Key, Compare, Allocator>::set(InputIt first, InputIt last,
										 Compare const& comp,
										 Allocator const& alloc,
										 typename enable_if< !is_fundamental<InputIt>::value, int >::type)
	: _compare(comp), _allocator(alloc), _endLeaf(_allocator.allocate(1)), _root(_endLeaf),
		_dummy(_allocator.allocate(1)), _size(0)
{
	_endLeaf->colour = _Node_t::BLACK;
	_endLeaf->parent = NULL;
	_endLeaf->left = NULL;
	_endLeaf->right = NULL;

	_dummy->colour = _Node_t::BLACK;
	_dummy->parent = NULL;
	_dummy->left = NULL;
	_dummy->right = NULL;
	this->insert(first, last);
}

template <class Key, class Compare, class Allocator>
ft::set<Key, Compare, Allocator>::set(set const& src)
	: _compare(src._compare), _allocator(src._allocator), _endLeaf(_allocator.allocate(1)), _root(_endLeaf),
		_dummy(_allocator.allocate(1)), _size(0)
{
	_endLeaf->colour = _Node_t::BLACK;
	_endLeaf->parent = NULL;
	_endLeaf->left = NULL;
	_endLeaf->right = NULL;

	_dummy->colour = _Node_t::BLACK;
	_dummy->parent = NULL;
	_dummy->left = NULL;
	_dummy->right = NULL;
	for (set::const_iterator it = src.begin(); it != src.end(); ++it)
	{
		this->insert(*it);
	}
}

template <class Key, class Compare, class Allocator>
ft::set<Key, Compare, Allocator>&
	ft::set<Key, Compare, Allocator>::operator=(set const& src)
{
	this->clear();
	this->_allocator = src._allocator;
	this->_compare = src._compare;
	this->insert(src.begin(), src.end());
	return (*this);
}

template <class Key, class Compare, class Allocator>
ft::pair<typename ft::set<Key, Compare, Allocator>::iterator, bool>
	ft::set<Key, Compare, Allocator>::insert(value_type const& val) // NOTE: value_type is a key/value pair!!!
{
	_Node_t	*tree = _root;
	_Node_t	*prev = NULL;
	bool		go_left = false; // Avoids extra _compare() call

	if (this->empty())
	{
		_root = _allocator.allocate(1);
		_allocator.construct(_root, _Node_t(val, NULL));
		_repositionEndLeaf(_root);
		++_size;
		return (_correctInsertion(_root, iterator(_root)));
	}

	while (!_isLeaf(tree))
	{
		prev = tree;
		if (this->_compare(val, tree->val) == true) {
			tree = tree->left;
			go_left = true;
		}
		else if (this->_compare(tree->val, val) == true) {
			tree = tree->right;
			go_left = false;
		}
		else // If two keys are equal, unable to insert
			return (ft::make_pair(iterator(tree), false));
	}
	++_size;
	if (go_left) {
		prev->left = _allocator.allocate(1);
		_allocator.construct(prev->left, _Node_t(val, prev));
		return (_correctInsertion(prev->left, iterator(prev->left)));
	}
	else {
		prev->right = _allocator.allocate(1);
		_allocator.construct(prev->right, _Node_t(val, prev));
		if (tree == _endLeaf) // Repositions end leaf correctly
			_repositionEndLeaf(prev->right);
		return (_correctInsertion(prev->right, iterator(prev->right)));
	}
}

template <class Key, class Compare, class Allocator>
typename ft::set<Key, Compare, Allocator>::iterator
	ft::set<Key, Compare, Allocator>::insert(iterator hint, value_type const& val)
{
#if SET_DEBUG_VERBOSE == true
		logstream << BCYN "Entering insert() hint overload." RESET << std::endl;
#endif
	if (hint == this->end())
		return (this->insert(val).first);

	Key	const& hintedKey = *hint;
	iterator next = hint;
	++next;

	if (_compare(hintedKey, val) == true
		&& (next == _endLeaf || _compare(val, *next) == true)) // hintedKey < val < nextKey
	{
#if SET_DEBUG_VERBOSE == true
			logstream << BCYN "Hint looks correct, trying to find slot..." RESET << std::endl;
#endif
		_Node_t	*hintNode = hint._node;
		_Node_t	*nextNode = next._node;
		if (_isLeaf(hintNode->right))
		{
			hintNode->right = _allocator.allocate(1);
			_allocator.construct(hintNode->right, _Node_t(val, hintNode));
			if (_endLeaf->parent == hintNode)
				_repositionEndLeaf(hintNode->right);
			++_size;
			return (_correctInsertion(hintNode->right, iterator(hintNode->right)).first);
		}
		else if (nextNode->left == NULL)
		{
			nextNode->left = _allocator.allocate(1);
			_allocator.construct(nextNode->left, _Node_t(val, nextNode));
			++_size;
			return (_correctInsertion(nextNode->left, iterator(nextNode->left)).first);
		}
	}
#if SET_DEBUG_VERBOSE == true
		logstream << BRED "No insertion optimisation could be done." RESET << std::endl;
#endif
	return (this->insert(val).first);
}

template <class Key, class Compare, class Allocator>
template <class InputIt>
typename ft::enable_if
<
	!ft::is_fundamental<InputIt>::value,
	void
>::type
	ft::set<Key, Compare, Allocator>::insert(InputIt first, InputIt last)
{
	for (; first != last; ++first)
	{
		this->insert(*first);
	}
}

template <class Key, class Compare, class Allocator>
void	ft::set<Key, Compare, Allocator>::erase(iterator pos)
{
	_Node_t	*node = pos._node;

#if SET_DEBUG_VERBOSE == true
		logstream << BBLU "DEBUG: Erasing node " << node->val << RESET << std::endl;
		debug_printTree();
#endif

	if (!_isLeaf(node->left) && (!_isLeaf(node->right) || node->right == _endLeaf))
	{
#if SET_DEBUG_VERBOSE == true
			logstream << BYEL"Node has two children, replacing with predecessor...\n" RESET;
#endif
		_Node_t	*src = node->left;
		while (!_isLeaf(src->right)) // Get the direct in-order predecessor
			src = src->right;
		node->val = src->val;
		node = src;
	}
	_Node_t	*child = _isLeaf(node->left) ? node->right : node->left;
	if (child == NULL) {
#if SET_DEBUG_VERBOSE == true
			logstream << BYEL"Found node with no children\n" RESET;
#endif
		if (_getColour(node) == _Node_t::BLACK)
		{
			child = _dummy;
			child->colour = _Node_t::BLACK;
			child->parent = node;
			node->left = child;
			child->left = NULL;
			child->right = NULL;
		}
	}

	if (_getColour(node) == _Node_t::RED)
	{
#if SET_DEBUG_VERBOSE == true
			logstream << UCYN "Targeted node is red, remove it and don't fix tree" << RESET << std::endl;
#endif
		_removeNodeWithSingleChild(node, child);
	}
	else
	{
#if SET_DEBUG_VERBOSE == true
			logstream << UCYN "Targeted node is black, remove it & DO fix tree" RESET << std::endl;
#endif
		_removeNodeWithSingleChild(node, child);
		_eraseTreeFix(child);
		// In case root has a single red child & is removed, recolor it black
		// otherwise the tree gets corrupted
		_root->colour = _Node_t::BLACK;
	}
	if (child == _dummy) {
		if (child->isLeftChild())
			child->parent->left = NULL;
		else
			child->parent->right = NULL;
		_dummy->parent = NULL;
	}
	--_size;
}

template <class Key, class Compare, class Allocator>
void	ft::set<Key, Compare, Allocator>::erase(iterator first, iterator last)
{
	while (first != last) {
		++first;
		erase(ft::prev(first));
	}
}

template <class Key, class Compare, class Allocator>
typename ft::set<Key, Compare, Allocator>::size_type
	ft::set<Key, Compare, Allocator>::erase(Key const& key)
{
	iterator target = find(key);
	if (target == this->end())
		return (0);
	else {
		erase(target);
		return (1);
	}
}

// https://www.cs.usfca.edu/~galles/visualization/RedBlack.html
// https://www.slideshare.net/ISquareIT/red-black-tree-insertion-deletion
// node is a double black node, which is the child of deleted node
template <class Key, class Compare, class Allocator>
void	ft::set<Key, Compare, Allocator>::_eraseTreeFix(_Node_t *node)
{
	while (node != _root && node != NULL)
	{
		if (_getColour(node) == _Node_t::RED) { // Went far enough up the tree, found red node
#if SET_DEBUG_VERBOSE == true
				logstream << UCYN"In case 1 (red node)" RESET << std::endl;
#endif
			node->colour = _Node_t::BLACK;
			return ;
		}
		else if (_getColour(node->brother()) == _Node_t::RED) // case 2: red brother
		{
#if SET_DEBUG_VERBOSE == true
				logstream << UCYN"In case 2 (red brother)" RESET << std::endl;
#endif
			node->brother()->colour = _Node_t::BLACK; // swap parent and brother's colours
			node->parent->colour = _Node_t::RED;
			if (node->isLeftChild())
				node->parent->rotateLeft(&_root);
			else
				node->parent->rotateRight(&_root);
		}
		if (_getColour(node->brother()->left) == _Node_t::BLACK // case 3: black brother, black nephews
			&& _getColour(node->brother()->right) == _Node_t::BLACK)
		{
#if SET_DEBUG_VERBOSE == true
				logstream << UCYN"In case 3 (black brother and black nephews)" RESET << std::endl;
#endif
			node->brother()->colour = _Node_t::RED;
			node = node->parent;
		}
		else // case 4: black brother, at least one red nephew
		{
#if SET_DEBUG_VERBOSE == true
				logstream << UCYN"In case 4 (black brother, at least one red nephew)" RESET << std::endl;
#endif
			if (node->isLeftChild()) // left symetry
			{
				assert(node->brother() != NULL);
				if (_getColour(node->brother()->right) == _Node_t::BLACK) // subcase: If far nephew isn't red
				{
#if SET_DEBUG_VERBOSE == true
						logstream << UCYN"In subcase 1, far nephew isn't red" RESET << std::endl;
#endif
					node->brother()->rotateRight(&_root);
					node->brother()->colour = _Node_t::BLACK;
					node->brother()->right->colour = _Node_t::RED; // make it so the far nephew is red
				}
				node->brother()->colour = _getColour(node->parent);
				node->parent->colour = _Node_t::BLACK;
				node->brother()->right->colour = _Node_t::BLACK;
				node->parent->rotateLeft(&_root);
				return ;
			}
			else // right symetry
			{
				assert(node->brother() != NULL);
				if (_getColour(node->brother()->left) == _Node_t::BLACK) // If far nephew isn't red
				{
#if SET_DEBUG_VERBOSE == true
						logstream << UCYN"In subcase 1, far nephew isn't red" RESET << std::endl;
#endif
					node->brother()->rotateLeft(&_root);
					node->brother()->colour = _Node_t::BLACK;
					node->brother()->left->colour = _Node_t::RED; // make it so the far nephew is red
				}
				node->brother()->colour = _getColour(node->parent);
				node->parent->colour = _Node_t::BLACK;
				node->brother()->left->colour = _Node_t::BLACK;
				node->parent->rotateRight(&_root);
				return ;
			}
		}
	}
}

template <class Key, class Compare, class Allocator>
void	ft::set<Key, Compare, Allocator>::swap(set& other)
{
	std::swap(this->_root, other._root);
	std::swap(this->_allocator, other._allocator);
	std::swap(this->_compare, other._compare);
	std::swap(this->_endLeaf, other._endLeaf);
	std::swap(this->_size, other._size);
}

// template <class Key, class Compare, class Allocator>
// T&	ft::set<Key, Compare, Allocator>::at(Key const& key)
// {
// 	iterator	target = this->find(key);
// 	if (target != this->end())
// 		return (target->second);
// 	else
// 		throw (ft::out_of_range("set::at: key not found"));
// }

// template <class Key, class Compare, class Allocator>
// T const&	ft::set<Key, Compare, Allocator>::at(Key const& key) const
// {
// 	const_iterator	target = this->find(key);
// 	if (target != this->end())
// 		return (target->second);
// 	else
// 		throw (ft::out_of_range("set::at: key not found"));
// }

// template <class Key, class Compare, class Allocator>
// T&	ft::set<Key, Compare, Allocator>::operator[](Key const& key)
// {
// 	iterator	target = this->find(key);
// 	if (target != this->end())
// 		return (target->second);
// 	else
// 		return (this->insert(ft::make_pair(key, T())).first->second);
// }

template <class Key, class Compare, class Allocator>
typename ft::set<Key, Compare, Allocator>::size_type
	ft::set<Key, Compare, Allocator>::count(Key const& key) const
{
	return (this->find(key) == this->end() ? 0 : 1);
}

template <class Key, class Compare, class Allocator>
typename ft::set<Key, Compare, Allocator>::iterator
	ft::set<Key, Compare, Allocator>::find(Key const& key)
{
	_Node_t	*node = _root;

	while (!_isLeaf(node))
	{
		if (_compare(key, node->val) == true)
			node = node->left;
		else if (_compare(node->val, key) == true)
			node = node->right;
		else
			return (iterator(node));
	}
	return (this->end());
}

template <class Key, class Compare, class Allocator>
typename ft::set<Key, Compare, Allocator>::const_iterator
	ft::set<Key, Compare, Allocator>::find(Key const& key) const
{
	_Node_t	*node = _root;

	while (!_isLeaf(node))
	{
		if (_compare(key, node->val) == true)
			node = node->left;
		else if (_compare(node->val, key) == true)
			node = node->right;
		else
			return (const_iterator(node));
	}
	return (this->end());
}

template <class Key, class Compare, class Allocator>
void	ft::set<Key, Compare, Allocator>::clear(void)
{
	this->_postfix_dealloc(_root);
	_root = _endLeaf;
	_size = 0;
}

template <class Key, class Compare, class Allocator>
ft::pair<
	typename ft::set<Key, Compare, Allocator>::iterator,
	typename ft::set<Key, Compare, Allocator>::iterator
>
ft::set<Key, Compare, Allocator>::equal_range(Key const& key)
{
	iterator lower = lower_bound(key);
	iterator upper = lower;
	while (upper != _endLeaf && _compare(key, *upper) == false)
		++upper;
	return (ft::make_pair(lower, upper));
}

template <class Key, class Compare, class Allocator>
typename ft::set<Key, Compare, Allocator>::iterator
	ft::set<Key, Compare, Allocator>::lower_bound(Key const& key)
{
	for (iterator it = this->begin(); it != this->end(); ++it) {
		if (_compare(*it, key) == false)
			return (it);
	}
	return (this->end());
}

template <class Key, class Compare, class Allocator>
typename ft::set<Key, Compare, Allocator>::const_iterator
	ft::set<Key, Compare, Allocator>::lower_bound(Key const& key) const
{
	for (const_iterator it = this->begin(); it != this->end(); ++it) {
		if (_compare(*it, key) == false)
			return (it);
	}
	return (this->end());
}

template <class Key, class Compare, class Allocator>
typename ft::set<Key, Compare, Allocator>::iterator
	ft::set<Key, Compare, Allocator>::upper_bound(Key const& key)
{
	for (iterator it = this->begin(); it != this->end(); ++it) {
		if (_compare(key, *it) == true)
			return (it);
	}
	return (this->end());
}

template <class Key, class Compare, class Allocator>
typename ft::set<Key, Compare, Allocator>::const_iterator
	ft::set<Key, Compare, Allocator>::upper_bound(Key const& key) const
{
	for (const_iterator it = this->begin(); it != this->end(); ++it) {
		if (_compare(key, *it) == true)
			return (it);
	}
	return (this->end());
}

// ============================================================================================== //
// ------------------------------------- PRIVATE FUNCTIONS -------------------------------------- //
// ============================================================================================== //

template <class Key, class Compare, class Allocator>
void	ft::set<Key, Compare, Allocator>::_postfix_dealloc(_Node_t *root)
{
	if (_isLeaf(root))
		return ;
	_postfix_dealloc(root->left);
	_postfix_dealloc(root->right);
#if SET_DEBUG_VERBOSE == true
		logstream << BBLU "DEBUG: _postfix_dealloc(): " RESET
			<< root->val << '\t' << root->val.second << std::endl;
#endif
	_allocator.destroy(root);
	_allocator.deallocate(root, 1);
}

template <class Key, class Compare, class Allocator>
int	ft::set<Key, Compare, Allocator>::_checkInsertValidity(_Node_t *node) const
{
	if (node->parent == NULL)
		return (CORRECT_ROOT);
	else if (node->parent->colour == _Node_t::BLACK)
		return (CORRECT_NOTHING);
	else if (_getColour(node->uncle()) == _Node_t::RED)
		return (CORRECT_COLOR);
	else // Uncle is black when it is NULL
		return (CORRECT_ROTATE);
}

// &retval is the value to be returned, which may not be an iterator to the same node at *node,
// due to function recursiveness.
template <class Key, class Compare, class Allocator>
ft::pair<typename ft::set<Key, Compare, Allocator>::iterator, bool>
	ft::set<Key, Compare, Allocator>::_correctInsertion(_Node_t *node, iterator const& retval)
{
#if SET_DEBUG_VERBOSE == true
		logstream << std::endl;
		debug_printByLevel(node->val);
#endif
	switch (_checkInsertValidity(node))
	{
		case CORRECT_ROOT:
#if SET_DEBUG_VERBOSE == true
				logstream << _BLU "DEBUG: insert case 1 for node " << node->val
					<< " (recolor root)" RESET << std::endl;
#endif
			node->colour = _Node_t::BLACK;
			return (ft::pair<iterator, bool>(retval, true));

		case CORRECT_NOTHING:
#if SET_DEBUG_VERBOSE == true
				logstream << _BLU "DEBUG: insert case 2 for node " << node->val
					<< " (do nothing)" RESET << std::endl;
#endif
			return (ft::pair<iterator, bool>(retval, true));

		case CORRECT_COLOR:
#if SET_DEBUG_VERBOSE == true
				logstream << _BLU "DEBUG: insert case 3 for node " << node->val
					<< " (recolor parent, uncle and grandparent)" RESET << std::endl;
#endif
			node->parent->colour = _Node_t::BLACK;
			node->uncle()->colour = _Node_t::BLACK;
			node->parent->parent->colour = _Node_t::RED;
			return (_correctInsertion(node->parent->parent, retval));

		case CORRECT_ROTATE:
#if SET_DEBUG_VERBOSE == true
				logstream << _BLU "DEBUG: insert case 4 for node " << node->val
					<< " (rotations and stuff)" RESET << std::endl;
#endif
			_correctInsertion_rotate(node);
			return (ft::pair<iterator, bool>(retval, true));
	}
	throw (std::logic_error("set: insert: _correctInsertion: unexpected fatal logic error"));
}

template <class Key, class Compare, class Allocator>
void	ft::set<Key, Compare, Allocator>::_correctInsertion_rotate(_Node_t *node)
{
	_Node_t	*grandparent = node->parent->parent;

	if (grandparent->left && grandparent->left->right
		&& node == grandparent->left->right)
	{
		node->parent->rotateLeft(&_root);
		node = node->left;
	}
	else if (grandparent->right && grandparent->right->left
		&& node == grandparent->right->left)
	{
		node->parent->rotateRight(&_root);
		node = node->right;
	}

	grandparent = node->parent->parent;
	if (node == node->parent->left)
		grandparent->rotateRight(&_root);
	else
		grandparent->rotateLeft(&_root);
	node->parent->colour = _Node_t::BLACK;
	grandparent->colour = _Node_t::RED;
}

// ============================================================================================== //
// -------------------------------------- DEBUG FUNCTIONS --------------------------------------- //
// ============================================================================================== //

#if SET_DEBUG_VERBOSE == true

	template <class Key, class Compare, class Allocator>
	void	ft::set<Key, Compare, Allocator>::debug_leftRotate(Key const& key)
	{
		iterator	target = this->find(key);
		if (target == this->end())
			throw (std::logic_error("DEBUG: debug_leftRotate: invalid key"));
		target._node->rotateLeft(&_root);
	}

	template <class Key, class Compare, class Allocator>
	void	ft::set<Key, Compare, Allocator>::debug_rightRotate(Key const& key)
	{
		iterator	target = this->find(key);
		if (target == this->end())
			throw (std::logic_error("DEBUG: debug_rightRotate: invalid key"));
		target._node->rotateRight(&_root);
	}

	template <class Key, class Compare, class Allocator>
	void	ft::set<Key, Compare, Allocator>::debug_printByLevel() const
	{
		std::queue<_Node_t *>	queue;
		queue.push(_root);
		logstream << BLUB "  " RESET BBLU "DEBUG: Print by level" RESET << std::endl;
		while (!queue.empty())
		{
			logstream << BLUB " " RESET " "
				<< (queue.front()->colour == _Node_t::RED ? REDB : BLKB)
				<< queue.front()->val << " | " << queue.front()->val.second
				<< RESET << std::endl;
			if (!_isLeaf(queue.front()->left))
				queue.push(queue.front()->left);
			if (!_isLeaf(queue.front()->right))
				queue.push(queue.front()->right);
			queue.pop();
		}
	}
	template <class Key, class Compare, class Allocator>
	void	ft::set<Key, Compare, Allocator>::debug_printByLevel(Key const& highlight_key) const
	{
		std::queue<_Node_t *>	queue;
		queue.push(_root);
		logstream << BLUB "  " RESET BBLU "DEBUG: Print by level" RESET << std::endl;
		while (!queue.empty())
		{
			logstream << BLUB " " RESET " "
				<< (queue.front()->colour == _Node_t::RED ? REDB : BLKB)
				<< queue.front()->val << " | " << queue.front()->val.second
				<< RESET << (queue.front()->val == highlight_key ? BGRN"*" RESET : "")
				<< std::endl;
			if (!_isLeaf(queue.front()->left))
				queue.push(queue.front()->left);
			if (!_isLeaf(queue.front()->right))
				queue.push(queue.front()->right);
			queue.pop();
		}
	}
	template <class Key, class Compare, class Allocator>
	void	ft::set<Key, Compare, Allocator>::debug_printFamily(Key const& key) const
	{
		const_iterator	target = this->find(key);
		if (target == this->end())
			throw (std::logic_error("DEBUG: debug_printFamily: invalid key"));
		logstream << BBLU "DEBUG: Family of node " << key << ": \n" RESET
			<< _YEL"Parent: " RESET
				<< (target._node->parent ? target._node->parent->val : 99999)
				<< (_getColour(target._node->parent) == _Node_t::BLACK ? " (B)" : " " REDB "(R)" RESET) << '\n'
			<< _RED"Left: " RESET
				<< (target._node->left ? target._node->left->val : 99999) << '\n'
				<< (_getColour(target._node->left) == _Node_t::BLACK ? " (B)" : " " REDB "(R)" RESET) << '\n'
			<< _GRN"Right: " RESET
				<< (target._node->right ? target._node->right->val : 99999)
				<< (_getColour(target._node->right) == _Node_t::BLACK ? " (B)" : " " REDB "(R)" RESET) << std::endl;
	}
	template <class Key, class Compare, class Allocator>
	void	ft::set<Key, Compare, Allocator>::debug_printFamily(_Node_t const *node) const
	{
		logstream << BBLU "DEBUG: Family of node " << node->val << ": \n" RESET
			<< _YEL "Parent: " RESET
				<< (node->parent ? node->parent->val : 99999)
				<< (_getColour(node->parent) == _Node_t::BLACK ? " (B)" : " " REDB "(R)" RESET) << '\n'
			<< _RED "Left: " RESET
				<< (node->left ? node->left->val : 99999)
				<< (_getColour(node->left) == _Node_t::BLACK ? " (B)" : " " REDB "(R)" RESET) << '\n'
			<< _GRN "Right: " RESET
				<< (node->right ? node->right->val : 99999)
				<< (_getColour(node->right) == _Node_t::BLACK ? " (B)" : " " REDB "(R)" RESET) << std::endl;
	}

	template <class Key, class Compare, class Allocator>
	void	ft::set<Key, Compare, Allocator>::debug_printTree(const _Node_t *node, std::string prefix) const
	{
		if (_isLeaf(node))
		{
			if (node == _endLeaf)
				logstream << prefix << (node->isLeftChild() ? "├──" : "└──" )
					<< (node->colour == _Node_t::RED ? REDB : BLKB)
					<< "END LEAF" RESET << std::endl;
			// else
			// 	logstream << prefix << BLKB << "LEAF" RESET << std::endl;
			return;
		}

        logstream << prefix << (node->isLeftChild() ? "├──" : "└──" );

        // print the value of the node
        logstream << (node->colour == _Node_t::RED ? REDB : BLKB)
			<< node->val << "," << node->val.second << RESET "\n";

        // enter the next tree level - left and right branch
        debug_printTree(node->left, prefix + (node->isLeftChild() ? "│   " : "    "));
        debug_printTree(node->right, prefix + (node->isLeftChild() ? "│   " : "    "));
	}

	template <class Key, class Compare, class Allocator>
	void	ft::set<Key, Compare, Allocator>::debug_printTree(std::string prefix) const
	{
		logstream << prefix << prefix << BBLU "DEBUG: Print tree" RESET << std::endl;
		debug_printTree(_root, prefix);
	}

#endif
