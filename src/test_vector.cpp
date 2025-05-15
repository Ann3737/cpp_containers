#include <gtest/gtest.h>
#include <string>
#include <utility>
#include "s21_vector.h"

using s21::vector;

struct MyClass {
    int id;
    std::string name;

    MyClass() : id(0), name("default") {}
    MyClass(int i, std::string n) : id(i), name(std::move(n)) {}
    bool operator==(const MyClass& other) const {
        return id == other.id && name == other.name;
    }
};

TEST(VectorTest, PairConstructorAndAccess) {
    vector<std::pair<int, std::string>> v = {{1, "a"}, {2, "b"}};
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0].first, 1);
    EXPECT_EQ(v[0].second, "a");
    EXPECT_EQ(v[1].first, 2);
    EXPECT_EQ(v[1].second, "b");
}

TEST(VectorTest, CustomClassInitializerList) {
    vector<MyClass> v = {MyClass(1, "Alice"), MyClass(2, "Bob")};
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0].id, 1);
    EXPECT_EQ(v[0].name, "Alice");
    EXPECT_EQ(v[1].id, 2);
    EXPECT_EQ(v[1].name, "Bob");
}

TEST(VectorTest, CustomClassPushBack) {
    vector<MyClass> v;
    v.push_back(MyClass(3, "Charlie"));
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0].id, 3);
    EXPECT_EQ(v[0].name, "Charlie");
}

TEST(VectorTest, CustomClassCopyConstructor) {
    vector<MyClass> v1 = {MyClass(4, "X")};
    vector<MyClass> v2(v1);
    EXPECT_EQ(v2.size(), 1);
    EXPECT_EQ(v2[0].id, 4);
    EXPECT_EQ(v2[0].name, "X");
}

TEST(VectorTest, DefaultConstructor) {
    vector<int> v;
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
    EXPECT_TRUE(v.empty());
}

TEST(VectorTest, ParamConstructor) {
    vector<int> v(5);
    EXPECT_EQ(v.size(), 5);
    EXPECT_GE(v.capacity(), 5);
}

TEST(VectorTest, InitializerListConstructor) {
    vector<int> v = {1, 2, 3};
    EXPECT_EQ(v.size(), 3);
    EXPECT_GE(v.capacity(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST(VectorTest, CopyConstructor) {
    vector<int> v1 = {4, 5, 6};
    vector<int> v2(v1);
    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v2[0], 4);
    EXPECT_EQ(v2[1], 5);
    EXPECT_EQ(v2[2], 6);
}

TEST(VectorTest, MoveConstructor) {
    vector<int> v1 = {7, 8, 9};
    vector<int> v2(std::move(v1));
    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v2[0], 7);
    EXPECT_EQ(v2[1], 8);
    EXPECT_EQ(v2[2], 9);
    EXPECT_EQ(v1.size(), 0);  // moved-from vector should be empty
}

TEST(VectorTest, ReserveIncreasesCapacity) {
    vector<int> v(2);
    size_t old_capacity = v.capacity();
    v.reserve(old_capacity + 10);
    EXPECT_GE(v.capacity(), old_capacity + 10);
    EXPECT_EQ(v.size(), 2);
}

TEST(VectorTest, PushBackIncreasesSize) {
    vector<int> v;
    v.push_back(42);
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 42);
}

TEST(VectorTest, StringInitializerList) {
    vector<std::string> v = {"hello", "world"};
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], "hello");
    EXPECT_EQ(v[1], "world");
}

TEST(VectorTest, StringPushBack) {
    vector<std::string> v;
    v.push_back("foo");
    v.push_back("bar");
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], "foo");
    EXPECT_EQ(v[1], "bar");
}

TEST(VectorTest, ElementAccessAndAt) {
    vector<int> v{10, 20, 30};
    EXPECT_EQ(v.at(0), 10);
    EXPECT_EQ(v.at(2), 30);
    EXPECT_THROW(v.at(3), std::out_of_range);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v.front(), 10);
    EXPECT_EQ(v.back(), 30);
    int* p = v.data();
    EXPECT_EQ(p[0], 10);
}

TEST(VectorTest, Iterators) {
    vector<int> v{1, 2, 3};
    EXPECT_EQ(*v.begin(), 1);
    EXPECT_EQ(*(v.end() - 1), 3);
    EXPECT_NE(v.cbegin(), v.cend());
}

TEST(VectorTest, CapacityAndEmpty) {
    vector<int> v;
    EXPECT_TRUE(v.empty());
    v.push_back(1);
    EXPECT_FALSE(v.empty());
}

TEST(VectorTest, ModifiersPushPopInsertErase) {
    vector<int> v{1, 2, 3};
    v.push_back(4);
    EXPECT_EQ(v.back(), 4);
    size_t old_size = v.size();

    v.pop_back();
    EXPECT_EQ(v.size(), old_size - 1);

    v.insert(v.begin() + 1, 42);
    EXPECT_EQ(v[1], 42);
    EXPECT_EQ(v.size(), old_size);

    v.erase(v.begin() + 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v.size(), old_size - 1);
}

TEST(VectorTest, SwapClearShrinkToFit) {
    vector<int> v1{10, 20};
    vector<int> v2{1, 2, 3};
    v2.swap(v1);
    EXPECT_EQ(v1.size(), 3);
    EXPECT_EQ(v2.size(), 2);

    v2.clear();
    EXPECT_TRUE(v2.empty());

    v1.reserve(100);
    EXPECT_GE(v1.capacity(), 100);

    v1.shrink_to_fit();
    EXPECT_EQ(v1.capacity(), v1.size());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
