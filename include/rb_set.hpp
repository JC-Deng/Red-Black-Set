#pragma once

#include "rb_tree.hpp"
#include <cstddef>
#include <iterator>
#include <iostream>
#include <utility>
#include <limits>

namespace rb
{

// Mutating, ascending iterator type of the set.
template <class NodeType>
class set_iter
{

public:

  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename std::remove_const_t<NodeType>;
  using difference_type = std::ptrdiff_t;
  using reference = NodeType&;
  using pointer = NodeType*;

  set_iter() : ptr_to_node(nullptr) {}
  set_iter(value_type* node) : ptr_to_node(node) {}
  set_iter(const value_type* node) : ptr_to_node(node) {}

  template <class OtherNode>
  requires std::is_convertible_v<OtherNode*, NodeType*>
  set_iter(const set_iter<OtherNode> &other) :
    ptr_to_node(other.ptr_to_node) {}

  const auto& operator*();
  const pointer operator->();

  set_iter& operator++();
  set_iter operator++(int);

  set_iter& operator--();
  set_iter operator--(int);

  // Checks if the iterator is at an end position.
  bool is_end() const;
  // Returns pointer to node for comparison in interpreter.
  const pointer get_ptr() const;

  template <class OtherNode>
  bool operator==(const set_iter<OtherNode> &other) const
  {
    return ptr_to_node == other.ptr_to_node;
  }
  template <class OtherNode>
  bool operator!=(const set_iter<OtherNode> &other) const
  {
    return ptr_to_node != other.ptr_to_node;
  }

private:

  NodeType* ptr_to_node;
  template <class Key, class Compare> friend class set;
  template <class Iter> friend class c_set_iter;
  template <class Iter> friend class r_set_iter;
  template <class Iter> friend class cr_set_iter;

};


// Mutating, descending iterator type of the set.
template <class Iter>
class r_set_iter
{

public:

  r_set_iter() : iter{} {}
  r_set_iter(Iter normal_iter) : iter{normal_iter} {}
  r_set_iter(const r_set_iter& other) : iter{other.iter} {}

  const auto& operator*();
  const auto* operator->();

  auto& operator++();
  auto operator++(int);

  auto& operator--();
  auto operator--(int);

  template <class OtherNode>
  bool operator==(const r_set_iter<OtherNode>& other) const
  {
    return iter == other.iter;
  }
  
  template <class OtherNode>
  bool operator!=(const r_set_iter<OtherNode>& other) const
  {
    return iter != other.iter;
  }

private:

  Iter iter;

};


template <class Key, class Compare = std::less<Key>>
class set
{

public:

  // Member types of the rb::set.
  using key_type = Key;
  using value_type = Key;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using key_compare = Compare;

  // Reference and pointer types of the rb::set.
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;

  // Iterator types of the rb::set.
  using iterator = set_iter<TreeNode<value_type>>;
  // using const_iterator = set_iter<TreeNode<const value_type>>;
  using const_iterator = iterator;
  using reverse_iterator = r_set_iter<iterator>;
  using const_reverse_iterator = reverse_iterator;

  // This node_type is not C++17 compatible because of
  // implementation differences.
  using node_type = TreeNode<value_type>;

  // Default constructor of set.
  set() : set_data{}, less_than{} {}
  // Destructor of set.
  ~set() {
    set_data.destroy();
  }

  // Copy constructor.
  set(const set& other);
  // Copy assignment operator.
  set& operator=(const set& other);
  // Move constructor.
  set(set&& other);
  // Move assignment operator.
  set& operator=(set&& other);

  // Returns the mutating begin iterator.
  iterator begin() const;
  // Returns the end iterator.
  iterator end() const;
  // Returns the constant begin iterator.
  const_iterator cbegin() const;
  // Returns the end iterator.
  const_iterator cend() const;

  // Returns the mutating reverse begin iterator.
  reverse_iterator rbegin() const;
  // Returns the end iterator.
  reverse_iterator rend() const;
  // Returns the constant reverse begin iterator.
  const_reverse_iterator crbegin() const;
  // Returns the end iterator.
  const_reverse_iterator crend() const;

  // Checks if set is empty.
  bool empty() const;
  // Returns the size of the set.
  size_type size() const;
  // Returns the maximum number of elements that can
  // be held by this container.
  long double max_size() const;

