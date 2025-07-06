#include <gtest/gtest.h>

#include <array>

#include "s21_array.h"

using s21::array;

TEST(ArrayTest, CopyAssignmentOperator) {
  array<int, 3> s21_a = {1, 2, 3};
  array<int, 3> s21_b;
  s21_b = s21_a;

  std::array<int, 3> std_a = {1, 2, 3};
  std::array<int, 3> std_b;
  std_b = std_a;

  for (size_t i = 0; i < 3; ++i) EXPECT_EQ(s21_b[i], std_b[i]);
}

TEST(ArrayTest, AtMethod) {
  array<int, 3> s21_arr = {10, 20, 30};
  std::array<int, 3> std_arr = {10, 20, 30};

  for (size_t i = 0; i < 3; ++i) EXPECT_EQ(s21_arr.at(i), std_arr.at(i));

  EXPECT_THROW(s21_arr.at(3), std::out_of_range);
  EXPECT_THROW(std_arr.at(3), std::out_of_range);
}

TEST(ArrayTest, OperatorSquareBrackets) {
  array<int, 3> s21_arr = {5, 6, 7};
  std::array<int, 3> std_arr = {5, 6, 7};

  for (size_t i = 0; i < 3; ++i) EXPECT_EQ(s21_arr[i], std_arr[i]);
}

TEST(ArrayTest, FrontMethod) {
  array<int, 3> s21_arr = {99, 2, 3};
  std::array<int, 3> std_arr = {99, 2, 3};

  EXPECT_EQ(s21_arr.front(), std_arr.front());
}

TEST(ArrayTest, BackMethod) {
  array<int, 3> s21_arr = {4, 5, 42};
  std::array<int, 3> std_arr = {4, 5, 42};

  EXPECT_EQ(s21_arr.back(), std_arr.back());
}

TEST(ArrayTest, EndIterator) {
  array<int, 3> s21_arr = {1, 2, 3};
  std::array<int, 3> std_arr = {1, 2, 3};

  EXPECT_EQ(*(s21_arr.end() - 1), *(std_arr.end() - 1));
  EXPECT_EQ(s21_arr.end() - s21_arr.begin(), std_arr.end() - std_arr.begin());
}

TEST(ArrayTest, DefaultConstructor) {
  s21::array<int, 3> s21_arr;
  std::array<int, 3> std_arr{};

  for (size_t i = 0; i < 3; ++i) EXPECT_EQ(s21_arr[i], std_arr[i]);
}

TEST(ArrayTest, InitializerListConstructor) {
  array<int, 3> s21_arr = {1, 2};
  std::array<int, 3> std_arr = {1, 2};

  for (size_t i = 0; i < 3; ++i) EXPECT_EQ(s21_arr[i], std_arr[i]);
}

TEST(ArrayTest, InitializerListTooMany) {
  EXPECT_THROW((array<int, 2>{1, 2, 3}), std::out_of_range);
}

TEST(ArrayTest, CopyConstructor) {
  array<int, 3> s21_src = {5, 6, 7};
  array<int, 3> s21_copy(s21_src);
  std::array<int, 3> std_src = {5, 6, 7};
  std::array<int, 3> std_copy(std_src);

  for (size_t i = 0; i < 3; ++i) EXPECT_EQ(s21_copy[i], std_copy[i]);
}

TEST(ArrayTest, MoveConstructor) {
  array<int, 3> s21_src = {1, 2, 3};
  std::array<int, 3> std_src = {1, 2, 3};

  array<int, 3> s21_moved(std::move(s21_src));
  std::array<int, 3> std_moved(std::move(std_src));

  for (size_t i = 0; i < 3; ++i) EXPECT_EQ(s21_moved[i], std_moved[i]);
}

TEST(ArrayTest, MoveAssignmentOperator) {
  array<int, 3> s21_src = {4, 5, 6};
  std::array<int, 3> std_src = {4, 5, 6};

  array<int, 3> s21_dest;
  std::array<int, 3> std_dest;

  s21_dest = std::move(s21_src);
  std_dest = std::move(std_src);

  for (size_t i = 0; i < 3; ++i) EXPECT_EQ(s21_dest[i], std_dest[i]);
}

