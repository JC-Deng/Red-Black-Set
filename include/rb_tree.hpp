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

  using NodeType = TreeNode<ValueType>;

  // Constructor.
  Tree() : root_node(nullptr) {}
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

  // Find the node with minimum value.
  NodeType* min_node() const;
  // Find the minimum value.
  // Pre-condition: The tree must not be empty.
  ValueType min_value() const;
  // Find the node with maximum value.
  NodeType* max_node() const;
  // Find the maximum value.
  // Pre-condition: The tree must not be empty.
  ValueType max_value() const;

  // Find the minimum node that is greater than the given node.
  NodeType* successor(NodeType* x) const;
  // Find the maximum node that is smaller than the given node.
  NodeType* predecessor(NodeType* x) const;

  // Insert x into the red-black tree.
  void insert(const ValueType &x) // TODO
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

  // Remove value x from tree if found.
  void remove(const ValueType &x) // TODO
  {
    NodeType* temp_node;

    if ((temp_node = iter_search(x)) != nullptr) {
      remove(root_node, temp_node);
    }
  }

  // Destroy the tree.
  void destroy() // TODO
  {
    destroy(root_node);
  }

  // Print the tree by rows and show color of each node.
  void manifest() const // TODO
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

  // Validate if the tree still satisfy the requirements of a
  // red-black tree.
  bool is_valid() const // TODO
  {
    return get_black_height(root_node) != -1 &&
      is_color_correct(root_node) &&
      is_value_correct(root_node);
  }

  // Check if the tree is ordered correctly.
  // TODO Move to private.
  bool is_value_correct(NodeType* sub_root) const
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

  // Check if the tree nodes satisfy the color requirement.
  // TODO Move to private.
  bool is_color_correct(NodeType* sub_root) const
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

  // Check if the tree satisfies the black height property.
  // TODO Move to private.
 int get_black_height(NodeType* sub_root) const
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

  // Return the minimum node from a sub-tree.
  NodeType* min_node(NodeType* sub_tree) const;
  // Return the maximum node from a sub-tree.
  NodeType* max_node(NodeType* sub_root) const;

  // Left rotate.
  // TODO
  void left_rotate(NodeType* &sub_root, NodeType* x)
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
  // TODO
  void right_rotate(NodeType* &sub_root, NodeType* y)
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
  // Insert node into sub-tree.
  // TODO
  void insert(NodeType* &sub_root, NodeType* node)
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
  // Rebalance tree after insert.
  // TODO
  void insert_fix(NodeType* &sub_root, NodeType* node)
  {
    NodeType* parent;
    NodeType* grand_parent;

    // If parent node exists, and it is red.
    while ((parent = node->parent) && parent->color == red) {
      grand_parent = parent->parent;

      if (parent == grand_parent->left) {
        NodeType* uncle = grand_parent->right;
        if (uncle && uncle->color == red) {
          uncle->color = black;
          parent->color = black;
          grand_parent->color = red;
          node = grand_parent;
          continue;
        }

        if (parent->right == node) {
          left_rotate(sub_root, parent);
          std::swap(parent, node);
        }

        parent->color = black;
        grand_parent->color = red;
        right_rotate(sub_root, grand_parent);
      } else {
        NodeType* uncle = grand_parent->left;
        if (uncle && uncle->color == red) {
          uncle->color = black;
          parent->color = black;
          grand_parent->color = red;
          node = grand_parent;
          continue;
        }

        if (parent->left == node) {
          right_rotate(sub_root, parent);
          std::swap(parent, node);
        }

        parent->color = black;
        grand_parent->color = red;
        left_rotate(sub_root, grand_parent);
      }
    }

    sub_root->color = black;
  }
  // Remove a given node from a sub-tree.
  // TODO
  void remove(NodeType* &sub_root, NodeType* node)
  {
    NodeType* child;
    NodeType* parent;
    NodeColor color;

    if (node->left != nullptr && node->right != nullptr) {

      NodeType* node_next = node->right;

      while (node_next->left != nullptr) {
        node_next = node_next->left;
      }

      if (node->parent) {
        if (node->parent->left == node) {
          node->parent->left = node_next;
        } else {
          node->parent->right = node_next;
        }
      } else {
        sub_root = node_next;
      }

      child = node_next->right;
      parent = node_next->parent;
      color = node_next->color;

      if (parent == node) {
        parent = node_next;
      } else {
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

      if (node->left != nullptr) {
        child = node->left;
      } else {
        child = node->right;
      }

      parent = node->parent;
      color = node->color;

      if (child) {
        child->parent = parent;
      }

      if (parent) {
        if (parent->left == node) {
          parent->left = child;
        } else {
          parent->right = child;
        }
      } else {
        sub_root = child;
      }

    }

    if (color == black) {
      remove_fix(sub_root, child, parent);
    }

    delete node;
  }
  // Rebalance tree after remove.
  // TODO
  void remove_fix(NodeType* &sub_root,  
    NodeType* node, NodeType* parent)
  {
    NodeType* temp_node;

    while ((!node || node->color == black) && node != sub_root)
    {
      if (parent->left == node) {
        temp_node = parent->right;
        if (temp_node->color == red) {
          temp_node->color = black;
          parent->color = red;
          left_rotate(sub_root, parent);
          temp_node = parent->right;
        }
        if ((!temp_node->left || temp_node->left->color == black) &&
          (!temp_node->right || temp_node->right->color == black))
        {
          temp_node->color = red;
          node = parent;
          parent = node->parent;
        } else {
          if (!temp_node->right ||
            temp_node->right->color == black)
          {
            temp_node->left->color = black;
            temp_node->color = red;
            right_rotate(sub_root, temp_node);
            temp_node = parent->right;
          }
          temp_node->color = parent->color;
          parent->color = black;
          temp_node->right->color = black;
          left_rotate(sub_root, parent);
          node = sub_root;
          break;
        }
      } else {
        temp_node = parent->left;
        if (temp_node->color == red) {
          temp_node->color = black;
          parent->color = red;
          right_rotate(sub_root, parent);
          temp_node = parent->left;
        }
        if ((!temp_node->left || temp_node->left->color == black) &&
          (!temp_node->right || temp_node->right->color == black))
        {
          temp_node->color = red;
          node = parent;
          parent = node->parent;
        } else {
          if (!temp_node->left || temp_node->left->color == black) {
            temp_node->right->color = black;
            temp_node->color = red;
            left_rotate(sub_root, temp_node);
            temp_node = parent->left;
          }
          temp_node->color = parent->color;
          parent->color = black;
          temp_node->left->color = black;
          right_rotate(sub_root, parent);
          node = sub_root;
          break;
        }
      }
    }
    if (node) {
      node->color = black;
    }
  }

  // Destry sub-tree.
  // TODO
  void destroy(NodeType* &sub_tree)
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
Tree<T, Compare>::NodeType*
Tree<T, Compare>::recur_search(const ValueType& x) const
{
  return recur_search(root_node, x);
}
// Recursive search on a sub-tree.
template <class T, class Compare>
Tree<T, Compare>::NodeType*
Tree<T, Compare>::recur_search(NodeType* sub_root,
  const ValueType& x) const
{
  if (sub_root == nullptr || sub_root->value == x) {
    return x;
  }

  if (less_than(x, sub_root->value)) {
    return recur_search(sub_root->left, x);
  } else {
    return recur_search(sub_root->right, x);
  }
}

// Non-recursive search.
template <class T, class Compare>
Tree<T, Compare>::NodeType*
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

// Find the minimum value.
// Pre-condition: The tree must not be empty.
template <class T, class Compare>
Tree<T, Compare>::ValueType
Tree<T, Compare>::min_value() const
{
  return min_node()->value;
}
// Find the node with minimum value.
template <class T, class Compare>
Tree<T, Compare>::NodeType*
Tree<T, Compare>::min_node() const
{
  return min_node(root_node);
}
// Return the minimum node from a sub-tree.
template <class T, class Compare>
Tree<T, Compare>::NodeType*
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

// Find the maximum value.
// Pre-condition: The tree must not be empty.
template <class T, class Compare>
Tree<T, Compare>::ValueType
Tree<T, Compare>::max_value() const
{
  return max_node()->value;
}
// Find the node with maximum value.
template <class T, class Compare>
Tree<T, Compare>::NodeType*
Tree<T, Compare>::max_node() const
{
  return max_node(root_node);
}

// Return the maximum node from a sub-tree.
template <class T, class Compare>
Tree<T, Compare>::NodeType*
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

// Find the minimum node that is greater than the given node.
template <class T, class Compare>
Tree<T, Compare>::NodeType*
Tree<T, Compare>::successor(NodeType* x) const
{
  if (x->right != nullptr) {
    return min_node(x->right);
  }

  NodeType* y = x->parent;
  while (y != nullptr && x == y->right) {
    x = y;
    y = y->parent;
  }

  return y;
}
// Find the maximum node that is smaller than the given node.
template <class T, class Compare>
Tree<T, Compare>::NodeType*
Tree<T, Compare>::predecessor(NodeType* x) const
{
  if (x->left != nullptr) {
    return max_node(x->left);
  }

  NodeType* y = x->parent;
  while (y != nullptr && x == y->left) {
    x = y;
    y = y->parent;
  }

  return y;
}

}