  // Clears the contents of the set.
  void clear();
  // Inserts key into the set. Returns the iterator
  // pointing to the inserted location.
  iterator insert(const value_type& key);
  // Erases the element pointed by the given iterator.
  // Returns the iterator following the last removed element.
  iterator erase(iterator position);
  // Erases the element pointed by the given reverse iterator.
  // Returns the iterator following the last removed element.
  reverse_iterator erase(reverse_iterator position);
  // Erases the key from the set if it exists.
  // Returns the iterator following the last removed element.
  iterator erase(const value_type& key);
  // Swaps the content of the set with the given set.
  void swap(set& other);

  // Returns the number of elements matching specific key.
  size_type count(const value_type& key) const;
  // Finds element with specific key. Returns the iterator
  // pointing to the key if found or end().
  iterator find(const value_type& key);
  // Checks if the container contains element with specific key.
  bool contains(const value_type& key) const;
  // Returns range of elements matching a specific key.
  // The returned std::pair containing a pair of iterators
  // defining the wanted range: the first pointing to the first
  // element that is not less than key andthe second pointing
  // to the first element greater than key.
  // If there are no elements not less than key, past-the-end
  // iterator is returned as the first element.
  // Similarly if there are no elements greater than key,
  // past-the-end iterator is returned as the second element.
  std::pair<iterator, iterator>
    equal_range(const value_type& key);
  // Returns an iterator to the first element not less than
  // the given key.
  iterator lower_bound(const value_type& key);
  // Returns an iterator to the first element greater than
  // the given key.
  iterator upper_bound(const value_type& key);
  // Returns the function that compares keys. Which is the
  // Compare member of the set.
  key_compare key_comp() const;
  // Returns the function that compares values. Which is the
  // Compare member of the set.
  key_compare value_comp() const;

  // Checks if the content of this set equals another set.
  bool operator==(const set& other) const;
  
  // For interpreter to determine whether an iterator belongs
  // to a set.
  const node_type* get_root_ptr() const
  {
    return set_data.root_node;
  }
  
private:

