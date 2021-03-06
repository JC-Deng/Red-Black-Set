#pragma once

#include <functional>
#include <iostream>
#include <vector>

namespace rb
{

// Node color enum type.
enum NodeColor{red, black};

// Node type of the red-black tree.
template <class T>
class TreeNode
{

public:

  // The type of the elements held by the node.
  using ValueType = T;

  // Members of the set node.
  NodeColor color;
  ValueType value;
  TreeNode* left;
  TreeNode* right;
  TreeNode* parent;

  TreeNode(NodeColor c, const ValueType &v, 
    TreeNode* l, TreeNode* r, TreeNode* p) : 
    color(c), value(v), left(l), right(r), parent() {}

};

template <class T, class Compare = std::less<T>>
class Tree
{

public:

  // The type of the elements held by the container.
  using ValueType = T;

  // An unsigned integral type used to represent sizes.
  using SizeType = std::size_t;

  // The type of the nodes of the tree.
  using NodeType = TreeNode<ValueType>;

  // Constructor.
  Tree() : root_node(nullptr) {}
  Tree(Tree&& other)
  {
    root_node = other.root_node;
    less_than = other.less_than;
    other.root_node = nullptr;
  }
  Tree& operator=(Tree&& other)
  {
    destroy();
    root_node = other.root_node;
    less_than = other.less_than;
    other.root_node = nullptr;
    return *this;
  }
  // Destructor.
  ~Tree() {
    destroy();
  }

  // Pre-order traversal of the tree.
  void pre_order() const;
  // In-order traversal of the tree.
  void in_order() const;
  // Post-order traversal of the tree.
  void post_order() const;

  // Recursive search.
  NodeType* recur_search(const ValueType& x) const;
  // Non-recursive search.
  NodeType* iter_search(const ValueType& x) const;
  // Alias of non-recursive search.
  // Search method of choice can be set here.
  NodeType* find(const ValueType& x) const;

  // Finds the node with minimum value.
  NodeType* min_node() const;
  // Finds the minimum value.
  // Pre-condition: The tree must not be empty.
  ValueType min_value() const;
  // Finds the node with maximum value.
  NodeType* max_node() const;
  // Finds the maximum value.
  // Pre-condition: The tree must not be empty.
  ValueType max_value() const;

  // Finds the minimum node that is greater than the given node.
  NodeType* successor(NodeType* x) const;
  // Finds the maximum node that is smaller than the given node.
  NodeType* predecessor(NodeType* x) const;

  // Inserts x into the red-black tree.
  void insert(const ValueType &x);
  // Removes value x from tree if found.
  void remove(const ValueType &x);
  // Returns the size of the tree.
  SizeType size() const;
  // Returns the size of the sub tree.
  SizeType size(NodeType* sub_root) const;

  // Destroys the tree.
  void destroy();

  // Prints the tree by rows and show color of each node.
  void manifest() const;

  // Validates if the tree still satisfy the requirements of a
  // red-black tree.
  bool is_valid() const;

private:

  // Root node of the red-black tree.
  NodeType* root_node;

  // Comparator.
  Compare less_than;

  // Pre-order traversal of a sub-tree.
  void pre_order(NodeType* sub_root) const;
  // In-order traversal of a sub-tree.
  void in_order(NodeType* sub_root) const;
  // Post-order traversal of a sub-tree.
  void post_order(NodeType* sub_root) const;

  // Recursive search on sub-tree.
  NodeType* recur_search(NodeType* sub_root,
    const ValueType& x) const;

  // Returns the minimum node from a sub-tree.
  NodeType* min_node(NodeType* sub_tree) const;
  // Returns the maximum node from a sub-tree.
  NodeType* max_node(NodeType* sub_root) const;

  // Left rotate.
  void left_rotate(NodeType* &sub_root, NodeType* x);
  // Right rotate.
  void right_rotate(NodeType* &sub_root, NodeType* y);

  // Inserts node into sub-tree.
  void insert(NodeType* &sub_root, NodeType* node);
  // Rebalances tree after insert.
  void insert_fix(NodeType* &sub_root, NodeType* node);

