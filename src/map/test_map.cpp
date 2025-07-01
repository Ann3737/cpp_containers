#include <gtest/gtest.h>

#include <map>
#include <string>
#include <utility>

#include "rb_tree.h"
#include "s21_map.h"

using s21::map;

TEST(RBTreeTest, EmptyTree) {
  rbtree<int, std::string> tree;
  EXPECT_TRUE(tree.empty());
  EXPECT_EQ(tree.size(), 0);
  EXPECT_EQ(tree.find(42), tree.end());
}

TEST(RBTreeTest, InsertSingleElement) {
  rbtree<int, std::string> tree;
  auto result = tree.insert(1, "one");
  EXPECT_TRUE(result.second);
  EXPECT_EQ(result.first->second, "one");
  EXPECT_FALSE(tree.empty());
  EXPECT_EQ(tree.size(), 1);
}

TEST(RBTreeTest, InsertDuplicateKey) {
  rbtree<int, std::string> tree;
  tree.insert(1, "one");
  auto result = tree.insert(1, "uno");
  EXPECT_FALSE(result.second);
  EXPECT_EQ(result.first->second, "one");
  EXPECT_EQ(tree.size(), 1);
}

TEST(RBTreeTest, InsertOrAssignWorks) {
  rbtree<int, std::string> tree;
  tree.insert_or_assign(1, "one");
  EXPECT_EQ(tree.find(1)->second, "one");
  tree.insert_or_assign(1, "uno");
  EXPECT_EQ(tree.find(1)->second, "uno");
  EXPECT_EQ(tree.size(), 1);
}

TEST(RBTreeTest, FindElement) {
  rbtree<int, std::string> tree;
  tree.insert(10, "ten");
  tree.insert(20, "twenty");
  EXPECT_NE(tree.find(10), tree.end());
  EXPECT_EQ(tree.find(10)->second, "ten");
  EXPECT_EQ(tree.find(30), tree.end());
}

TEST(RBTreeTest, IterationOrder) {
  rbtree<int, std::string> tree;
  tree.insert(3, "three");
  tree.insert(1, "one");
  tree.insert(2, "two");

  std::vector<int> expected_keys = {1, 2, 3};
  std::vector<int> actual_keys;

  for (auto it = tree.begin(); it != tree.end(); ++it) {
    actual_keys.push_back(it->first);
  }

  EXPECT_EQ(actual_keys, expected_keys);
}

TEST(RBTreeTest, ReverseIteration) {
  rbtree<int, std::string> tree;
  tree.insert(1, "a");
  tree.insert(2, "b");
  tree.insert(3, "c");

  auto it = tree.end();
  std::vector<int> keys;
  for (int i = 0; i < 3; ++i) {
    --it;
    keys.push_back(it->first);
  }

  EXPECT_EQ(keys, std::vector<int>({3, 2, 1}));
}

TEST(RBTreeTest, FindExistingElement1) {
  rbtree<int, std::string> tree;
  tree.insert({1, "one"});
  tree.insert({2, "two"});
  auto it = tree.find(2);
  ASSERT_NE(it, tree.cend());
  EXPECT_EQ(it->second, "two");
}

TEST(RBTreeTest, FindNonExistingElement1) {
  rbtree<int, std::string> tree;
  tree.insert({1, "one"});
  auto it = tree.find(42);
  EXPECT_EQ(it, tree.cend());
}

TEST(RBTreeTest, EraseElementReturnsNextIterator1) {
  rbtree<int, std::string> tree;
  tree.insert({1, "one"});
  tree.insert({2, "two"});
  auto it = tree.find(1);
  auto next = tree.erase(it);
  ASSERT_NE(next, tree.cend());
  EXPECT_EQ(next->first, 2);
}

TEST(RBTreeTest, EraseLastElementReturnsEnd1) {
  rbtree<int, std::string> tree;
  tree.insert({1, "one"});
  auto it = tree.find(1);
  auto next = tree.erase(it);
  EXPECT_EQ(next, tree.cend());
}