TEST(ArrayTest, AtValid) {
  array<int, 3> s21_arr = {10, 20, 30};
  std::array<int, 3> std_arr = {10, 20, 30};

  EXPECT_EQ(s21_arr.at(1), std_arr.at(1));
}

TEST(ArrayTest, AtInvalid) {
  array<int, 3> s21_arr = {1, 2, 3};
  std::array<int, 3> std_arr = {1, 2, 3};

  EXPECT_THROW(s21_arr.at(5), std::out_of_range);
  EXPECT_THROW(std_arr.at(5), std::out_of_range);
}

TEST(ArrayTest, OperatorSquareBrackets2) {
  array<int, 3> s21_arr = {1, 2, 3};
  std::array<int, 3> std_arr = {1, 2, 3};

  for (size_t i = 0; i < 3; ++i) EXPECT_EQ(s21_arr[i], std_arr[i]);
}

TEST(ArrayTest, FrontBackAccess) {
  array<int, 3> s21_arr = {9, 8, 7};
  std::array<int, 3> std_arr = {9, 8, 7};

  EXPECT_EQ(s21_arr.front(), std_arr.front());
  EXPECT_EQ(s21_arr.back(), std_arr.back());
}

TEST(ArrayTest, DataPointer) {
  array<int, 3> s21_arr = {1, 2, 3};
  std::array<int, 3> std_arr = {1, 2, 3};

  for (size_t i = 0; i < 3; ++i)
    EXPECT_EQ(*(s21_arr.data() + i), *(std_arr.data() + i));
}

TEST(ArrayTest, Iterators) {
  array<int, 3> s21_arr = {1, 2, 3};
  std::array<int, 3> std_arr = {1, 2, 3};

  auto s21_it = s21_arr.begin();
  auto std_it = std_arr.begin();

  for (size_t i = 0; i < 3; ++i, ++s21_it, ++std_it)
    EXPECT_EQ(*s21_it, *std_it);
}

TEST(ArrayTest, ConstIterators) {
  const array<int, 3> s21_arr = {4, 5, 6};
  const std::array<int, 3> std_arr = {4, 5, 6};

  auto s21_it = s21_arr.begin();
  auto std_it = std_arr.begin();

  for (size_t i = 0; i < 3; ++i, ++s21_it, ++std_it)
    EXPECT_EQ(*s21_it, *std_it);
}

TEST(ArrayTest, EmptyCheck) {
  array<int, 0> s21_empty;
  std::array<int, 0> std_empty;
  EXPECT_EQ(s21_empty.empty(), std_empty.empty());

  array<int, 3> s21_non_empty = {1, 2, 3};
  std::array<int, 3> std_non_empty = {1, 2, 3};
  EXPECT_EQ(s21_non_empty.empty(), std_non_empty.empty());
}

TEST(ArrayTest, SizeAndMaxSize) {
  array<int, 3> s21_arr;
  std::array<int, 3> std_arr;

  EXPECT_EQ(s21_arr.size(), std_arr.size());
  EXPECT_EQ(s21_arr.max_size(), std_arr.max_size());
}

TEST(ArrayTest, Fill) {
  array<int, 4> s21_arr;
  std::array<int, 4> std_arr;

  s21_arr.fill(42);
  std_arr.fill(42);

  for (size_t i = 0; i < 4; ++i) EXPECT_EQ(s21_arr[i], std_arr[i]);
}

TEST(ArrayTest, Swap) {
  array<int, 3> s21_a = {1, 2, 3};
  array<int, 3> s21_b = {4, 5, 6};

  std::array<int, 3> std_a = {1, 2, 3};
  std::array<int, 3> std_b = {4, 5, 6};

  s21_a.swap(s21_b);
  std_a.swap(std_b);

  for (size_t i = 0; i < 3; ++i) {
    EXPECT_EQ(s21_a[i], std_a[i]);
    EXPECT_EQ(s21_b[i], std_b[i]);
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
