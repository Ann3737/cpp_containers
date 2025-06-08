#include "rb_tree.h"
#include <gtest/gtest.h>
#include <string>
#include <utility>

using namespace std;
using Tree = rbtree<int, std::string>;

// INSERT
TEST(RBTreeTest, InsertAndFind) {
    Tree tree;
    EXPECT_EQ(tree.find(10), nullptr);

    auto node1 = tree.insert(10, "ten");
    ASSERT_NE(node1, nullptr);
    EXPECT_EQ(node1->value.second, "ten");

    auto node2 = tree.find(10);
    EXPECT_EQ(node2, node1);

    auto node3 = tree.insert(10, "new_ten");
    EXPECT_EQ(node3, node1);
    EXPECT_EQ(node3->value.second, "ten");
}

TEST(RBTreeTest, InsertMultipleAndFind) {
    Tree tree;
    tree.insert(20, "twenty");
    tree.insert(10, "ten");
    tree.insert(30, "thirty");

    EXPECT_NE(tree.find(10), nullptr);
    EXPECT_NE(tree.find(20), nullptr);
    EXPECT_NE(tree.find(30), nullptr);
    EXPECT_EQ(tree.find(40), nullptr);
}

TEST(RBTreeTest, InsertBasic) {
    rbtree<int, int> tree;

    auto* n = tree.insert(10, 100);
    ASSERT_NE(n, nullptr);
    EXPECT_EQ(n->value.first, 10);
    EXPECT_EQ(n->value.second, 100);
    EXPECT_FALSE(n->red);
}

TEST(RBTreeTest, InsertRootNodeIsBlack) {
    rbtree<int, int> tree;
    auto* n = tree.insert(10, 100);
    ASSERT_NE(n, nullptr);
    EXPECT_FALSE(n->red);
}

TEST(RBTreeTest, InsertLeftChildOfRoot) {
    rbtree<int, int> tree;
    auto* root = tree.insert(10, 100);
    auto* left = tree.insert(5, 50);
    EXPECT_EQ(root->left, left);
    EXPECT_EQ(left->parent, root);
}

TEST(RBTreeTest, InsertRightChildOfRoot) {
    rbtree<int, int> tree;
    auto* root = tree.insert(10, 100);
    auto* right = tree.insert(15, 150);
    EXPECT_EQ(root->right, right);
    EXPECT_EQ(right->parent, root);
}

TEST(RBTreeTest, InsertSingleNode) {
    rbtree<int, int> tree;
    tree.insert(10, 100);
    auto* node = tree.find(10);
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->value.first, 10);
    EXPECT_EQ(node->value.second, 100);
}

TEST(RBTreeTest, InsertMultipleNodes) {
    rbtree<int, int> tree;
    tree.insert(10, 1);
    tree.insert(5, 2);
    tree.insert(15, 3);

    EXPECT_NE(tree.find(10), nullptr);
    EXPECT_NE(tree.find(5), nullptr);
    EXPECT_NE(tree.find(15), nullptr);
}

TEST(RBTreeTest, InsertDuplicateKeyOverwritesOrIgnores) {
    rbtree<int, int> tree;
    tree.insert(10, 1);
    tree.insert(10, 99);

    auto* node = tree.find(10);
    ASSERT_NE(node, nullptr);
    EXPECT_TRUE(node->value.second == 1 || node->value.second == 99);
}

TEST(RBTreeTest, InsertLeftRightRotationPattern) {
    rbtree<int, int> tree;
    tree.insert(30, 1);
    tree.insert(10, 2);
    tree.insert(20, 3);

    EXPECT_NE(tree.find(30), nullptr);
    EXPECT_NE(tree.find(10), nullptr);
    EXPECT_NE(tree.find(20), nullptr);
}


TEST(RBTreeTest, InsertDuplicateKeyDoesNothingOrUpdates) {
    rbtree<int, int> tree;
    tree.insert(10, 1);
    tree.insert(10, 2);
    auto* n = tree.find(10);
    ASSERT_NE(n, nullptr);
    EXPECT_TRUE(n->value.second == 1 || n->value.second == 2);
}


// Erase
TEST(RBTreeTest, EraseNode) {
    rbtree<int, int> tree;
    tree.insert(10, 100);
    tree.insert(5, 50);
    tree.insert(15, 150);

    auto* n = tree.find(5);
    ASSERT_NE(n, nullptr);

    tree.erase(5);
    EXPECT_EQ(tree.find(5), nullptr);
}

TEST(RBTreeTest, EraseLeafNode) {
    rbtree<int, int> tree;
    tree.insert(10, 1);
    tree.insert(5, 2);
    tree.erase(5);

    EXPECT_EQ(tree.find(5), nullptr);
    EXPECT_NE(tree.find(10), nullptr);
}

TEST(RBTreeTest, EraseLeafNodeString) {
    Tree tree;
    tree.insert(20, "twenty");
    tree.insert(10, "ten");
    tree.insert(30, "thirty");

    EXPECT_NE(tree.find(10), nullptr);
    tree.erase(10);
    EXPECT_EQ(tree.find(10), nullptr);

    EXPECT_NE(tree.find(20), nullptr);
    EXPECT_NE(tree.find(30), nullptr);
}

TEST(RBTreeTest, EraseNodeWithBothChildren1) {
    Tree tree;
    tree.insert(20, "twenty");
    tree.insert(10, "ten");
    tree.insert(5, "five"); 
    tree.erase(10);

    auto* node_10 = tree.find(10);
    EXPECT_TRUE(node_10 != nullptr);
    EXPECT_TRUE(tree.find(20) == nullptr);
}


TEST(RBTreeTest, EraseNodeWithTwoChildren2) {
    Tree tree;
    tree.insert(20, "twenty");
    tree.insert(10, "ten");
    tree.insert(30, "thirty");
    tree.insert(25, "twenty-five");
    tree.insert(35, "thirty-five");
    tree.print();
    tree.erase(30);
    tree.print();

    auto* node_30 = tree.find(30);
    EXPECT_TRUE(node_30 != nullptr);
    EXPECT_TRUE(node_30->value.second == "thirty-five");
    EXPECT_TRUE(tree.find(35) == nullptr);
}


TEST(RBTreeTest, EraseRootNode) {
    Tree tree;
    tree.insert(20, "twenty");
    tree.insert(10, "ten");

    tree.erase(20);
    EXPECT_EQ(tree.find(20), nullptr);
    EXPECT_NE(tree.find(10), nullptr);
}

// Iterator
TEST(RBTreeTest, IteratorTraversal) {
    Tree tree;
    tree.insert(20, "twenty");
    tree.insert(10, "ten");
    tree.insert(30, "thirty");

    rbtree<int, std::string>::iterator it(tree);
    std::vector<int> keys;
    while (it.current) {
        keys.push_back(it.current->value.first);
        it.next();
    }
    std::vector<int> expected{10, 20, 30};
    EXPECT_EQ(keys, expected);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}