TEST(RBTreeTest, InsertNewElement1) {
  rbtree<int, std::string> tree;
  auto [it, inserted] = tree.insert({1, "one"});
  EXPECT_TRUE(inserted);
  EXPECT_EQ(it->second, "one");
}

TEST(RBTreeTest, InsertDuplicateElement1) {
  rbtree<int, std::string> tree;
  tree.insert({1, "one"});
  auto [it, inserted] = tree.insert({1, "duplicate"});
  EXPECT_FALSE(inserted);
  EXPECT_EQ(it->second, "one");
}

TEST(RBTreeTest, CbeginPointsToSmallestElement1) {
  rbtree<int, std::string> tree;
  tree.insert({2, "two"});
  tree.insert({1, "one"});
  auto it = tree.cbegin();
  EXPECT_EQ(it->first, 1);
}

TEST(RBTreeTest, CendIsAlwaysNullptr1) {
  rbtree<int, std::string> tree;
  auto it = tree.cend();
  EXPECT_EQ(it, tree.cend());
}

TEST(RBTreeTest, EraseLeafNode1) {
  rbtree<int, std::string> tree;
  tree.insert({10, "a"});
  tree.insert({5, "b"});
  auto it = tree.find(5);
  tree.erase(it);
  EXPECT_EQ(tree.find(5), tree.cend());
}

TEST(RBTreeTest, EraseNodeWithOneChild1) {
  rbtree<int, std::string> tree;
  tree.insert({10, "a"});
  tree.insert({5, "b"});
  tree.insert({2, "c"});
  tree.insert({1, "d"});
  auto it = tree.find(2);
  tree.erase(it);
  EXPECT_EQ(tree.find(2), tree.cend());
}

TEST(RBTreeTest, EraseNodeWithTwoChild1) {
  rbtree<int, std::string> tree;
  tree.insert({10, "a"});
  tree.insert({5, "b"});
  tree.insert({2, "c"});
  auto it = tree.find(5);
  tree.erase(it);
  EXPECT_EQ(tree.find(5), tree.cend());
}

TEST(RBTreeTest, EraseNodeWithTwoChildren1) {
  rbtree<int, std::string> tree;
  tree.insert({10, "a"});
  tree.insert({5, "b"});
  tree.insert({15, "c"});
  tree.insert({7, "d"});
  tree.insert({3, "e"});
  auto it = tree.find(5);
  tree.erase(it);
  EXPECT_EQ(tree.find(5), tree.cend());
}

TEST(RBTreeTest, EraseNodeWithTwoChildren_ConstIterator) {
  rbtree<int, std::string> tree;
  tree.insert({10, "a"});
  tree.insert({5, "b"});
  tree.insert({15, "c"});
  tree.insert({7, "d"});
  tree.insert({3, "e"});
  rbtree<int, std::string>::const_iterator it = tree.find(10);
  rbtree<int, std::string>::iterator next = tree.erase(it);
  EXPECT_EQ(tree.find(10), tree.cend());
}

TEST(RBTreeTest, EraseRootNode1) {
  rbtree<int, std::string> tree;
  tree.insert({10, "root"});
  tree.erase(tree.find(10));
  EXPECT_EQ(tree.find(10), tree.cend());
}

TEST(RBTreeIteratorTest, EndAndDecrement) {
  rbtree<int, std::string> tree;

  tree.insert(10, "ten");
  tree.insert(20, "twenty");
  tree.insert(5, "five");

  auto it_end = tree.end();
  auto it_last = --it_end;
  ASSERT_NE(it_last, tree.end());
  EXPECT_EQ(it_last->first, 20);
  EXPECT_EQ(it_last->second, "twenty");

  --it_last;
  EXPECT_EQ(it_last->first, 10);
  EXPECT_EQ(it_last->second, "ten");

  --it_last;
  EXPECT_EQ(it_last->first, 5);
  EXPECT_EQ(it_last->second, "five");

  --it_last;
  EXPECT_EQ(it_last, tree.end());
}

