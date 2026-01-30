#include <gtest/gtest.h>
#include "s21_queue.h"

using s21::queue;
using s21::vector;

TEST(QueueTest, DefaultConstructor) {
    queue<int> q;
    EXPECT_TRUE(q.empty());
    EXPECT_EQ(q.size(), 0u);
}

TEST(QueueTest, InitializerListConstructor) {
    queue<int> q{1, 2, 3};
    EXPECT_FALSE(q.empty());
    EXPECT_EQ(q.size(), 3u);
    EXPECT_EQ(q.front(), 1);
    EXPECT_EQ(q.back(), 3);
}

TEST(QueueTest, CopyConstructor) {
    queue<int> original{1, 2, 3};
    queue<int> copy(original);
    EXPECT_EQ(copy.size(), 3u);
    EXPECT_EQ(copy.front(), 1);
    EXPECT_EQ(copy.back(), 3);
}

TEST(QueueTest, MoveConstructor) {
    queue<int> original{1, 2, 3};
    queue<int> moved(std::move(original));
    EXPECT_EQ(moved.size(), 3u);
    EXPECT_EQ(moved.front(), 1);
    EXPECT_EQ(moved.back(), 3);
}

TEST(QueueTest, MoveAssignmentOperator) {
    queue<int> q1{10, 20, 30};
    queue<int> q2;
    q2 = std::move(q1);
    EXPECT_EQ(q2.size(), 3u);
    EXPECT_EQ(q2.front(), 10);
    EXPECT_EQ(q2.back(), 30);
}

TEST(QueueTest, PushBack) {
    queue<int> q;
    q.push(100);
    EXPECT_EQ(q.size(), 1u);
    EXPECT_EQ(q.front(), 100);
    EXPECT_EQ(q.back(), 100);

    q.push(200);
    EXPECT_EQ(q.size(), 2u);
    EXPECT_EQ(q.front(), 100);
    EXPECT_EQ(q.back(), 200);
}

TEST(QueueTest, PopFront) {
    queue<int> q{10, 20, 30};
    q.pop();
    EXPECT_EQ(q.size(), 2u);
    EXPECT_EQ(q.front(), 20);

    q.pop();
    EXPECT_EQ(q.front(), 30);
    q.pop();
    EXPECT_TRUE(q.empty());
}

TEST(QueueTest, FrontBackConstCorrectness) {
    const queue<int> q{1, 2, 3};
    EXPECT_EQ(q.front(), 1);
    EXPECT_EQ(q.back(), 3);
}

TEST(QueueTest, Swap) {
    queue<int> q1{1, 2, 3};
    queue<int> q2{4, 5};

    q1.swap(q2);

    EXPECT_EQ(q1.size(), 2u);
    EXPECT_EQ(q1.front(), 4);
    EXPECT_EQ(q1.back(), 5);

    EXPECT_EQ(q2.size(), 3u);
    EXPECT_EQ(q2.front(), 1);
    EXPECT_EQ(q2.back(), 3);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}