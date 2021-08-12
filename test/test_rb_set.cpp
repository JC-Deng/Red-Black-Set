#define CATCH_CONFIG_MAIN
#include "rb_set.hpp"
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
bool same_with_std(const set<T>& rb_set, const std::set<T>& std_set)
{
  if (rb_set.size() != std_set.size()) {
    return false;
  }

  auto const_iter = rb_set.cbegin();
  for (auto& x : std_set) {
    if (*const_iter != x) {
      return false;
    }
    ++const_iter;
  }

  return true;
}

TEMPLATE_TEST_CASE("Constructors.", "[ctor]",
  int, long, long long, float, double, long double)
{
  set<TestType> test_set;
  std::set<TestType> std_set;

  REQUIRE(test_set.empty());

  for (int i = 0; i < 20; ++i) {
    test_set.insert((TestType)i);
    std_set.insert((TestType)i);
  }
  REQUIRE(same_with_std(test_set, std_set));

  auto test_copy{test_set};
  REQUIRE(same_with_std(test_copy, std_set));

  auto test_copy_assign = test_set;
  REQUIRE(same_with_std(test_copy_assign, std_set));

  auto test_move{std::move(test_copy)};
  REQUIRE(test_copy.empty());
  REQUIRE(same_with_std(test_move, std_set));

  auto test_move_assign = std::move(test_move);
  REQUIRE(test_move.empty());
  REQUIRE(same_with_std(test_move_assign, std_set));
}

TEMPLATE_TEST_CASE("Iterator tests.", "[iter]",
  int, long, long long, float, double, long double)
{
  set<TestType> test_set;
  std::set<TestType> std_set;

  for (int i = 0; i < test_quantity; i++) {
    auto temp = (TestType)distribution(rand_engine);
    test_set.insert(temp);
    std_set.insert(temp);
  }

  bool iter_test_flag = true;
  auto std_iter = std_set.begin();
  for (auto iter = test_set.begin();
    iter != test_set.end(); ++iter)
  {
    if (*iter != *std_iter) {
      iter_test_flag = false;
    }
    ++std_iter;
  }
  REQUIRE(iter_test_flag);

  iter_test_flag = true;
  std_iter = std_set.begin();
  auto rb_iter = test_set.begin();
  auto std_iter_next = ++std_iter;
  --std_iter;
  while (std_iter_next != std_set.end()) {
    ++std_iter;
    ++std_iter_next;
    ++rb_iter;
  }
  for (;rb_iter != test_set.end(); --rb_iter) {
    if (*rb_iter != *std_iter) {
      iter_test_flag = false;
    }
    --std_iter;
  }
  REQUIRE(iter_test_flag);

  iter_test_flag = true;
  auto std_citer = std_set.cbegin();
  for (auto iter = test_set.cbegin();
    iter != test_set.cend(); ++iter)
  {
    if (*iter != *std_citer) {
      iter_test_flag = false;
    }
    ++std_citer;
  }
  REQUIRE(iter_test_flag);

  iter_test_flag = true;
  std_citer = std_set.cbegin();
  auto rb_citer = test_set.cbegin();
  auto std_citer_next = ++std_citer;
  --std_citer;
  while (std_citer_next != std_set.end()) {
    ++std_citer;
    ++std_citer_next;
    ++rb_citer;
  }
  for (;rb_citer != test_set.end(); --rb_citer) {
    if (*rb_citer != *std_citer) {
      iter_test_flag = false;
    }
    --std_citer;
  }
  REQUIRE(iter_test_flag);

  iter_test_flag = true;
  auto std_riter = std_set.rbegin();
  for (auto iter = test_set.rbegin();
    iter != test_set.rend(); ++iter)
  {
    if (*iter != *std_riter) {
      iter_test_flag = false;
    }
    ++std_riter;
  }
  REQUIRE(iter_test_flag);

  iter_test_flag = true;
  std_riter = std_set.rbegin();
  auto rb_riter = test_set.rbegin();
  auto std_riter_next = ++std_riter;
  --std_riter;
  while (std_riter_next != std_set.rend()) {
    ++std_riter;
    ++std_riter_next;
    ++rb_riter;
  }
  for (;rb_riter != test_set.rend(); --rb_riter) {
    if (*rb_riter != *std_riter) {
      iter_test_flag = false;
    }
    --std_riter;
  }
  REQUIRE(iter_test_flag);

  iter_test_flag = true;
  auto std_criter = std_set.crbegin();
  for (auto iter = test_set.crbegin();
    iter != test_set.crend(); ++iter)
  {
    if (*iter != *std_criter) {
      iter_test_flag = false;
    }
    ++std_criter;
  }
  REQUIRE(iter_test_flag);

  iter_test_flag = true;
  std_criter = std_set.rbegin();
  auto rb_criter = test_set.rbegin();
  auto std_criter_next = ++std_criter;
  --std_criter;
  while (std_criter_next != std_set.crend()) {
    ++std_criter;
    ++std_criter_next;
    ++rb_criter;
  }
  for (;rb_criter != test_set.crend(); --rb_criter) {
    if (*rb_criter != *std_criter) {
      iter_test_flag = false;
    }
    --std_criter;
  }
  REQUIRE(iter_test_flag);

  auto test_copy{test_set};

  for (auto iter = test_set.begin();
    iter != test_set.end(); ++iter)
  {
    test_set.erase(iter);
  }
  REQUIRE(test_set.empty());

  for (auto iter = test_copy.rbegin();
    iter != test_copy.rend(); ++iter)
  {
    test_copy.erase(iter);
  }
  REQUIRE(test_copy.empty());
}

