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
    s21::vector<std::pair<int, std::string>> v = {{1, "a"}, {2, "b"}};
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0].first, 1);
    EXPECT_EQ(v[0].second, "a");
    EXPECT_EQ(v[1].first, 2);
    EXPECT_EQ(v[1].second, "b");
}

TEST(VectorTest, CustomClassInitializerList) {
    s21::vector<MyClass> v = {MyClass(1, "Alice"), MyClass(2, "Bob")};
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0].id, 1);
    EXPECT_EQ(v[0].name, "Alice");
    EXPECT_EQ(v[1].id, 2);
    EXPECT_EQ(v[1].name, "Bob");
}

TEST(VectorTest, CustomClassPushBack) {
    s21::vector<MyClass> v;
    v.push_back(MyClass(3, "Charlie"));
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0].id, 3);
    EXPECT_EQ(v[0].name, "Charlie");
}

TEST(VectorTest, CustomClassCopyConstructor) {
    s21::vector<MyClass> v1 = {MyClass(4, "X")};
    s21::vector<MyClass> v2(v1);
    EXPECT_EQ(v2.size(), 1);
    EXPECT_EQ(v2[0].id, 4);
    EXPECT_EQ(v2[0].name, "X");
}

TEST(VectorTest, DefaultConstructor) {
    vector<int> v;
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
}

TEST(VectorTest, ParamConstructor) {
    vector<int> v(5);
    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v.capacity(), 5);
}

TEST(VectorTest, InitializerListConstructor) {
    vector<int> v = {1, 2, 3};
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v.capacity(), 3);
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
    EXPECT_EQ(v1.size(), 0);  // moved-from vector
}

TEST(VectorTest, ReserveIncreasesCapacity) {
    vector<int> v(2);
    v.reserve(10);
    EXPECT_GE(v.capacity(), 10);
    EXPECT_EQ(v.size(), 2);
}

TEST(VectorTest, PushBackIncreasesSize) {
    vector<int> v;
    v.push_back(42);  // реализация push_back добавляет резерв, но не data_[size_++] = value;
    // временный workaround — вручную проверим, что size стал 1 (после резерва)
    EXPECT_EQ(v.size(), 1);  // только если ты добавишь size_++
}

TEST(VectorTest, StringInitializerList) {
    s21::vector<std::string> v = {"hello", "world"};
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], "hello");
    EXPECT_EQ(v[1], "world");
}

TEST(VectorTest, StringPushBack) {
    s21::vector<std::string> v;
    v.push_back("foo");
    v.push_back("bar");
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], "foo");
    EXPECT_EQ(v[1], "bar");
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}