#include <gtest/gtest.h>

#include <map>
#include <string>
#include <vector>

#include "s21_map.h"

using s21::map;

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

TEST(MapTest, DestructorTriggersClear) {
  map<int, std::string> local_map{{1, "a"}, {2, "b"}};
  EXPECT_EQ(local_map.size(), 2);
}

TEST(MapTest, BracketOperatorGetExistingKey) {
  map<int, std::string> s21_map{{1, "a"}};
  EXPECT_EQ(s21_map[1], "a");
}

TEST(MapTest, ClearMethod) {
  map<int, std::string> s21_map{{1, "a"}, {2, "b"}};
  s21_map.clear();
  EXPECT_TRUE(s21_map.empty());
}

TEST(MapTest, BeginAndEnd) {
  map<int, std::string> s21_map{{1, "a"}, {2, "b"}};
  auto it = s21_map.begin();
  EXPECT_EQ(it->first, 1);
  ++it;
  EXPECT_EQ(it->first, 2);
  ++it;
  EXPECT_EQ(it, s21_map.end());
}

TEST(MapTest, ConstAtMethod) {
  const map<int, std::string> m{{1, "a"}};
  EXPECT_EQ(m.at(1), "a");
  EXPECT_THROW(m.at(2), std::out_of_range);
}

TEST(MapTest, ConstIteration) {
  const map<int, std::string> m{{1, "a"}, {2, "b"}};
  auto it = m.cbegin();
  EXPECT_EQ(it->first, 1);
  ++it;
  EXPECT_EQ(it->first, 2);
  ++it;
  EXPECT_EQ(it, m.cend());
}

TEST(MapTest, ConstFindMethod) {
  const map<int, std::string> m{{1, "a"}};
  EXPECT_NE(m.find(1), m.end());
  EXPECT_EQ(m.find(2), m.end());
}

TEST(MapTest, MaxSize) {
  map<int, std::string> m;
  std::map<int, std::string> std_m;
  EXPECT_GT(m.max_size(), 0u);
  EXPECT_GE(m.max_size(), std_m.max_size());
}

TEST(MapTest, InsertByKeyAndValue) {
  map<int, std::string> m;
  auto [it, inserted] = m.insert(1, "a");
  EXPECT_TRUE(inserted);
  EXPECT_EQ(it->second, "a");
}

TEST(MapTest, EraseConstIterator) {
  map<int, std::string> m{{1, "a"}, {2, "b"}};
  auto it = m.find(1);
  const map<int, std::string>::const_iterator cit = it;
  auto next = m.erase(cit);
  EXPECT_EQ(m.find(1), m.end());
  EXPECT_EQ(next->first, 2);
}

TEST(MapTest, BracketOperatorExistingKey) {
  map<int, std::string> m;
  m[1] = "hello";
  m[1];
  EXPECT_EQ(m[1], "hello");
}

TEST(MapTest, MaxSizeIsPositive) {
  map<int, std::string> m;
  EXPECT_GT(m.max_size(), 0u);
}

TEST(MapTest, InitListWithDuplicateKeys) {
  map<int, std::string> m{{1, "a"}, {1, "b"}, {2, "c"}};
  EXPECT_EQ(m.size(), 2);
  EXPECT_EQ(m.at(1), "a");
  EXPECT_EQ(m.at(2), "c");
}

TEST(MapTest, DestructorCoversClearBranch) {
  map<int, std::string>* m = new map<int, std::string>{{1, "a"}, {2, "b"}};
  delete m;
  SUCCEED();
}

TEST(MapTest, BracketOperatorInsertAndOverwrite) {
  map<int, std::string> m;
  m[1] = "hello";
  m[1] = "world";
  EXPECT_EQ(m[1], "world");
}

TEST(MapTest, ConstCBeginAndCEnd) {
  const map<int, std::string> m{{1, "a"}, {2, "b"}};
  std::vector<int> keys;
  for (auto it = m.cbegin(); it != m.cend(); ++it) {
    keys.push_back(it->first);
  }
  EXPECT_EQ(keys, (std::vector<int>{1, 2}));
}

TEST(MapTest, ConstMaxSize) {
  const map<int, std::string> m;
  EXPECT_GT(m.max_size(), 0u);
}

TEST(MapTest, EraseConstIteratorDirectCall) {
  map<int, std::string> m{{1, "a"}, {2, "b"}, {3, "c"}};
  auto it = m.find(2);
  m.erase(static_cast<map<int, std::string>::const_iterator>(it));
  EXPECT_EQ(m.find(2), m.end());
}

TEST(MapInsertManyTest, InsertUniqueKeys) {
  map<int, std::string> m;
  auto result =
      m.insert_many(std::make_pair(1, "one"), std::make_pair(2, "two"),
                    std::make_pair(3, "three"));

  EXPECT_EQ(m.size(), 3);
  EXPECT_EQ(m.at(1), "one");
  EXPECT_EQ(m.at(2), "two");
  EXPECT_EQ(m.at(3), "three");

  for (const auto& [it, success] : result) {
    EXPECT_TRUE(success);
  }
}

TEST(MapInsertManyTest, InsertDuplicateKeys) {
  map<int, std::string> m = {{1, "one"}};
  auto result =
      m.insert_many(std::make_pair(1, "uno"), std::make_pair(2, "two"));

  EXPECT_EQ(m.size(), 2);
  EXPECT_EQ(m.at(1), "one");
  EXPECT_EQ(m.at(2), "two");

  EXPECT_FALSE(result[0].second);
  EXPECT_TRUE(result[1].second);
}

TEST(MapInsertManyTest, EmptyInsertMany) {
  map<int, std::string> m;
  auto result = m.insert_many();

  EXPECT_TRUE(result.empty());
  EXPECT_TRUE(m.empty());
}

TEST(MapInsertManyTest, InsertManyWithRvaluePairs) {
  map<int, std::string> m;
  auto result = m.insert_many(std::make_pair(10, "ten"),
                              std::pair<int, std::string>{20, "twenty"},
                              std::pair<const int, std::string>{30, "thirty"});

  EXPECT_EQ(m.size(), 3);
  EXPECT_EQ(m.at(10), "ten");
  EXPECT_EQ(m.at(20), "twenty");
  EXPECT_EQ(m.at(30), "thirty");

  for (const auto& [it, success] : result) {
    EXPECT_TRUE(success);
  }
}
