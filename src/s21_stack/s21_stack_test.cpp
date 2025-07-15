#include <gtest/gtest.h>
#include "s21_stack.h"  // <-- твой Stack
#include "../s21_list/s21_list.h"   // <-- нужный контейнер

using namespace s21;

TEST(StackTest, InsertManyBack) {
  s21::Stack<int> stack;
  stack.insert_many_back(1, 2, 3);  // ← должно вставиться: 1 (дно), 2, 3 (top)

  // Элементы в стеке хранятся в порядке: top() == 3, затем 2, затем 1
  std::vector<int> expected = {3, 2, 1};
  for (int val : expected) {
    ASSERT_FALSE(stack.empty());
    EXPECT_EQ(stack.top(), val);
    stack.pop();
  }

  EXPECT_TRUE(stack.empty());  // после всех pop стек должен быть пуст
}


TEST(StackTest, DefaultConstructor) {
  Stack<int> s;
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.size(), 0);
}

TEST(StackTest, InitListConstructor) {
  Stack<int> s = {1, 2, 3};
  EXPECT_EQ(s.size(), 3);
  EXPECT_EQ(s.top(), 3);  // последний элемент
}

TEST(StackTest, CopyConstructor) {
  Stack<int> s1 = {1, 2, 3};
  Stack<int> s2 = s1;
  EXPECT_EQ(s2.size(), 3);
  EXPECT_EQ(s2.top(), 3);
}

TEST(StackTest, MoveConstructor) {
  Stack<int> s1 = {1, 2, 3};
  Stack<int> s2 = std::move(s1);
  EXPECT_EQ(s2.size(), 3);
  EXPECT_EQ(s2.top(), 3);
}

TEST(StackTest, CopyAssignment) {
  Stack<int> s1 = {1, 2, 3};
  Stack<int> s2;
  s2 = s1;
  EXPECT_EQ(s2.size(), 3);
  EXPECT_EQ(s2.top(), 3);
}

TEST(StackTest, MoveAssignment) {
  Stack<int> s1 = {1, 2, 3};
  Stack<int> s2;
  s2 = std::move(s1);
  EXPECT_EQ(s2.size(), 3);
  EXPECT_EQ(s2.top(), 3);
}

TEST(StackTest, PushLValue) {
  Stack<std::string> s;
  std::string str = "hello";
  s.push(str);
  EXPECT_EQ(s.top(), "hello");
  EXPECT_EQ(s.size(), 1);
}

TEST(StackTest, PushRValue) {
  Stack<std::string> s;
  s.push(std::string("world"));
  EXPECT_EQ(s.top(), "world");
  EXPECT_EQ(s.size(), 1);
}

TEST(StackTest, Pop) {
  Stack<int> s = {1, 2, 3};
  s.pop();
  EXPECT_EQ(s.size(), 2);
  EXPECT_EQ(s.top(), 2);
}

TEST(StackTest, TopConstCorrectness) {
  const Stack<int> s = {1, 2, 3};
  EXPECT_EQ(s.top(), 3);
}

TEST(StackTest, Swap) {
  Stack<int> s1 = {1, 2};
  Stack<int> s2 = {10, 20, 30};

  s1.swap(s2);

  EXPECT_EQ(s1.size(), 3);
  EXPECT_EQ(s1.top(), 30);
  EXPECT_EQ(s2.size(), 2);
  EXPECT_EQ(s2.top(), 2);
}

TEST(StackTest, EmptyAndSize) {
  Stack<int> s;
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.size(), 0);
  s.push(10);
  EXPECT_FALSE(s.empty());
  EXPECT_EQ(s.size(), 1);
}
