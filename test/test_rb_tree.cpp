#include "rb_tree.hpp"
#include <iostream>
#include <chrono>
#include <random>
#include <limits>
#include <set>

using namespace rb;

int main(int argc, char const *argv[])
{
  Tree<std::size_t> test_tree;

  std::random_device rand_seed;
  std::mt19937_64 rand_engine{rand_seed()};
  std::uniform_int_distribution<std::size_t> distribution;

  std::set<std::size_t> test_set;

  for (int i = 0; i < 20; ++i) {
    auto temp = distribution(rand_engine);
    test_set.insert(temp);
    test_tree.insert(temp);

    if (!test_tree.is_valid()) {
      std::cout << "Oops.\n";
    }
  }

  test_tree.insert(12345);
  auto search_node = test_tree.iter_search(12345);
  std::cout << search_node->value << '\n';

  std::cout << test_tree.min_value() << '\n';
  std::cout << test_tree.max_value() << '\n';

  for (auto &x : test_set) {
    test_tree.remove(x);

    if (!test_tree.is_valid()) {
      std::cout << "Ooooops.\n";
    }
  }

  test_tree.manifest();

  // for (int i = 3; i < 9; i += 2) {
  //   test_tree.remove(i);
  //   test_tree.manifest();
  // }

  // // Order test.
  // for (int i = 0; i < 9; ++i) {
  //   test_tree.insert(i);
  // }
  // test_tree.manifest();

  // test_tree.remove(3);
  // test_tree.remove(5);
  // test_tree.remove(7);

  // auto current_node = test_tree.max_node();

  // while (test_tree.predecessor(current_node) != nullptr) {
  //   std::cout << current_node->value << '/';
  //   if (current_node->color == black) {
  //     std::cout << "b ";
  //   } else {
  //     std::cout << "r ";
  //   }
  //   current_node = test_tree.predecessor(current_node);
  // }
  // std::cout << current_node->value << '/';
  // if (current_node->color == black) {
  //   std::cout << "b ";
  // } else {
  //   std::cout << "r ";
  // }
  // std::cout << '\n';

  return 0;
}