TEMPLATE_TEST_CASE("Modifier tests.", "[mod]",
  int, long, long long, float, double, long double)
{
  set<TestType> test_set;
  std::set<TestType> std_set;

  for (int i = 0; i < test_quantity; ++i) {
    auto temp = (TestType)distribution(rand_engine);
    test_set.insert(temp);
    std_set.insert(temp);
  }
  REQUIRE(same_with_std(test_set, std_set));

  bool mod_test_flag = true;
  auto test_copy{test_set};
  auto test_std_copy{std_set};

  while (test_std_copy.size() != 0) {
    auto rand_num = rand()%test_std_copy.size();
    auto iter = std::begin(test_std_copy);
    std::advance(iter, rand_num);

    test_copy.erase(*iter);
    test_std_copy.erase(*iter);
    if (!same_with_std(test_copy, test_std_copy)) {
      mod_test_flag = false;
    }
  }
  REQUIRE(mod_test_flag);

  mod_test_flag = true;
  test_copy = test_set;
  test_std_copy = std_set;

  while (test_std_copy.size() != 0) {
    auto rand_num = rand()%test_std_copy.size();
    auto iter = std::begin(test_std_copy);
    std::advance(iter, rand_num);

    auto erase_iter = test_copy.find(*iter);
    test_copy.erase(erase_iter);
    test_std_copy.erase(*iter);
    if (!same_with_std(test_copy, test_std_copy)) {
      mod_test_flag = false;
    }
  }
  REQUIRE(mod_test_flag);

  test_copy = test_set;
  set<TestType> test_swap;
  test_swap.swap(test_copy);
  REQUIRE(test_copy.empty());
  REQUIRE(same_with_std(test_swap, std_set));

  test_copy = std::move(test_set);
  test_copy.clear();
  REQUIRE(test_copy.empty());
  REQUIRE(test_set.empty());
}

TEMPLATE_TEST_CASE("Observer tests.", "[observer]",
  int, long, long long, float, double, long double)
{
  set<TestType> test_set;
  set<TestType> other_set;
  auto test_number = (TestType)5;
  
  REQUIRE(std::numeric_limits<TestType>::max() ==
    test_set.max_size());

  REQUIRE(test_set.count(test_number) == 0);
  REQUIRE(test_set.contains(test_number) == false);
  REQUIRE(test_set.equal_range(test_number) ==
    std::make_pair(test_set.end(), test_set.end()));
  REQUIRE(test_set.lower_bound(test_number) == test_set.end());
  REQUIRE(test_set.upper_bound(test_number) == test_set.end());
  REQUIRE(test_set == other_set);

  for (int i = 0; i < 20; ++i) {
    test_set.insert((TestType)i);
  }
  REQUIRE(test_set.count(test_number) == 1);
  REQUIRE(test_set.contains(test_number) == true);
  REQUIRE(test_set.equal_range(test_number) ==
    std::make_pair(test_set.find(test_number),
    test_set.find(test_number + 1)));
  REQUIRE(test_set.lower_bound(test_number) ==
    test_set.find(test_number));
  REQUIRE(test_set.upper_bound(test_number) ==
    test_set.find(test_number + 1));
  REQUIRE_FALSE(test_set == other_set);
  for (int i = 0; i < 20; ++i) {
    other_set.insert((TestType)i);
  }
  REQUIRE(test_set == other_set);

  for (int i = 0; i < 20; ++i) {
    test_set.insert(test_number);
  }
  REQUIRE(test_set.size() == 20);
  REQUIRE(test_set.count(test_number) == 1);
  REQUIRE(test_set.contains(test_number) == true);
  REQUIRE(test_set.equal_range(test_number) ==
    std::make_pair(test_set.find(test_number),
    test_set.find(test_number + 1)));
  REQUIRE(test_set.lower_bound(test_number) ==
    test_set.find(test_number));
  REQUIRE(test_set.upper_bound(test_number) ==
    test_set.find(test_number + 1));
  REQUIRE(test_set == other_set);
  for (int i = 0; i < 20; ++i) {
    other_set.insert((TestType)i);
  }
  REQUIRE(test_set == other_set);

  test_set.clear();
  REQUIRE(test_set.count(test_number) == 0);
  REQUIRE(test_set.contains(test_number) == false);  
  REQUIRE(test_set.equal_range(test_number) ==
      std::make_pair(test_set.end(), test_set.end()));
  REQUIRE(test_set.lower_bound(test_number) == test_set.end());
  REQUIRE(test_set.upper_bound(test_number) == test_set.end());
  REQUIRE_FALSE(test_set == other_set);
  other_set.clear();
  REQUIRE(test_set == other_set);

  std::less<TestType> ref_obj;
  auto key_comp_obj = test_set.key_comp();
  auto value_comp_obj = test_set.value_comp();
  REQUIRE(ref_obj((TestType)1, (TestType)2) ==
    key_comp_obj((TestType)1, (TestType)2));
  REQUIRE(ref_obj((TestType)2, (TestType)1) ==
    key_comp_obj((TestType)2, (TestType)1));
  REQUIRE(ref_obj((TestType)2, (TestType)2) ==
    key_comp_obj((TestType)2, (TestType)2));
  REQUIRE(ref_obj((TestType)1, (TestType)2) ==
    value_comp_obj((TestType)1, (TestType)2));
  REQUIRE(ref_obj((TestType)2, (TestType)1) ==
    value_comp_obj((TestType)2, (TestType)1));
  REQUIRE(ref_obj((TestType)2, (TestType)2) ==
    value_comp_obj((TestType)2, (TestType)2));
}