  // Removes a given node from a sub-tree.
  void remove(NodeType* &sub_root, NodeType* node);
  // Rebalances tree after remove.
  void remove_fix(NodeType* &sub_root, NodeType* node,
    NodeType* parent);

  // Destrys sub-tree.
  void destroy(NodeType* &sub_tree);

  // Checks if the tree is ordered correctly.
  bool is_value_correct(NodeType* sub_root) const;
  // Checks if the tree nodes satisfy the color requirement.
  bool is_color_correct(NodeType* sub_root) const;
  // Checks if the tree satisfies the black height property.
  int get_black_height(NodeType* sub_root) const;

  // Makes rb::set a friend class of rb::tree.
  template <class Key, class OtherCompare> friend class set;

};

// Pre-order traversal of the tree.
template <class T, class Compare>
void Tree<T, Compare>::pre_order() const
{
  pre_order(root_node);
  std::cout << '\n';
}
// Pre-order traversal of a sub-tree.
template <class T, class Compare>
void Tree<T, Compare>::pre_order(NodeType* sub_root) const
{
  if (sub_root != nullptr) {
    std::cout << sub_root->value << ' ';
    pre_order(sub_root->left);
    pre_order(sub_root->right);
  }
}

// In-order traversal of the tree.
template <class T, class Compare>
void Tree<T, Compare>::in_order() const
{
  in_order(root_node);
  std::cout << '\n';
}
// In-order traversal of a sub-tree.
template <class T, class Compare>
void Tree<T, Compare>::in_order(NodeType* sub_root) const
{
  if (sub_root != nullptr) {
    in_order(sub_root->left);
    std::cout << sub_root->value << ' ';
    in_order(sub_root->right);
  }
}

// Post-order traversal of the tree.
template <class T, class Compare>
void Tree<T, Compare>::post_order() const
{
  post_order(root_node);
  std::cout << '\n';
}
// Post-order traversal of a sub-tree.
template <class T, class Compare>
void Tree<T, Compare>::post_order(NodeType* sub_root) const
{
  if (sub_root != nullptr) {
    post_order(sub_root->left);
    post_order(sub_root->right);
    std::cout << sub_root->value << ' ';
  }
}

// Recursive search.
template <class T, class Compare>
typename Tree<T, Compare>::NodeType*
Tree<T, Compare>::recur_search(const ValueType& x) const
{
  return recur_search(root_node, x);
}
// Recursive search on a sub-tree.
template <class T, class Compare>
typename Tree<T, Compare>::NodeType*
Tree<T, Compare>::recur_search(NodeType* sub_root,
  const ValueType& x) const
{
  if (sub_root == nullptr || sub_root->value == x) {
    return sub_root;
  }

  if (less_than(x, sub_root->value)) {
    return recur_search(sub_root->left, x);
  } else {
    return recur_search(sub_root->right, x);
  }
}

// Non-recursive search.
template <class T, class Compare>
typename Tree<T, Compare>::NodeType*
Tree<T, Compare>::iter_search(const ValueType& x) const
{
  NodeType* search_node = root_node;

  while (search_node != nullptr && search_node->value != x) {
    if (less_than(x, search_node->value)) {
      search_node = search_node->left;
    } else {
      search_node = search_node->right;
    }
  }

  return search_node;
}

// Alias of non-recursive search.
// Search method of choice can be set here.
template <class T, class Compare>
typename Tree<T, Compare>::NodeType*
Tree<T, Compare>::find(const ValueType& x) const
{
  return iter_search(x);
}

// Finds the minimum value.
// Pre-condition: The tree must not be empty.
template <class T, class Compare>
typename Tree<T, Compare>::ValueType
Tree<T, Compare>::min_value() const
{
  return min_node()->value;
}
// Finds the node with minimum value.
template <class T, class Compare>
typename Tree<T, Compare>::NodeType*
Tree<T, Compare>::min_node() const
{
  return min_node(root_node);
}
// Returns the minimum node from a sub-tree.
template <class T, class Compare>
typename Tree<T, Compare>::NodeType*
Tree<T, Compare>::min_node(NodeType* sub_tree) const
{
  if (sub_tree == nullptr) {
    return nullptr;
  }

  while (sub_tree->left != nullptr) {
    sub_tree = sub_tree->left;
  }
  return sub_tree;
}

// Finds the maximum value.
// Pre-condition: The tree must not be empty.
template <class T, class Compare>
typename Tree<T, Compare>::ValueType
Tree<T, Compare>::max_value() const
{
  return max_node()->value;
}
// Finds the node with maximum value.
template <class T, class Compare>
typename Tree<T, Compare>::NodeType*
Tree<T, Compare>::max_node() const
{
  return max_node(root_node);
}
// Returns the maximum node from a sub-tree.
template <class T, class Compare>
typename Tree<T, Compare>::NodeType*
Tree<T, Compare>::max_node(NodeType* sub_root) const
{
  if (sub_root == nullptr) {
    return nullptr;
  }

  while (sub_root->right != nullptr) {
    sub_root = sub_root->right;
  }
  return sub_root;
}

// Finds the minimum node that is greater than the given node.
template <class T, class Compare>
typename Tree<T, Compare>::NodeType*
Tree<T, Compare>::successor(NodeType* x) const
{
  // If x has right node, the minimum node of its right child
  // is the successor.
  if (x->right != nullptr) {
    return min_node(x->right);
  }

  // If x has no right child, either x is a left child and its
  // successor is its parent, or x is a right child and its
  // successor is the minimum parent that has a left child.
  NodeType* y = x->parent;
  while (y != nullptr && x == y->right) {
    x = y;
    y = y->parent;
  }

  return y;
}
// Finds the maximum node that is smaller than the given node.
template <class T, class Compare>
typename Tree<T, Compare>::NodeType*
Tree<T, Compare>::predecessor(NodeType* x) const
{
  // If x has left child, the predecessor is the maximum node
  // of its left child.
  if (x->left != nullptr) {
    return max_node(x->left);
  }

  // If x has no left child, either x is a right child and its
  // predecessor is its parent, or x is a left child and its
  // predecessor is the minimum parent that has a right child.
  NodeType* y = x->parent;
  while (y != nullptr && x == y->left) {
    x = y;
    y = y->parent;
  }

  return y;
}

// Left rotate.
//    /               /
//   x               y
//  /\              /\
// a  y     ->     x  c
//    /\          /\
//   b  c        a  b
template <class T, class Compare>
void Tree<T, Compare>::left_rotate(NodeType* &sub_root, 
  NodeType* x)
{
  NodeType* y = x->right;

  x->right = y->left;
  if (y->left != nullptr) {
    y->left->parent = x;
  }

  y->parent = x->parent;

  if (x->parent == nullptr) {
    sub_root = y;
  } else {
    if (x->parent->left == x) {
      x->parent->left = y;
    } else {
      x->parent->right = y;
    }
  }

  y->left = x;
  x->parent = y;
}
// Right rotate.
//    /               /
//   x               y
//  /\              /\
// a  y     <-     x  c
//    /\          /\
//   b  c        a  b
template <class T, class Compare>
void Tree<T, Compare>::right_rotate(NodeType* &sub_root,
  NodeType* y)
{
  NodeType* x = y->left;

  y->left = x->right;
  if (x->right != nullptr) {
    x->right->parent = y;
  }

  x->parent = y->parent;

  if (y->parent == nullptr) {
    sub_root = x;
  } else {
    if (y == y->parent->right) {
      y->parent->right = x;
    } else {
      y->parent->left = x;
    }
  }

  x->right = y;
  y->parent = x;
}

// Inserts x into the red-black tree.
template <class T, class Compare>
void Tree<T, Compare>::insert(const ValueType &x)
{
  NodeType* new_node = nullptr;

  // If fails to create a new node, return.
  if ((new_node = new NodeType{black, x,
    nullptr, nullptr, nullptr}) == nullptr)
  {
    return;
  }

  // Insert the new node into main tree.
  insert(root_node, new_node);
}
// Inserts node into sub-tree.
template <class T, class Compare>
void Tree<T, Compare>::insert(NodeType* &sub_root, NodeType* node)
{
  NodeType* y = nullptr;
  NodeType* x = sub_root;

  // Regard the red-black tree as a binary search tree,
  // and add the new node to a binary search tree.
  while (x != nullptr) {
    y = x;
    if (less_than(node->value, y->value)) {
      x = x->left;
    } else {
      x = x->right;
    }
  }

  node->parent = y;
  if (y != nullptr) {
    if (less_than(node->value, y->value)) {
      y->left = node;
    } else {
      y->right = node;
    }
  } else {
    sub_root = node;
  }

  // Set node color to red.
  node->color = red;

  // Fix a sub-tree and make it a binary search tree again.
  insert_fix(sub_root, node);
}
// Rebalances tree after insert.
template <class T, class Compare>
void Tree<T, Compare>::insert_fix(NodeType* &sub_root, NodeType* node)
{
  NodeType* parent;
  NodeType* grand_parent;

  // If parent node exists, and it is red.
  while ((parent = node->parent) && parent->color == red) {
    grand_parent = parent->parent;

    // If the parent is the left child of grand parent.
    if (parent == grand_parent->left) {

      // Case 1: The uncle is red
      NodeType* uncle = grand_parent->right;
      if (uncle && uncle->color == red) {
        uncle->color = black;
        parent->color = black;
        grand_parent->color = red;
        node = grand_parent;

        // Case 1 doesn't transform to other cases. Continue.
        continue;
      }

      // Case 2: The uncle is black, and the current node
      // is a right child.
      if (parent->right == node) {
        left_rotate(sub_root, parent);
        std::swap(parent, node);
      }

      // Case 3: The uncle is black, and the current node
      // is a left child.
      parent->color = black;
      grand_parent->color = red;
      right_rotate(sub_root, grand_parent);
    } else {
      // If the parent is the right child of grand parent.

      // Case 1: The uncle is red.
      NodeType* uncle = grand_parent->left;
      if (uncle && uncle->color == red) {
        uncle->color = black;
        parent->color = black;
        grand_parent->color = red;
        node = grand_parent;
        
        // Case 1 doesn't transform to other cases. Continue.
        continue;
      }

      // Case 2: The uncle is black, and the current node
      // is a left child.
      if (parent->left == node) {
        right_rotate(sub_root, parent);
        std::swap(parent, node);
      }

      // Case 2: The uncle is black, and the current node
      // is a right child.
      parent->color = black;
      grand_parent->color = red;
      left_rotate(sub_root, grand_parent);
    }
  }

  // Set sub-root to black.
  sub_root->color = black;
}

// Removes value x from tree if found.
template <class T, class Compare>
void Tree<T, Compare>::remove(const ValueType &x)
{
  NodeType* temp_node;

  if ((temp_node = iter_search(x)) != nullptr) {
    remove(root_node, temp_node);
  }
}
// Removes a given node from a sub-tree.
template <class T, class Compare>
void Tree<T, Compare>::remove(NodeType* &sub_root, NodeType* node)
{
  NodeType* child;
  NodeType* parent;
  NodeColor color;

  // If the deleted node has no NIL child.
  if (node->left != nullptr && node->right != nullptr) {

    // Get the node that is replacing the deleted node.
    NodeType* node_next = node->right;

    while (node_next->left != nullptr) {
      node_next = node_next->left;
    }

    // Parameter node is not root node.
    if (node->parent) {
      if (node->parent->left == node) {
        node->parent->left = node_next;
      } else {
        node->parent->right = node_next;
      }
    } else {
      // Parameter node is root node.
      // Update it.
      sub_root = node_next;
    }

    // Save the right child of the deleted node
    // for later adjustments.
    child = node_next->right;
    parent = node_next->parent;
    // Save the color of next node.
    color = node_next->color;

    // If the deleted node is the parent of the
    // next node.
    if (parent == node) {
      parent = node_next;
    } else {

      // If child exists.
      if (child) {
        child->parent = parent;
      }
      parent->left = child;

      node_next->right = node->right;
      node->right->parent = node_next;
    }

    node_next->parent = node->parent;
    node_next->color = node->color;
    node_next->left = node->left;
    node->left->parent = node_next;

  } else {
    // If the deleted node has NIL children.

    // Save child information for remove fix.
    if (node->left != nullptr) {
      child = node->left;
    } else {
      child = node->right;
    }

    // Save parent and color information for remove fix.
    parent = node->parent;
    color = node->color;

    if (child) {
      child->parent = parent;
    }

    // If deleted node is not root node.
    if (parent) {
      if (parent->left == node) {
        parent->left = child;
      } else {
        parent->right = child;
      }
    } else {
      // If deleted node is root node.
      sub_root = child;
    }

  }

  // If next node is originally black, rebalance the
  // tree to satisfy the red-black tree requirements.
  if (color == black) {
    remove_fix(sub_root, child, parent);
  }

  delete node;
}
// Rebalances tree after remove.
template <class T, class Compare>
void Tree<T, Compare>::remove_fix(NodeType* &sub_root,
  NodeType* node, NodeType* parent)
{
  NodeType* temp_node;

  // Exit when two requirements are met:
  // 1. node is NIL or node is black.
  // 2. node is not root of the sub-tree.
  while ((!node || node->color == black) && node != sub_root)
  {
    if (parent->left == node) {
      temp_node = parent->right;
      if (temp_node->color == red) {

        // Case 1: The sibling is red.
        temp_node->color = black;
        parent->color = red;
        left_rotate(sub_root, parent);
        temp_node = parent->right;
      }
      if ((!temp_node->left || temp_node->left->color == black) &&
        (!temp_node->right || temp_node->right->color == black))
      {

        // Case 2: The sibling is black, and the children
        // of the sibling are all black.
        temp_node->color = red;
        node = parent;
        parent = node->parent;
      } else {
        if (!temp_node->right ||
          temp_node->right->color == black)
        {

          // Case 3: The sibling is black. The left child
          // of the sibling is red and the right child of the
          // brother is black.
          temp_node->left->color = black;
          temp_node->color = red;
          right_rotate(sub_root, temp_node);
          temp_node = parent->right;
        }

        // Case 4: The sibling is black, and the right child
        // of the sibling is red. Case 3 is guaranteed to become
        // case 4.
        temp_node->color = parent->color;
        parent->color = black;
        temp_node->right->color = black;
        left_rotate(sub_root, parent);
        node = sub_root;
        break;
      }
    } else {
      temp_node = parent->left;
      
      // Case 1: The sibling is red.
      if (temp_node->color == red) {
        temp_node->color = black;
        parent->color = red;
        right_rotate(sub_root, parent);
        temp_node = parent->left;
      }
      if ((!temp_node->left || temp_node->left->color == black) &&
        (!temp_node->right || temp_node->right->color == black))
      {

        // Case 2: The sibling is black, and the children
        // of the sibling are all black.
        temp_node->color = red;
        node = parent;
        parent = node->parent;
      } else {
        if (!temp_node->left || temp_node->left->color == black) {

          // Case 3: The sibling is black. The left child
          // of the sibling is red and the right child of the
          // brother is black.
          temp_node->right->color = black;
          temp_node->color = red;
          left_rotate(sub_root, temp_node);
          temp_node = parent->left;
        }

        // Case 4: The sibling is black, and the right child
        // of the sibling is red.
        temp_node->color = parent->color;
        parent->color = black;
        temp_node->left->color = black;
        right_rotate(sub_root, parent);
        node = sub_root;
        break;
      }
    }
  }

  // Set node color to black.
  if (node) {
    node->color = black;
  }
}
// Returns the size of the tree.
template <class T, class Compare>
typename Tree<T, Compare>::SizeType
Tree<T, Compare>::size() const
{
  return size(root_node);
}
// Returns the size of the sub tree.
template <class T, class Compare>
typename Tree<T, Compare>::SizeType
Tree<T, Compare>::size(NodeType* sub_root) const
{
  if (sub_root == nullptr) {
    return 0;
  }

  SizeType left_size = size(sub_root->left);
  SizeType right_size = size(sub_root->right);

  return left_size + right_size + 1;
}

// Destroys the tree.
template <class T, class Compare>
void Tree<T, Compare>::destroy()
{
  destroy(root_node);
}
// Destrys sub-tree.
template <class T, class Compare>
void Tree<T, Compare>::destroy(NodeType* &sub_tree)
{
  if (sub_tree == nullptr) {
    return;
  }
  if (sub_tree->left != nullptr) {
    destroy(sub_tree->left);
  }
  if (sub_tree->right != nullptr) {
    destroy(sub_tree->right);
  }
  delete sub_tree;
  sub_tree = nullptr;
}

// Prints the tree by rows and show color of each node.
template <class T, class Compare>
void Tree<T, Compare>::manifest() const
{
  if (root_node == nullptr) {
    return;
  }

  std::vector<NodeType*> current_level;
  std::vector<NodeType*> next_level;
  SizeType num_of_level = 0;

  current_level.push_back(root_node);
  while (!current_level.empty()) {
    ++num_of_level;
    for (auto &x : current_level) {
      // std::cout << x->value << ' ';
      std::cout << x->value << '/';
      if (x->color == red) {
        std::cout << "r ";
      }
      if (x->color == black) {
        std::cout << "b ";
      }
    }
    std::cout << '\n';

    next_level.clear();
    for (auto &x : current_level) {
      if (x->left != nullptr) {
        next_level.push_back(x->left);
      }
      if (x->right != nullptr) {
        next_level.push_back(x->right);
      }
    }
    std::swap(current_level, next_level);
  }
  std::cout << '\n';
  // std::cout << "\nNum of Level: " << num_of_level << '\n';
}

// Validates if the tree still satisfy the requirements of a
// red-black tree.
template <class T, class Compare>
bool Tree<T, Compare>::is_valid() const
{
  return is_value_correct(root_node) &&
    is_color_correct(root_node) &&
    get_black_height(root_node) != -1;
}

// Checks if the tree is ordered correctly.
template <class T, class Compare>
bool Tree<T, Compare>::is_value_correct(NodeType* sub_root) const
{
  if (sub_root == nullptr) {
    return true;
  }

  NodeType* start_node = min_node(sub_root);
  NodeType* next_node = successor(start_node);
  bool res = true;

  while (next_node != nullptr) {
    if (!less_than(start_node->value, next_node->value)) {
      res = false;
    }
    start_node = next_node;
    next_node = successor(next_node);
  }

  return res;
}
// Checks if the tree nodes satisfy the color requirement.
template <class T, class Compare>
bool Tree<T, Compare>::is_color_correct(NodeType* sub_root) const
{
  // If the current node is NIL, there will be no problem.
  if (sub_root == nullptr) {
    return true;
  }

  bool left_check = is_color_correct(sub_root->left);
  bool right_check = is_color_correct(sub_root->right);
  bool current_check = false;

  if (sub_root == root_node) {
    if (sub_root->color == black) {
      current_check = true;
    }
  } else {
    if (sub_root->color == black) {
      current_check = true;
    } else if (sub_root->color == red) {
      if ((sub_root->left == nullptr ||
        sub_root->left->color == black) &&
        (sub_root->right == nullptr ||
        sub_root->right->color == black))
      {
        current_check = true;
      }
    }
  }

  return current_check && left_check && right_check;
}
// Checks if the tree satisfies the black height property.
template <class T, class Compare>
int Tree<T, Compare>::get_black_height(NodeType* sub_root) const
{
  if (sub_root == nullptr) {
    return 0;
  }

  int left_height = get_black_height(sub_root->left);
  int right_height = get_black_height(sub_root->right);
  int add;
  
  if (sub_root->color == black) {
    add = 1;
  } else {
    add = 0;
  }

  if (left_height == -1 || right_height == -1 ||
    left_height != right_height)
  {
    return -1;
  } else {
    return left_height + add;
  }
}

}