TEST(RBTreeTest, EraseSingleElement) {
  rbtree<int, std::string> tree;
  auto it = tree.insert(42, "forty-two").first;
  EXPECT_EQ(tree.size(), 1);
  tree.erase(it);
  EXPECT_EQ(tree.size(), 0);
  EXPECT_TRUE(tree.empty());
  EXPECT_EQ(tree.find(42), tree.end());
}

TEST(RBTreeTest, EraseRootWithTwoChildren) {
  rbtree<int, std::string> tree;
  tree.insert(10, "root");
  tree.insert(5, "left");
  tree.insert(15, "right");
  auto it = tree.find(10);
  tree.erase(it);
  EXPECT_EQ(tree.size(), 2);
  EXPECT_EQ(tree.find(10), tree.end());
}

TEST(RBTreeTest, EraseLeaf) {
  rbtree<int, std::string> tree;
  tree.insert(10, "root");
  tree.insert(5, "leaf");
  tree.erase(tree.find(5));
  EXPECT_EQ(tree.find(5), tree.end());
  EXPECT_EQ(tree.size(), 1);
}

TEST(RBTreeTest, EraseNodeWithOneChild) {
  rbtree<int, std::string> tree;
  tree.insert(10, "root");
  tree.insert(5, "left");
  tree.erase(tree.find(10));
  EXPECT_EQ(tree.size(), 1);
  EXPECT_EQ(tree.find(10), tree.end());
}

TEST(RBTreeTest, EraseNodeWithTwoChildren_SuccessorIsRightChild) {
  rbtree<int, std::string> tree;
  tree.insert({20, "root"});
  tree.insert({10, "left"});
  tree.insert({30, "right"});
  tree.insert({15, "left_right"});

  auto it = tree.find(10);
  ASSERT_NE(it, tree.cend());
  tree.erase(it);

  EXPECT_EQ(tree.find(15)->first, 15);
  EXPECT_EQ(tree.find(10), tree.cend());
}

TEST(RBTreeTest, EraseNodeWithTwoChildren_SuccessorIsDeeper) {
  rbtree<int, std::string> tree;
  tree.insert({20, "root"});
  tree.insert({10, "left"});
  tree.insert({30, "right"});
  tree.insert({25, "right_left"});

  auto it = tree.find(20);
  ASSERT_NE(it, tree.cend());
  tree.erase(it);

  EXPECT_EQ(tree.find(25)->first, 25);
  EXPECT_EQ(tree.find(20), tree.cend());
}

TEST(RBTreeTest, EraseNodeWithTwoChildren_SuccessorWithAndWithoutRightChild) {
  rbtree<int, std::string> tree;
  tree.insert({20, "root"});
  tree.insert({10, "left"});
  tree.insert({30, "right"});
  tree.insert({25, "right_left"});
  tree.insert({27, "right_left_right"});

  auto it = tree.find(20);
  ASSERT_NE(it, tree.cend());
  tree.erase(it);

  EXPECT_EQ(tree.find(25)->first, 25);
  EXPECT_EQ(tree.find(27)->first, 27);
  EXPECT_EQ(tree.find(20), tree.cend());

  it = tree.find(25);
  ASSERT_NE(it, tree.cend());
  tree.erase(it);

  EXPECT_EQ(tree.find(25), tree.cend());
  EXPECT_EQ(tree.find(27)->first, 27);
}

TEST(MapTest, DefaultConstructor) {
  map<int, std::string> s21_map;
  std::map<int, std::string> std_map;

  EXPECT_TRUE(s21_map.empty());
  EXPECT_EQ(s21_map.size(), std_map.size());
}

TEST(MapTest, InitializerListConstructor) {
  map<int, std::string> s21_map{{1, "a"}, {2, "b"}};
  std::map<int, std::string> std_map{{1, "a"}, {2, "b"}};

  EXPECT_EQ(s21_map.size(), std_map.size());
  for (const auto& [k, v] : std_map) {
    EXPECT_EQ(s21_map.at(k), v);
  }
}

TEST(MapTest, CopyConstructor) {
  map<int, std::string> original{{1, "a"}, {2, "b"}};
  map<int, std::string> copy(original);

  EXPECT_EQ(copy.size(), original.size());
  for (const auto& [k, v] : original) {
    EXPECT_EQ(copy.at(k), v);
  }
}

