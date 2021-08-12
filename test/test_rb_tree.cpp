#define CATCH_CONFIG_MAIN
#include "rb_tree.hpp"
#include <catch2/catch.hpp>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <random>
#include <limits>
#include <set>

using namespace rb;

std::random_device rand_seed;
std::mt19937_64 rand_engine{rand_seed()};
std::uniform_int_distribution<long long> distribution;
constexpr std::size_t test_quantity = 1'000;

template <class T>
bool same_with_std(const Tree<T>& rb_tree, const std::set<T>& std_set)
{
  if (rb_tree.size() != std_set.size()) {
    return false;
  }

  TreeNode<T>* rb_iter = rb_tree.min_node();
  auto std_iter = std_set.cbegin();
  while (rb_iter != nullptr) {
    if (rb_iter->value != *std_iter) {
      return false;
    }
    rb_iter = rb_tree.successor(rb_iter);
    ++std_iter;
  }

  return true;
}

TEMPLATE_TEST_CASE("Constructor tests.", "[ctor]",
  int, long, long long, float, double, long double)
{
  Tree<TestType> test_tree;
  std::set<TestType> test_std_set;
  REQUIRE(same_with_std(test_tree, test_std_set));

  for (int i = -20; i < 20; ++i) {
    test_tree.insert((TestType)i);
    test_std_set.insert((TestType)i);
  }
  REQUIRE(same_with_std(test_tree, test_std_set));

  Tree<TestType> test_move_tree{std::move(test_tree)};
  REQUIRE(test_tree.size() == 0);
  REQUIRE(same_with_std(test_move_tree, test_std_set));

  auto test_move_assign_tree = std::move(test_move_tree);
  REQUIRE(test_move_tree.size() == 0);
  REQUIRE(same_with_std(test_move_assign_tree, test_std_set));
}

TEMPLATE_TEST_CASE("Search functions.", "[search]",
  int, long, long long, float, double, long double)
{
  Tree<TestType> test_tree;
  std::set<TestType> test_std_set;
  
  // Randomly insert elements so that branches in insert_fix()
  // can be mostly covered.
  for (int i = 0; i < test_quantity; ++i) {
    auto temp = (TestType)distribution(rand_engine);
    if (test_tree.find(temp) == nullptr) {
      test_tree.insert(temp);
      test_std_set.insert(temp);
    }
  }

  bool search_test_flag = true;
  for (auto& x : test_std_set) {
    auto search_res = test_tree.recur_search(x);
    if (search_res == nullptr) {
      search_test_flag = false;
    }
    search_res = test_tree.iter_search(x);
    if (search_res == nullptr) {
      search_test_flag = false;
    }
    search_res = test_tree.find(x);
    if (search_res == nullptr) {
      search_test_flag = false;
    }
  }
  REQUIRE(search_test_flag);

  test_tree.destroy();
  search_test_flag = false;
  for (auto& x : test_std_set) {
    auto search_res = test_tree.recur_search(x);
    if (search_res != nullptr) {
      search_test_flag = true;
    }
    search_res = test_tree.iter_search(x);
    if (search_res != nullptr) {
      search_test_flag = true;
    }
    search_res = test_tree.find(x);
    if (search_res != nullptr) {
      search_test_flag = true;
    }
  }
  REQUIRE_FALSE(search_test_flag);
}

TEMPLATE_TEST_CASE("Min/max functions.", "[limit]",
  int, long, long long, float, double, long double)
{
  Tree<TestType> test_tree;
  std::set<TestType> test_std_set;
  
  for (int i = 0; i < test_quantity; ++i) {
    auto temp = (TestType)distribution(rand_engine);
    if (test_tree.find(temp) == nullptr) {
      test_tree.insert(temp);
      test_std_set.insert(temp);
    }
  }

  auto min_node_ptr = test_tree.min_node();
  auto max_node_ptr = test_tree.max_node();
  auto min_value = test_tree.min_value();
  auto max_value = test_tree.max_value();

  REQUIRE(min_node_ptr->value == *test_std_set.begin());
  REQUIRE(max_node_ptr->value == *test_std_set.rbegin());
  REQUIRE(min_value == *test_std_set.begin());
  REQUIRE(max_value == *test_std_set.rbegin());
}

TEMPLATE_TEST_CASE("Successor and predecessor.", "[iter]",
  int, long, long long, float, double, long double)
{
  Tree<TestType> test_tree;
  std::set<TestType> test_std_set;
  
  for (int i = 0; i < test_quantity; ++i) {
    auto temp = (TestType)distribution(rand_engine);
    if (test_tree.find(temp) == nullptr) {
      test_tree.insert(temp);
      test_std_set.insert(temp);
    }
  }

  bool iter_test_flag = true;
  auto tree_iter = test_tree.min_node();

  for (auto& x : test_std_set) {
    if (tree_iter->value != x) {
      iter_test_flag = false;
    }
    tree_iter = test_tree.successor(tree_iter);
  }

  REQUIRE(iter_test_flag);

  iter_test_flag = true;
  auto tree_riter = test_tree.max_node();
  for (auto i = test_std_set.rbegin();
    i != test_std_set.rend(); ++i)
  {
    if (tree_riter->value != *i) {
      iter_test_flag = false;
    }
    tree_riter = test_tree.predecessor(tree_riter);
  }
  REQUIRE(iter_test_flag);
}

TEMPLATE_TEST_CASE("size(), is_valid() and supplementary tests"
  "of insert, remove.", "[misc]",
  int, long, long long, float, double, long double)
{
  Tree<TestType> test_tree;
  std::set<TestType> test_std_set;

  bool test_flag = true;
  for (int i = 0; i < test_quantity; ++i) {
    auto temp = (TestType)distribution(rand_engine);
    if (test_tree.find(temp) == nullptr) {
      test_tree.insert(temp);
      test_std_set.insert(temp);
      if (test_tree.size() != test_std_set.size()) {
        test_flag = false;
      }
      if (!test_tree.is_valid()) {
        test_flag = false;
      }
    }
  }
  REQUIRE(test_flag);

  test_flag = true;
  while (test_std_set.size() != 0) {
    // Randomly remove from reference set so that the branches
    // in remove_fix() can be mostly covered.
    auto rand_num = rand()%test_std_set.size();
    auto iter = std::begin(test_std_set);
    std::advance(iter, rand_num);

    test_tree.remove(*iter);
    test_std_set.erase(*iter);
    // if (test_tree.size() != test_std_set.size()) {
    if (!same_with_std(test_tree, test_std_set)) {
      test_flag = false;
    }
    if (!test_tree.is_valid()) {
      test_flag = false;
    }
  }
  REQUIRE(test_flag);
}