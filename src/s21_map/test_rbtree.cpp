#include <map>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "rb_tree.h"

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

TEST(RBTreeTest, NewTree) {
  rbtree<int, std::string> tree;
  tree.insert({10, "ten"});
  tree.insert({5, "five"});
  tree.insert({15, "fifteen"});
  tree.insert({1, "one"});
  tree.insert({6, "six"});
  tree.insert({12, "twelve"});
  tree.insert({17, "seventeen"});
  tree.insert({0, "null"});
  auto it = tree.find(15);
  tree.erase(it);
  ASSERT_NE(it, tree.cend());
}

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

TEST(RBTreeTest, DeleteRedLeaf) {
  rbtree<int, std::string> tree;
  tree.insert({10, "ten"});
  tree.insert({5, "five"});
  tree.insert({15, "fifteen"});

  auto it = tree.find(15);
  ASSERT_NE(it, tree.cend());
  tree.erase(it);

  ASSERT_EQ(tree.find(15), tree.cend());
}

TEST(RBTreeTest, DeleteBlackLeaf) {
  rbtree<int, std::string> tree;
  tree.insert({10, "ten"});
  tree.insert({5, "five"});
  tree.insert({15, "fifteen"});
  tree.insert({1, "one"});
  tree.insert({6, "six"});
  tree.insert({0, "zero"});

  auto it = tree.find(0);
  ASSERT_NE(it, tree.cend());
  tree.erase(it);

  ASSERT_EQ(tree.find(0), tree.cend());
}

TEST(RBTreeTest, DeleteNodeWithOneRedChild) {
  rbtree<int, std::string> tree;
  tree.insert({10, "ten"});
  tree.insert({5, "five"});
  tree.insert({1, "one"});

  auto it = tree.find(5);
  ASSERT_NE(it, tree.cend());
  tree.erase(it);

  ASSERT_EQ(tree.find(5), tree.cend());
  ASSERT_NE(tree.find(1), tree.cend());
}

TEST(RBTreeTest, DeleteNodeWithTwoChildren) {
  rbtree<int, std::string> tree;
  tree.insert({20, "twenty"});
  tree.insert({10, "ten"});
  tree.insert({30, "thirty"});
  tree.insert({25, "twenty-five"});
  tree.insert({35, "thirty-five"});

  auto it = tree.find(30);
  ASSERT_NE(it, tree.cend());
  tree.erase(it);

  ASSERT_EQ(tree.find(30), tree.cend());
  ASSERT_NE(tree.find(25), tree.cend());
  ASSERT_NE(tree.find(35), tree.cend());
}

TEST(RBTreeTest, DeleteRootNode) {
  rbtree<int, std::string> tree;
  tree.insert({10, "ten"});
  tree.insert({5, "five"});
  tree.insert({15, "fifteen"});

  auto it = tree.find(10);
  ASSERT_NE(it, tree.cend());
  tree.erase(it);

  ASSERT_EQ(tree.find(10), tree.cend());
  ASSERT_NE(tree.find(5), tree.cend());
  ASSERT_NE(tree.find(15), tree.cend());
}

TEST(RBTreeTest, SequentialDeletions) {
  rbtree<int, std::string> tree;
  std::vector<int> values = {10, 5, 15, 1, 6, 12, 17, 0};
  for (int v : values) {
    tree.insert({v, std::to_string(v)});
  }
  for (int v : values) {
    auto it = tree.find(v);
    ASSERT_NE(it, tree.cend());
    tree.erase(it);
    ASSERT_EQ(tree.find(v), tree.cend());
  }

  ASSERT_EQ(tree.begin(), tree.end());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}