TEST(MapTest, MoveConstructor) {
  map<int, std::string> moved_from{{1, "a"}, {2, "b"}};
  map<int, std::string> moved_to(std::move(moved_from));

  EXPECT_EQ(moved_to.size(), 2);
  EXPECT_TRUE(moved_from.empty());
}

TEST(MapTest, CopyAssignment) {
  map<int, std::string> a{{1, "a"}, {2, "b"}};
  map<int, std::string> b;
  b = a;

  EXPECT_EQ(b.size(), a.size());
  for (const auto& [k, v] : a) {
    EXPECT_EQ(b.at(k), v);
  }
}

TEST(MapTest, MoveAssignment) {
  map<int, std::string> a{{1, "a"}, {2, "b"}};
  map<int, std::string> b;
  b = std::move(a);

  EXPECT_EQ(b.size(), 2);
  EXPECT_TRUE(a.empty());
}

TEST(MapTest, AtMethod) {
  map<int, std::string> s21_map{{1, "a"}};
  EXPECT_EQ(s21_map.at(1), "a");
  EXPECT_THROW(s21_map.at(2), std::out_of_range);
}

TEST(MapTest, BracketOperator) {
  map<int, std::string> s21_map;
  s21_map[1] = "a";
  EXPECT_EQ(s21_map.at(1), "a");
  s21_map[2];
  EXPECT_EQ(s21_map.at(2), "");
}

TEST(MapTest, IteratorsBasic) {
  map<int, std::string> s21_map{{1, "a"}, {2, "b"}};
  std::map<int, std::string> std_map{{1, "a"}, {2, "b"}};

  auto it1 = s21_map.begin();
  auto it2 = std_map.begin();

  while (it1 != s21_map.end() && it2 != std_map.end()) {
    EXPECT_EQ(it1->first, it2->first);
    EXPECT_EQ(it1->second, it2->second);
    ++it1;
    ++it2;
  }

  EXPECT_EQ(it1 == s21_map.end(), true);
  EXPECT_EQ(it2 == std_map.end(), true);
}

TEST(MapTest, InsertAndErase) {
  map<int, std::string> s21_map;
  std::map<int, std::string> std_map;

  s21_map.insert({1, "a"});
  std_map.insert({1, "a"});

  s21_map.insert({2, "b"});
  std_map.insert({2, "b"});

  EXPECT_EQ(s21_map.size(), std_map.size());
  EXPECT_EQ(s21_map.at(1), std_map.at(1));
  EXPECT_EQ(s21_map.at(2), std_map.at(2));

  s21_map.erase(s21_map.find(1));
  std_map.erase(1);

  EXPECT_EQ(s21_map.size(), std_map.size());
  EXPECT_TRUE(s21_map.find(1) == s21_map.end());
}

TEST(MapTest, InsertOrAssign) {
  map<int, std::string> s21_map{{1, "a"}};
  auto result = s21_map.insert_or_assign(1, "b");
  EXPECT_FALSE(result.second);
  EXPECT_EQ(s21_map.at(1), "b");
}

TEST(MapTest, SwapMethod) {
  map<int, std::string> a{{1, "a"}};
  map<int, std::string> b{{2, "b"}};

  a.swap(b);
  EXPECT_EQ(a.at(2), "b");
  EXPECT_EQ(b.at(1), "a");
}

TEST(MapTest, MergeMethod) {
  map<int, std::string> a{{1, "a"}, {2, "b"}};
  map<int, std::string> b{{3, "c"}, {1, "x"}};

  a.merge(b);

  EXPECT_EQ(a.size(), 3);
  EXPECT_EQ(a.at(3), "c");
  EXPECT_EQ(a.at(1), "a");
  EXPECT_EQ(b.contains(1), true);
  EXPECT_FALSE(b.contains(3));
}

TEST(MapTest, ContainsMethod) {
  map<int, std::string> m{{1, "a"}};
  EXPECT_TRUE(m.contains(1));
  EXPECT_FALSE(m.contains(2));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}