  // Underlying red-black set.
  Tree<key_type, key_compare> set_data;
  Compare less_than;

};


// Definitions of set_iter.
template <class NodeType>
const auto& set_iter<NodeType>::operator*()
{
  return ptr_to_node->value;
}

template <class NodeType>
const set_iter<NodeType>::pointer
set_iter<NodeType>::operator->()
{
  return ptr_to_node;
}

template <class NodeType>
set_iter<NodeType>&
set_iter<NodeType>::operator++()
{
  if (ptr_to_node == nullptr) {
    return *this;
  }
  
  if (ptr_to_node->right != nullptr) {
    ptr_to_node = ptr_to_node->right;

    while (ptr_to_node->left != nullptr) {
      ptr_to_node = ptr_to_node->left;
    }
  } else {
    NodeType* temp_node = ptr_to_node;
    ptr_to_node = ptr_to_node->parent;
    while (ptr_to_node != nullptr &&
      temp_node == ptr_to_node->right)
    {
      temp_node = ptr_to_node;
      ptr_to_node = ptr_to_node->parent;
    }
  }

  return *this;
}

template <class NodeType>
set_iter<NodeType>
set_iter<NodeType>::operator++(int)
{
  if (ptr_to_node == nullptr) {
    return *this;
  }

  auto iter_copy{*this};

  if (ptr_to_node->right != nullptr) {
    ptr_to_node = ptr_to_node->right;

    while (ptr_to_node->left != nullptr) {
      ptr_to_node = ptr_to_node->left;
    }
  } else {
    NodeType* temp_node = ptr_to_node;
    ptr_to_node = ptr_to_node->parent;
    while (ptr_to_node != nullptr &&
      temp_node == ptr_to_node->right)
    {
      temp_node = ptr_to_node;
      ptr_to_node = ptr_to_node->parent;
    }
  }

  return iter_copy;
}

template <class NodeType>
set_iter<NodeType>&
set_iter<NodeType>::operator--()
{
  if (ptr_to_node == nullptr) {
    return *this;
  }

  if (ptr_to_node->left != nullptr) {
    ptr_to_node = ptr_to_node->left;

    while (ptr_to_node->right != nullptr) {
      ptr_to_node = ptr_to_node->right;
    }
  } else {
    NodeType* temp_node = ptr_to_node;
    ptr_to_node = ptr_to_node->parent;
    while (ptr_to_node != nullptr &&
      temp_node == ptr_to_node->left)
    {
      temp_node = ptr_to_node;
      ptr_to_node = ptr_to_node->parent;
    }
  }

  return *this;
}

template <class NodeType>
set_iter<NodeType>
set_iter<NodeType>::operator--(int)
{
  if (ptr_to_node == nullptr) {
    return *this;
  }
  
  auto iter_copy{*this};
  
  if (ptr_to_node->left != nullptr) {
    ptr_to_node = ptr_to_node->left;

    while (ptr_to_node->right != nullptr) {
      ptr_to_node = ptr_to_node->right;
    }
  } else {
    NodeType* temp_node = ptr_to_node;
    ptr_to_node = ptr_to_node->parent;
    while (ptr_to_node != nullptr &&
      temp_node == ptr_to_node->left)
    {
      temp_node = ptr_to_node;
      ptr_to_node = ptr_to_node->parent;
    }
  }

  return iter_copy;
}

template <class NodeType>
bool set_iter<NodeType>::is_end() const
{
  return ptr_to_node == nullptr;
}

template <class NodeType>
const set_iter<NodeType>::pointer
set_iter<NodeType>::get_ptr() const
{
  return ptr_to_node;
}

// Definitions of r_set_iter.

template <class Iter>
const auto& r_set_iter<Iter>::operator*()
{
  return *iter;
}
template <class Iter>
const auto* r_set_iter<Iter>::operator->()
{
  return iter.ptr_to_node;
}

template <class Iter>
auto& r_set_iter<Iter>::operator++()
{
  --iter;
  return *this;
}
template <class Iter>
auto r_set_iter<Iter>::operator++(int)
{
  auto iter_copy{*this};
  --iter;
  return iter_copy;
}

template <class Iter>
auto& r_set_iter<Iter>::operator--()
{
  ++iter;
  return *this;
}
template <class Iter>
auto r_set_iter<Iter>::operator--(int)
{
  auto iter_copy{*this};
  ++iter;
  return iter_copy;
}


// Definitions of set APIs.

// Constructors.

template <class Key, class Compare>
set<Key, Compare>::set(const set& other) :
  set_data{}, less_than{}
{
  for (const auto& x : other) {
    insert(x);
  }
}
template <class Key, class Compare>
set<Key, Compare>&
set<Key, Compare>::operator=(const set& other)
{
  set_data.destroy();
  for (const auto& x : other) {
    insert(x);
  }
  return *this;
}

template <class Key, class Compare>
set<Key, Compare>::set(set&& other) :
  set_data{std::move(other.set_data)},
  less_than{other.less_than} {}
template <class Key, class Compare>
set<Key, Compare>&
set<Key, Compare>::operator=(set&& other)
{
  set_data = std::move(other.set_data);
  return *this;
}

// Iterator APIs.

template <class Key, class Compare>
set<Key, Compare>::iterator
set<Key, Compare>::begin() const
{
  return iterator{set_data.min_node()};
}
template <class Key, class Compare>
set<Key, Compare>::iterator
set<Key, Compare>::end() const
{
  return iterator{};
}
template <class Key, class Compare>
set<Key, Compare>::const_iterator
set<Key, Compare>::cbegin() const
{
  return const_iterator{begin()};
}
template <class Key, class Compare>
set<Key, Compare>::const_iterator
set<Key, Compare>::cend() const
{
  return const_iterator{};
}

template <class Key, class Compare>
set<Key, Compare>::reverse_iterator
set<Key, Compare>::rbegin() const
{
  return reverse_iterator{iterator{set_data.max_node()}};
}
template <class Key, class Compare>
set<Key, Compare>::reverse_iterator
set<Key, Compare>::rend() const
{
  return reverse_iterator{};
}
template <class Key, class Compare>
set<Key, Compare>::const_reverse_iterator
set<Key, Compare>::crbegin() const
{
  return const_reverse_iterator{iterator{set_data.max_node()}};
}
template <class Key, class Compare>
set<Key, Compare>::const_reverse_iterator
set<Key, Compare>::crend() const
{
  return const_reverse_iterator{};
}

// Capacity APIs.

template <class Key, class Compare>
bool set<Key, Compare>::empty() const
{
  return set_data.root_node == nullptr;
}
template <class Key, class Compare>
set<Key, Compare>::size_type
set<Key, Compare>::size() const
{
  return set_data.size();
}
template <class Key, class Compare>
long double
set<Key, Compare>::max_size() const
{
  return std::numeric_limits<Key>::max();
}

// Modifiers.

template <class Key, class Compare>
void set<Key, Compare>::clear()
{
  set_data.destroy();
}
template <class Key, class Compare>
set<Key, Compare>::iterator
set<Key, Compare>::insert(const value_type& key)
{
  if (set_data.find(key) == nullptr) {
    set_data.insert(key);
  }
  return iterator{set_data.find(key)};
}
template <class Key, class Compare>
set<Key, Compare>::iterator
set<Key, Compare>::erase(iterator position)
{
  auto origin_iter = position++;
  set_data.remove(origin_iter->value);
  return position;
}
template <class Key, class Compare>
set<Key, Compare>::reverse_iterator
set<Key, Compare>::erase(reverse_iterator position)
{
  auto origin_iter = position++;
  set_data.remove(origin_iter->value);
  return position;
}
template <class Key, class Compare>
set<Key, Compare>::iterator
set<Key, Compare>::erase(const value_type& key)
{
  auto find_res = set_data.find(key);
  iterator next_iter{};
  if (find_res != nullptr) {
    next_iter = iterator{set_data.successor(find_res)};
  }
  set_data.remove(key);
  return next_iter;
}
template <class Key, class Compare>
void set<Key, Compare>::swap(set& other)
{
  std::swap(set_data.less_than, other.set_data.less_than);
  std::swap(set_data.root_node, other.set_data.root_node);
}

// Lookup APIs.

template <class Key, class Compare>
set<Key, Compare>::size_type
set<Key, Compare>::count(const value_type& key) const
{
  if (set_data.find(key) != nullptr) {
    return 1;
  } else {
    return 0;
  }
}
template <class Key, class Compare>
set<Key, Compare>::iterator
set<Key, Compare>::find(const value_type& key)
{
  auto node_ptr = set_data.find(key);
  auto iter = iterator{};
  if (node_ptr != nullptr) {
    iter = iterator{node_ptr};
  }
  return iter;
}
template <class Key, class Compare>
bool set<Key, Compare>::contains(const value_type& key) const
{
  auto node_ptr = set_data.find(key);
  bool res;
  if (node_ptr != nullptr) {
    res = true;
  } else {
    res = false;
  }
  return res;
}
template <class Key, class Compare>
std::pair<typename set<Key, Compare>::iterator,
  typename set<Key, Compare>::iterator>
set<Key, Compare>::equal_range(const value_type& key)
{
  return std::make_pair(lower_bound(key), upper_bound(key));
}
template <class Key, class Compare>
set<Key, Compare>::iterator
set<Key, Compare>::lower_bound(const value_type& key)
{
  auto node_ptr = set_data.min_node();

  while (node_ptr != nullptr && less_than(node_ptr->value, key)) {
    node_ptr = set_data.successor(node_ptr);
  }

  iterator res{};
  if (node_ptr != nullptr) {
    res = iterator{node_ptr};
  }

  return res;
}
template <class Key, class Compare>
set<Key, Compare>::iterator
set<Key, Compare>::upper_bound(const value_type& key)
{
  auto node_ptr = set_data.min_node();

  while (node_ptr != nullptr && !less_than(key, node_ptr->value)) {
    node_ptr = set_data.successor(node_ptr);
  }

  iterator res{};
  if (node_ptr != nullptr) {
    res = iterator{node_ptr};
  }

  return res;
}

// Observers.

template <class Key, class Compare>
set<Key, Compare>::key_compare
set<Key, Compare>::key_comp() const
{
  return less_than;
}
template <class Key, class Compare>
set<Key, Compare>::key_compare
set<Key, Compare>::value_comp() const
{
  return less_than;
}

// Set compare API.

template <class Key, class Compare>
bool set<Key, Compare>::operator==(const set& other) const
{
  if (size() != other.size()) {
    return false;
  } else {
    auto this_iter = set_data.min_node();
    auto other_iter = other.set_data.min_node();

    while (this_iter != nullptr) {
      if (this_iter->value != other_iter->value) {
        return false;
      } else {
        this_iter = set_data.successor(this_iter);
        other_iter = set_data.successor(other_iter);
      }
    }

    return true;
  }
}

}