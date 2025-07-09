#include "s21_set.h"

#include <gtest/gtest.h>

#include <algorithm>  // std::sort
#include <random>     // std::shuffle
#include <vector>

using s21::set;

TEST(RBTreeTest, NewTree) {
  set<int> tree;
  tree.insert(10);
  tree.insert(5);
  tree.insert(15);
  tree.insert(1);
  tree.insert(6);
  tree.insert(12);
  tree.insert(17);
  tree.insert(0);
  auto it = tree.find(15);
  tree.erase(it);
  ASSERT_NE(it, tree.end());
}

TEST(RBTreeTest, EraseElementReturnsNextIterator1) {
  set<int> tree;
  tree.insert(1);
  tree.insert(2);
  auto it = tree.find(1);
  tree.erase(it);
  auto next = tree.begin();

  ASSERT_NE(next, tree.end());
}

TEST(RBTreeTest, EraseLastElementReturnsEnd1) {
  set<int> tree;
  tree.insert(1);
  auto it = tree.find(1);
  tree.erase(it);
  auto next = tree.begin();
  EXPECT_EQ(next, tree.end());
}

TEST(RBTreeTest, EraseLeafNode1) {
  set<int> tree;
  tree.insert(10);
  tree.insert(5);
  auto it = tree.find(5);
  tree.erase(it);
  it = tree.find(5);
  EXPECT_EQ(it, tree.end());
}

TEST(RBTreeTest, EraseNodeWithOneChild1) {
  set<int> tree;
  tree.insert(10);
  tree.insert(5);
  tree.insert(2);
  tree.insert(1);
  auto it = tree.find(2);
  tree.erase(it);
  auto iter = tree.find(2);
  EXPECT_EQ(iter, tree.end());
}

TEST(RBTreeTest, EraseNodeWithTwoChild1) {
  set<int> tree;
  tree.insert(10);
  tree.insert(5);
  tree.insert(2);
  auto it = tree.find(5);
  tree.erase(it);
  auto iter = tree.find(5);
  EXPECT_EQ(iter, tree.end());
}

TEST(RBTreeTest, EraseNodeWithTwoChildren1) {
  set<int> tree;
  tree.insert(10);
  tree.insert(5);
  tree.insert(15);
  tree.insert(7);
  tree.insert(3);
  auto it = tree.find(5);
  tree.erase(it);
  it = tree.find(5);
  EXPECT_EQ(it, tree.end());
}

TEST(RBTreeTest, EraseRootNode1) {
  set<int> tree;
  tree.insert(10);
  tree.erase(tree.find(10));
  auto it = tree.find(10);
  EXPECT_EQ(it, tree.end());
}

TEST(RBTreeTest, EraseSingleElement) {
  set<int> tree;
  auto it = tree.insert(42).first;
  EXPECT_EQ(tree.size(), 1);
  tree.erase(it);
  EXPECT_EQ(tree.size(), 0);
  EXPECT_TRUE(tree.empty());
  it = tree.find(42);
  EXPECT_EQ(it, tree.end());
}

TEST(RBTreeTest, EraseRootWithTwoChildren) {
  set<int> tree;
  tree.insert(10);
  tree.insert(5);
  tree.insert(15);
  auto it = tree.find(10);
  tree.erase(it);
  EXPECT_EQ(tree.size(), 2);
  it = tree.find(10);
  EXPECT_EQ(it, tree.end());
}

TEST(RBTreeTest, EraseLeaf) {
  set<int> tree;
  tree.insert(10);
  tree.insert(5);
  tree.erase(tree.find(5));
  auto it = tree.find(5);
  EXPECT_EQ(it, tree.end());
  EXPECT_EQ(tree.size(), 1);
}

TEST(RBTreeTest, EraseNodeWithOneChild) {
  set<int> tree;
  tree.insert(10);
  tree.insert(5);
  tree.erase(tree.find(10));
  EXPECT_EQ(tree.size(), 1);
  auto it = tree.find(10);
  EXPECT_EQ(it, tree.end());
}

TEST(RBTreeTest, EraseNodeWithTwoChildren_SuccessorIsRightChild) {
  set<int> tree;
  tree.insert(20);
  tree.insert(10);
  tree.insert(30);
  tree.insert(15);

  auto it = tree.find(10);
  ASSERT_NE(it, tree.end());
  tree.erase(it);
  it = tree.find(15);

  EXPECT_EQ(*it, 15);
  it = tree.find(10);
  EXPECT_EQ(it, tree.end());
}

TEST(RBTreeTest, EraseNodeWithTwoChildren_SuccessorIsDeeper) {
  set<int> tree;
  tree.insert(20);
  tree.insert(10);
  tree.insert(30);
  tree.insert(25);

  auto it = tree.find(20);
  ASSERT_NE(it, tree.end());
  tree.erase(it);

  // Преемник 20 — это 25
  it = tree.find(25);
  ASSERT_NE(it, tree.end());
  EXPECT_EQ(*it, 25);

  // 20 удалён
  it = tree.find(20);
  EXPECT_EQ(it, tree.end());

  // 10 остался
  it = tree.find(10);
  EXPECT_NE(it, tree.end());
}

TEST(RBTreeTest, EraseNodeWithTwoChildren_SuccessorWithAndWithoutRightChild) {
  set<int> tree;
  tree.insert(20);
  tree.insert(10);
  tree.insert(30);
  tree.insert(25);
  tree.insert(27);

  auto it = tree.find(20);
  ASSERT_NE(it, tree.end());
  tree.erase(it);
  auto it1 = tree.find(25);
  auto it2 = tree.find(27);
  auto it3 = tree.find(20);

  EXPECT_EQ(*it1, 25);
  EXPECT_EQ(*it2, 27);
  EXPECT_EQ(it3, tree.end());

  it = tree.find(25);
  ASSERT_NE(it, tree.end());
  tree.erase(it);
  it1 = tree.find(25);
  it2 = tree.find(27);
  EXPECT_EQ(it1, tree.end());
  EXPECT_EQ(*it2, 27);
}

TEST(RBTreeTest, DeleteRedLeaf) {
  set<int> tree;
  tree.insert(10);
  tree.insert(5);
  tree.insert(15);

  auto it = tree.find(15);
  ASSERT_NE(it, tree.end());
  tree.erase(it);
  it = tree.find(15);
  ASSERT_EQ(it, tree.end());
}

TEST(RBTreeTest, DeleteBlackLeaf) {
  set<int> tree;
  tree.insert(10);
  tree.insert(5);
  tree.insert(15);
  tree.insert(1);
  tree.insert(6);
  tree.insert(0);

  auto it = tree.find(0);
  ASSERT_NE(it, tree.end());
  tree.erase(it);
  it = tree.find(0);
  ASSERT_EQ(it, tree.end());
}

TEST(RBTreeTest, DeleteNodeWithOneRedChild) {
  set<int> tree;
  tree.insert(10);
  tree.insert(5);
  tree.insert(1);

  auto it = tree.find(5);
  ASSERT_NE(it, tree.end());
  tree.erase(it);
  auto it1 = tree.find(5);
  auto it2 = tree.find(1);
  ASSERT_EQ(it1, tree.end());
  ASSERT_NE(it2, tree.end());
}

TEST(RBTreeTest, DeleteNodeWithTwoChildren) {
  set<int> tree;
  tree.insert(20);
  tree.insert(10);
  tree.insert(30);
  tree.insert(25);
  tree.insert(35);

  auto it = tree.find(30);
  ASSERT_NE(it, tree.end());
  tree.erase(it);
  auto it1 = tree.find(30);
  auto it2 = tree.find(25);
  auto it3 = tree.find(35);
  ASSERT_EQ(it1, tree.end());
  ASSERT_NE(it2, tree.end());
  ASSERT_NE(it3, tree.end());
}

TEST(RBTreeTest, DeleteRootNode) {
  set<int> tree;
  tree.insert(10);
  tree.insert(5);
  tree.insert(15);

  auto it = tree.find(10);
  ASSERT_NE(it, tree.end());
  tree.erase(it);
  auto it1 = tree.find(10);
  ASSERT_EQ(it1, tree.end());
}

TEST(SetEraseTest, Case3_LeftBrotherLeftRedRightBlack) {
  s21::set<int> set;

  // Сконструируем вручную, чтобы получить нужную ситуацию:
  // Вставка специально для построения нужной конфигурации
  set.insert(10);
  set.insert(5);
  set.insert(15);
  set.insert(3);
  set.insert(7);
  set.insert(1);  // <-- Удалим его, чтобы current стал nullptr и был слева

  set.print();  // посмотреть конфигурацию

  // Теперь удалим 1 (левый чёрный лист)
  auto it = set.find(1);
  set.erase(it);

  set.print();  // должна сработать Case 3
}

TEST(SetEraseTest, EraseRoot) {
  s21::set<int> set;
  set.insert(10);
  set.insert(5);
  set.insert(15);

  auto it = set.find(10);  // корень
  ASSERT_NE(it, set.end());
  set.erase(it);

  ASSERT_EQ(set.size(), 2);
  ASSERT_EQ(set.find(10), set.end());
}

TEST(SETEraseTest, EraseNodeWithTwoChildren) {
  s21::set<int> set;
  set.insert(20);
  set.insert(10);
  set.insert(30);
  set.insert(5);
  set.insert(15);

  auto it = set.find(10);
  set.erase(it);

  ASSERT_EQ(set.size(), 4);
  ASSERT_EQ(set.find(10), set.end());
  ASSERT_NE(set.find(5), set.end());
  ASSERT_NE(set.find(15), set.end());
}

TEST(SetEraseTest, EraseLeaf) {
  s21::set<int> set;
  set.insert(10);
  set.insert(5);
  set.insert(15);

  auto it = set.find(5);  // лист
  set.erase(it);

  ASSERT_EQ(set.size(), 2);
  ASSERT_EQ(set.find(5), set.end());
}

TEST(SETEraseTest, EraseNodeWithOneChild) {
  s21::set<int> set;
  set.insert(10);
  set.insert(5);
  set.insert(3);  // 5 имеет одного потомка (3)

  auto it = set.find(5);
  set.erase(it);

  ASSERT_EQ(set.size(), 2);
  ASSERT_EQ(set.find(5), set.end());
  ASSERT_NE(set.find(3), set.end());
}

TEST(SetEraseTest, EraseFromBeginningToEnd) {
  s21::set<int> set;
  for (int i = 1; i <= 10; ++i) {
    set.insert(i);
  }

  for (int i = 1; i <= 10; ++i) {
    auto it = set.find(i);
    set.erase(it);
    ASSERT_EQ(set.find(i), set.end());
  }

  ASSERT_TRUE(set.empty());
}

TEST(SetEraseTest, EraseInReverseOrder) {
  s21::set<int> set;
  for (int i = 1; i <= 10; ++i) {
    set.insert(i);
  }

  for (int i = 10; i >= 1; --i) {
    auto it = set.find(i);
    set.erase(it);
    ASSERT_EQ(set.find(i), set.end());
  }

  ASSERT_TRUE(set.empty());
}

TEST(SetEraseTest, EraseComplexBalanceCase) {
  s21::set<int> set;
  // Вставим такие значения, чтобы при удалении были разные случаи балансировки
  std::vector<int> values = {10, 5, 15, 1, 6, 12, 17, 0, 2, 7};
  for (int val : values) set.insert(val);

  set.print();  // опционально

  // Удалим узел, у которого два потомка и это вызовет балансировку
  auto it = set.find(5);
  ASSERT_NE(it, set.end());
  set.erase(it);
  ASSERT_EQ(set.find(5), set.end());

  // Проверим, что дерево ещё корректно
  for (int val : values) {
    if (val != 5) {
      ASSERT_NE(set.find(val), set.end());
    }
  }
}

TEST(SetTest, InsertAscending) {
  s21::set<int> set;
  for (int i = 1; i <= 50; ++i) {
    set.insert(i);
    // set.print(); // можно включить для отладки
  }

  ASSERT_EQ(set.size(), 50);
  for (int i = 1; i <= 50; ++i) {
    ASSERT_NE(set.find(i), set.end());
  }
}

TEST(SetTest, InsertDescending) {
  s21::set<int> set;
  for (int i = 50; i >= 1; --i) {
    set.insert(i);
    // set.print(); // можно включить для отладки
  }

  ASSERT_EQ(set.size(), 50);
  for (int i = 1; i <= 50; ++i) {
    ASSERT_NE(set.find(i), set.end());
  }
}

TEST(SetInsertTes, RRRotate) {
  s21::set<int> s;
  s.insert(10);
  s.insert(20);
  s.insert(30);  // здесь должен сработать RR-поворот на узле 10

  // Проверяем, что элементы отсортированы (in-order traversal)
  std::vector<int> result(s.begin(), s.end());
  std::vector<int> expected = {10, 20, 30};

  EXPECT_EQ(result, expected);

  // Можно добавить вывод дерева, если есть метод print()
  s.print();
}

// 1
TEST(SetInsertTest, InsertRootOnly) {
  s21::set<int> s;
  s.insert(10);
  std::vector<int> result(s.begin(), s.end());
  EXPECT_EQ(result, std::vector<int>{10});
  s.print();
}

// 2
TEST(SetInsertTest, LLRotate) {
  s21::set<int> s;
  s.insert(30);
  s.insert(20);
  s.insert(10);  // LL-поворот на 30
  std::vector<int> result(s.begin(), s.end());
  std::vector<int> expected = {10, 20, 30};
  EXPECT_EQ(result, expected);
  s.print();
}

// 3
TEST(SetInsertTest, RRRotate) {
  s21::set<int> s;
  s.insert(30);
  s.insert(10);
  s.insert(20);  // RR-поворот на 10
  std::vector<int> result(s.begin(), s.end());
  std::vector<int> expected = {10, 20, 30};
  EXPECT_EQ(result, expected);
  s.print();
}

// 4
TEST(SetInsertTest, LRRotate) {
  s21::set<int> s;
  s.insert(10);
  s.insert(30);
  s.insert(20);  // LR-поворот на 30
  std::vector<int> result(s.begin(), s.end());
  std::vector<int> expected = {10, 20, 30};
  EXPECT_EQ(result, expected);
  s.print();
}

// 5
TEST(SetInsertTest, RLRotate) {
  s21::set<int> s;
  s.insert(10);
  s.insert(30);
  s.insert(20);  // RL-поворот на 10
  std::vector<int> result(s.begin(), s.end());
  std::vector<int> expected = {10, 20, 30};
  EXPECT_EQ(result, expected);
  s.print();
}

// 6
TEST(SetInsertTest, RecoloringUncleRed) {
  s21::set<int> s;
  s.insert(10);
  s.insert(5);
  s.insert(20);
  s.insert(1);
  s.insert(6);  // дядя (20) красный, будет перекраска
  std::vector<int> result(s.begin(), s.end());
  EXPECT_EQ(result, std::vector<int>({1, 5, 6, 10, 20}));
  s.print();
}

// 7
TEST(SetInsertTest, RecoloringWithRecursion) {
  s21::set<int> s;
  s.insert(10);
  s.insert(5);
  s.insert(20);
  s.insert(1);
  s.insert(6);
  s.insert(0);  // приведёт к перекраске деда (5), и пойдёт вверх к 10
  std::vector<int> result(s.begin(), s.end());
  EXPECT_EQ(result, std::vector<int>({0, 1, 5, 6, 10, 20}));
  s.print();
}

// 8
TEST(SetInsertTest, RLRotateAtRoot) {
  s21::set<int> s;
  s.insert(50);
  s.insert(70);
  s.insert(60);  // RL-поворот на корне
  std::vector<int> result(s.begin(), s.end());
  EXPECT_EQ(result, std::vector<int>({50, 60, 70}));
  s.print();
}

TEST(SetInsertTest, RLRotate_SunWithChildren) {
  s21::set<int> s;
  s.insert(10);
  s.insert(60);
  s.insert(30);
  s.insert(20);
  s.insert(40);
  s.insert(35);  // RL-поворот, sun = 30, есть 20 и 40
  std::vector<int> result(s.begin(), s.end());
  EXPECT_EQ(result, std::vector<int>({10, 20, 30, 35, 40, 60}));
  s.print();
}

TEST(SetInsertTest, RLRotate_SunHasLeftChildOnly) {
  s21::set<int> s;
  s.insert(50);
  s.insert(80);
  s.insert(60);
  s.insert(55);  // sun = 60, есть только левый
  std::vector<int> result(s.begin(), s.end());
  EXPECT_EQ(result, std::vector<int>({50, 55, 60, 80}));
  s.print();
}

TEST(SetInsertTest, RLRotate_SunHasRightChildOnly) {
  s21::set<int> s;
  s.insert(50);
  s.insert(80);
  s.insert(60);
  s.insert(65);  // sun = 60, есть только правый
  std::vector<int> result(s.begin(), s.end());
  EXPECT_EQ(result, std::vector<int>({50, 60, 65, 80}));
  s.print();
}

TEST(SetInsertTest, RLRotate_SunHasBothChildren) {
  s21::set<int> s;
  s.insert(50);
  s.insert(80);
  s.insert(60);
  s.insert(55);  // sun->left_
  s.insert(65);  // sun->right_
  std::vector<int> result(s.begin(), s.end());
  EXPECT_EQ(result, std::vector<int>({50, 55, 60, 65, 80}));
  s.print();
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(SetInsertTest, InsertManyFixedElements) {
  s21::set<int> s;

  // Вставляем элементы вручную через insert
  s.insert(50);
  s.insert(20);
  s.insert(70);
  s.insert(10);
  s.insert(30);
  s.insert(60);
  s.insert(80);
  s.insert(5);
  s.insert(15);
  s.insert(25);
  s.insert(35);
  s.insert(55);
  s.insert(65);
  s.insert(75);
  s.insert(85);
  s.print();
  // Собираем результат в вектор
  std::vector<int> result(s.begin(), s.end());

  // Ожидаемый вектор — элементы должны быть в отсортированном порядке
  std::vector<int> expected = {5,  10, 15, 20, 25, 30, 35, 50,
                               55, 60, 65, 70, 75, 80, 85};

  EXPECT_EQ(result, expected);
}

// Тест итератора с инкрементом в теле цикла
TEST(SetIteratorTest, IterateWithBodyIncrement) {
  s21::set<int> s;
  s.insert(1);
  s.insert(2);
  s.insert(3);

  auto it = s.begin();
  std::vector<int> values;
  while (it != s.end()) {
    values.push_back(*it);
    ++it;
  }
  EXPECT_EQ(values, std::vector<int>({1, 2, 3}));
}

// Тест конст итератора с инкрементом в теле цикла
TEST(SetIteratorTest, ConstIterateWithBodyIncrement) {
  const s21::set<int> s = [] {
    s21::set<int> temp;
    temp.insert(1);
    temp.insert(2);
    temp.insert(3);
    return temp;
  }();

  auto it = s.begin();
  std::vector<int> values;
  while (it != s.end()) {
    values.push_back(*it);
    ++it;
  }
  EXPECT_EQ(values, std::vector<int>({1, 2, 3}));
}

// Тест префиксного декремента итератора
TEST(SetIteratorTest, DecrementWorks) {
  s21::set<int> s;
  s.insert(10);
  s.insert(20);
  s.insert(30);

  auto it = s.end();
  --it;  // должен указывать на 30
  EXPECT_EQ(*it, 30);
  --it;  // 20
  EXPECT_EQ(*it, 20);
  --it;  // 10
  EXPECT_EQ(*it, 10);
}

// Исправленный тест: декремент begin() не выбрасывает исключение
TEST(SetIteratorTest, DecrementBeginNoThrow) {
  s21::set<int> s;
  s.insert(10);
  auto it = s.begin();

  // Проверяем, что декремент begin() не вызывает исключения
  EXPECT_NO_THROW({ --it; });

  // В зависимости от реализации итератора, итератор может оставаться на begin()
  // или перейти в end(). Проверим, что итератор валидный:
  // Например, можно проверить, что он не равен nullptr внутри итератора (если
  // есть доступ) Но в тестах проще проверить, что оператор* не кидает (если не
  // end)
  if (it != s.end()) {
    EXPECT_NO_THROW({
      volatile auto val = *it;  // просто разыменуем
    });
  }
}

// Тест оператора сравнения итераторов
TEST(SetIteratorTest, IteratorComparison) {
  s21::set<int> s;
  s.insert(5);
  s.insert(10);

  auto it1 = s.begin();
  auto it2 = s.begin();
  EXPECT_TRUE(it1 == it2);
  ++it2;
  EXPECT_TRUE(it1 != it2);
}

// Тест разыменования итератора на end() — должно бросать
TEST(SetIteratorTest, DereferenceEndThrows) {
  s21::set<int> s;
  s.insert(1);
  auto it = s.end();
  EXPECT_THROW(*it, std::out_of_range);
}

TEST(SetIteratorTest, BeginAndEnd) {
  set<int> s;
  s.insert(2);
  s.insert(1);
  s.insert(3);

  auto it = s.begin();
  EXPECT_EQ(*it, 1);  // минимальное значение

  auto end = s.end();
  EXPECT_NE(it, end);  // begin() != end()

  std::vector<int> result;
  while (it != end) {
    result.push_back(*it);
    ++it;
  }

  std::vector<int> expected = {1, 2, 3};
  EXPECT_EQ(result, expected);
}

TEST(SetIteratorTest, PrefixIncrement) {
  set<int> s;
  s.insert(10);
  s.insert(5);
  s.insert(15);

  auto it = s.begin();
  EXPECT_EQ(*it, 5);
  ++it;
  EXPECT_EQ(*it, 10);
  ++it;
  EXPECT_EQ(*it, 15);
  ++it;
  EXPECT_EQ(it, s.end());
}

TEST(SetIteratorTest, PrefixDecrementFromEnd) {
  set<int> s;
  s.insert(10);
  s.insert(5);
  s.insert(15);

  auto it = s.end();
  --it;
  EXPECT_EQ(*it, 15);
  --it;
  EXPECT_EQ(*it, 10);
  --it;
  EXPECT_EQ(*it, 5);
}

TEST(SetIteratorTest, IteratorDereferenceThrowsOnEnd) {
  set<int> s;
  auto it = s.end();

  EXPECT_THROW(*it, std::out_of_range);
}

TEST(SetConstIteratorTest, ConstIteration) {
  set<int> s;
  s.insert(8);
  s.insert(3);
  s.insert(10);

  const set<int>& const_s = s;

  std::vector<int> result;
  for (auto it = const_s.begin(); it != const_s.end(); ++it) {
    result.push_back(*it);
  }

  std::vector<int> expected = {3, 8, 10};
  EXPECT_EQ(result, expected);
}

TEST(SetConstIteratorTest, ConvertFromIterator) {
  set<int> s;
  s.insert(42);

  set<int>::iterator it = s.begin();
  set<int>::const_iterator cit(
      it);  // const_iterator(const iterator&) конструктор

  EXPECT_EQ(*cit, 42);
}

TEST(ConstIteratorTest, IncrementCoversLeftTraversalAndParentTraversal) {
  set<int> s = {20, 10, 30, 25, 35};  // 30 has left 25, right 35

  auto it = s.find(20);
  ++it;
  EXPECT_EQ(*it, 25);  // идёт вправо, потом влево

  it = s.find(35);
  ++it;
  EXPECT_EQ(it, s.end());  // переход к end
}

TEST(ConstIteratorTest, DecrementCoversLeftTraversalAndParentTraversal) {
  set<int> s = {20, 10, 30, 25, 35};

  auto it = s.end();  // current_ == nullptr
  --it;
  EXPECT_EQ(*it, 35);  // rightmost node

  --it;  // к 30
  EXPECT_EQ(*it, 30);

  --it;  // к 25 (влево от 30)
  EXPECT_EQ(*it, 25);
}

TEST(ConstIteratorTest, DereferenceThrowsWhenEnd) {
  set<int> s = {1, 2, 3};
  set<int>::const_iterator it = s.end();
  EXPECT_THROW(*it, std::out_of_range);
}

TEST(SetConstIteratorTest, ConstIteratorDecrement) {
  set<int> s;
  s.insert(1);
  s.insert(2);
  s.insert(3);

  const set<int>& cs = s;
  auto it = cs.end();
  --it;
  EXPECT_EQ(*it, 3);
  --it;
  EXPECT_EQ(*it, 2);
  --it;
  EXPECT_EQ(*it, 1);
}

TEST(SetConstructorTest, DefaultConstructor) {
  set<int> s;
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.size(), 0);
}

//----- Конструкторы  -----

TEST(SetConstructorTest, InitializerListConstructor) {
  set<int> s = {3, 1, 4};
  EXPECT_EQ(s.size(), 3);
  EXPECT_TRUE(s.contains(1));
  EXPECT_TRUE(s.contains(3));
  EXPECT_TRUE(s.contains(4));
}

TEST(SetConstructorTest, CopyConstructor) {
  set<int> original = {1, 2, 3};
  set<int> copy(original);
  EXPECT_EQ(copy.size(), 3);
  EXPECT_TRUE(copy.contains(2));
}

TEST(SetConstructorTest, MoveConstructor) {
  set<int> s1 = {10, 20};
  set<int> s2(std::move(s1));
  EXPECT_EQ(s2.size(), 2);
  EXPECT_TRUE(s2.contains(10));
  EXPECT_TRUE(s1.empty());
}

//---- оператор присваивания -----

TEST(SetOperatorTest, CopyAssignment) {
  set<int> a = {1, 2, 3};
  set<int> b;
  b = a;
  EXPECT_EQ(b.size(), 3);
  EXPECT_TRUE(b.contains(1));
}

TEST(SetOperatorTest, MoveAssignment) {
  set<int> a = {4, 5, 6};
  set<int> b;
  b = std::move(a);
  EXPECT_EQ(b.size(), 3);
  EXPECT_TRUE(b.contains(5));
  EXPECT_TRUE(a.empty());
}

//----- базовые методы ------

TEST(SetMethodTest, InsertAndContains) {
  set<int> s;
  s.insert(7);
  s.insert(3);
  EXPECT_TRUE(s.contains(7));
  EXPECT_TRUE(s.contains(3));
  EXPECT_FALSE(s.contains(9));
}

TEST(SetMethodTest, EraseOneElement) {
  set<int> s = {1, 2, 3};
  s.erase(s.find(2));
  EXPECT_FALSE(s.contains(2));
  EXPECT_EQ(s.size(), 2);
}

TEST(SetMethodTest, Find) {
  set<int> s = {10, 20, 30};
  auto it = s.find(20);
  EXPECT_NE(it, s.end());
  EXPECT_EQ(*it, 20);
}

TEST(SetMethodTest, EmptyAndSize) {
  set<int> s;
  EXPECT_TRUE(s.empty());
  s.insert(1);
  EXPECT_FALSE(s.empty());
  EXPECT_EQ(s.size(), 1);
}

TEST(SetMethodTest, Clear) {
  set<int> s = {1, 2, 3};
  s.clear();
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.size(), 0);
}

TEST(SetBalanceTest, InsertLeftLeftRotation) {
  set<int> s;
  s.insert(30);
  s.insert(20);
  s.insert(10);  // вызовет правый поворот (LL case)
  EXPECT_TRUE(s.contains(10));
  EXPECT_TRUE(s.contains(20));
  EXPECT_TRUE(s.contains(30));
}

TEST(SetBalanceTest, InsertRightRightRotation) {
  set<int> s;
  s.insert(10);
  s.insert(20);
  s.insert(30);  // вызовет левый поворот (RR case)
  EXPECT_TRUE(s.contains(10));
  EXPECT_TRUE(s.contains(20));
  EXPECT_TRUE(s.contains(30));
}

TEST(SetBalanceTest, InsertLeftRightRotation) {
  set<int> s;
  s.insert(30);
  s.insert(10);
  s.insert(20);  // LR case: сначала левый, потом правый поворот
  EXPECT_TRUE(s.contains(10));
  EXPECT_TRUE(s.contains(20));
  EXPECT_TRUE(s.contains(30));
}

TEST(SetBalanceTest, InsertRightLeftRotation) {
  set<int> s;
  s.insert(10);
  s.insert(30);
  s.insert(20);  // RL case: сначала правый, потом левый поворот
  EXPECT_TRUE(s.contains(10));
  EXPECT_TRUE(s.contains(20));
  EXPECT_TRUE(s.contains(30));
}

TEST(SetEraseTest, EraseLeafNode) {
  set<int> s = {10, 5, 15};
  s.erase(s.find(5));  // удаление листа
  EXPECT_FALSE(s.contains(5));
  EXPECT_TRUE(s.contains(10));
  EXPECT_TRUE(s.contains(15));
}

TEST(SetEraseTest, EraseNodeWithOneChild) {
  set<int> s;
  s.insert(10);
  s.insert(5);
  s.insert(2);  // создаёт левого потомка у 5

  s.erase(s.find(5));  // 5 имеет одного потомка — 2
  EXPECT_FALSE(s.contains(5));
  EXPECT_TRUE(s.contains(2));
  EXPECT_TRUE(s.contains(10));
}

TEST(SetEraseTest, EraseNodeWithTwoChildren) {
  set<int> s;
  s.insert(20);
  s.insert(10);
  s.insert(30);
  s.insert(25);
  s.insert(35);

  s.erase(s.find(30));  // 30 имеет двух детей: 25 и 35
  EXPECT_FALSE(s.contains(30));
  EXPECT_TRUE(s.contains(25));
  EXPECT_TRUE(s.contains(35));
  EXPECT_TRUE(s.contains(20));
}

//----- Проверка балансировки после удаления -----

TEST(SetBalanceTest, EraseCausesRebalanceLL) {
  set<int> s = {30, 20, 40, 10};
  s.erase(s.find(40));  // удаление вызовет LL дисбаланс
  EXPECT_TRUE(s.contains(10));
  EXPECT_TRUE(s.contains(20));
  EXPECT_TRUE(s.contains(30));
}

TEST(SetBalanceTest, EraseCausesRebalanceRR) {
  set<int> s = {10, 5, 20, 30};
  s.erase(s.find(5));  // RR дисбаланс
  EXPECT_TRUE(s.contains(10));
  EXPECT_TRUE(s.contains(20));
  EXPECT_TRUE(s.contains(30));
}

TEST(SetBalanceTest, EraseCausesRebalanceLR) {
  set<int> s = {50, 20, 70, 10, 30, 25};
  s.erase(s.find(70));  // LR дисбаланс после удаления
  EXPECT_TRUE(s.contains(25));
}

TEST(SetBalanceTest, EraseCausesRebalanceRL) {
  set<int> s = {10, 5, 30, 25, 40, 35};
  s.erase(s.find(5));  // RL дисбаланс после удаления
  EXPECT_TRUE(s.contains(35));
}

//----- Попытка удаления несуществующего элемента -----

TEST(SetEraseTest, EraseNonExistent) {
  set<int> s = {1, 2, 3};
  auto it = s.find(999);
  EXPECT_EQ(it, s.end());  // find вернёт end()
  // ничего не удаляем — просто проверка устойчивости
  EXPECT_NO_THROW(s.erase(it));
}

//----- Вставка уже существующего элемента -----

TEST(SetInsertTest, InsertDuplicate) {
  set<int> s = {1, 2, 3};
  size_t old_size = s.size();
  s.insert(2);                    // дубликат
  EXPECT_EQ(s.size(), old_size);  // размер не изменится
}

//-----  Методы swap, merge, max_size -----

TEST(SetMethodTest, Swap) {
  set<int> a = {1, 2};
  set<int> b = {3, 4, 5};
  a.swap(b);
  EXPECT_EQ(a.size(), 3);
  EXPECT_TRUE(a.contains(3));
  EXPECT_EQ(b.size(), 2);
  EXPECT_TRUE(b.contains(1));
}

TEST(SetMethodTest, Merge) {
  set<int> a = {1, 2};
  a.print();
  set<int> b = {2, 3, 4};
  b.print();
  a.merge(b);
  EXPECT_EQ(a.size(), 4);
  EXPECT_TRUE(a.contains(4));
  EXPECT_EQ(b.size(), 1);  // если merge "перевозит" узлы
}

TEST(SetMethodTest, MaxSize) {
  set<int> s;
  EXPECT_GT(s.max_size(), 1000);
}

TEST(SetBalanceAndEraseTest, InsertLLRotation) {
  set<int> s;
  s.insert(30);
  s.insert(20);
  s.insert(10);  // LL
  EXPECT_TRUE(s.contains(10));
  EXPECT_TRUE(s.contains(20));
  EXPECT_TRUE(s.contains(30));
}

TEST(SetBalanceAndEraseTest, InsertRRRotation) {
  set<int> s;
  s.insert(10);
  s.insert(20);
  s.insert(30);  // RR
  EXPECT_TRUE(s.contains(10));
  EXPECT_TRUE(s.contains(20));
  EXPECT_TRUE(s.contains(30));
}

TEST(SetBalanceAndEraseTest, InsertLRRotation) {
  set<int> s;
  s.insert(30);
  s.insert(10);
  s.insert(20);  // LR
  EXPECT_TRUE(s.contains(10));
  EXPECT_TRUE(s.contains(20));
  EXPECT_TRUE(s.contains(30));
}

TEST(SetBalanceAndEraseTest, InsertRLRotation) {
  set<int> s;
  s.insert(10);
  s.insert(30);
  s.insert(20);  // RL
  EXPECT_TRUE(s.contains(10));
  EXPECT_TRUE(s.contains(20));
  EXPECT_TRUE(s.contains(30));
}

TEST(SetEraseCaseTest, EraseLeafNode) {
  set<int> s = {50, 30, 70};
  s.erase(s.find(30));  // лист
  EXPECT_FALSE(s.contains(30));
}

TEST(SetEraseCaseTest, EraseNodeWithOneChildLeft) {
  set<int> s;
  s.insert(50);
  s.insert(30);
  s.insert(20);  // 30 с левым ребёнком
  s.erase(s.find(30));
  EXPECT_FALSE(s.contains(30));
  EXPECT_TRUE(s.contains(20));
}

TEST(SetEraseCaseTest, EraseNodeWithOneChildRight) {
  set<int> s;
  s.insert(50);
  s.insert(70);
  s.insert(90);  // 70 с правым ребёнком
  s.erase(s.find(70));
  EXPECT_FALSE(s.contains(70));
  EXPECT_TRUE(s.contains(90));
}

TEST(SetEraseCaseTest, EraseNodeWithTwoChildren) {
  set<int> s;
  s.insert(50);
  s.insert(30);
  s.insert(70);
  s.insert(60);
  s.insert(80);
  s.erase(s.find(70));  // два потомка
  EXPECT_FALSE(s.contains(70));
  EXPECT_TRUE(s.contains(60));
  EXPECT_TRUE(s.contains(80));
}

TEST(SetEraseCaseTest, EraseRootNodeWithTwoChildren) {
  set<int> s = {40, 20, 60, 10, 30, 50, 70};
  s.erase(s.find(40));  // корень с двумя потомками
  EXPECT_FALSE(s.contains(40));
  EXPECT_TRUE(s.contains(50));  // либо successor, либо predecessor
}

TEST(SetRBTreeEraseBalance, Case1_BrotherRed) {
  // Дерево, где брат удаляемого узла — красный
  set<int> s = {10, 5, 15, 2, 7, 12, 17};

  // Искусственно создаём ситуацию, где после удаления 2 брат (узел 7) — красный
  // Обычно нужно вручную добиться этого цвета (если дерево балансируется само —
  // подбираем carefully)
  s.erase(s.find(2));
  EXPECT_FALSE(s.contains(2));
}

TEST(SetRBTreeEraseBalance, Case2_BlackBrotherAndBlackChildren) {
  set<int> s = {20, 10, 30, 5, 15, 25, 35};
  // после удаления 5, брат и его дети чёрные — Case 2
  s.erase(s.find(5));
  EXPECT_FALSE(s.contains(5));
}

TEST(SetRBTreeEraseBalance, Case3_BlackBrotherAndRedLeftChild) {
  set<int> s;
  s.insert(50);
  s.insert(30);
  s.insert(70);
  s.insert(60);
  s.insert(80);
  s.insert(55);  // Левый ребёнок у брата (60) => case 3

  s.erase(s.find(80));  // вызовет RB Case 3
  EXPECT_FALSE(s.contains(80));
}

TEST(SetRBTreeEraseBalance, Case4_BlackBrotherAndRedRightChild) {
  set<int> s = {40, 20, 60,
                50, 70, 65};  // 60 имеет правого потомка 65 (красный)
  s.erase(s.find(20));  // Вызовет Case 4: брат чёрный, правый ребёнок красный
  EXPECT_FALSE(s.contains(20));
}

TEST(SetRBTreeEraseBalance, NullptrCasesSafeExit) {
  set<int> s;
  s.insert(1);
  s.erase(s.find(1));  // единственный элемент
  EXPECT_TRUE(
      s.empty());  // current = nullptr, parent = nullptr → безопасный выход
}

// 0. Удаление листа (без детей), который черный — вызывает rebalanceAfterErase
TEST(SetEraseCasesTest, RemoveBlackLeaf) {
  s21::set<int> s;
  s.insert(10);
  s.insert(5);
  s.insert(15);
  s.insert(1);  // лист, черный

  auto it = s.find(1);
  s.erase(it);

  EXPECT_FALSE(s.contains(1));
  EXPECT_EQ(s.size(), 3);
}

// 1. Удаление красного листа (балансировка не нужна)
TEST(SetEraseCasesTest, RemoveRedLeaf) {
  s21::set<int> s;
  s.insert(10);
  s.insert(5);
  s.insert(15);
  s.insert(6);  // красный лист

  auto it = s.find(6);
  s.erase(it);

  EXPECT_FALSE(s.contains(6));
  EXPECT_EQ(s.size(), 3);
}

// 2. Удаление узла с одним ребёнком (черный или красный)
TEST(SetEraseCasesTest, RemoveNodeWithOneChild) {
  s21::set<int> s;
  // Формируем дерево с узлом 5, у которого один ребёнок 6
  s.insert(10);
  s.insert(5);
  s.insert(15);
  s.insert(6);

  auto it = s.find(5);
  s.erase(it);

  EXPECT_FALSE(s.contains(5));
  EXPECT_TRUE(s.contains(6));
  EXPECT_EQ(s.size(), 3);
}

// 3. Удаление узла с двумя детьми (с заменой на минимальный элемент правого
// поддерева)
TEST(SetEraseCasesTest, RemoveNodeWithTwoChildren) {
  s21::set<int> s;
  s.insert(10);
  s.insert(5);
  s.insert(15);
  s.insert(12);
  s.insert(18);

  auto it = s.find(15);
  s.erase(it);

  EXPECT_FALSE(s.contains(15));
  EXPECT_TRUE(s.contains(12));
  EXPECT_TRUE(s.contains(18));
  EXPECT_EQ(s.size(), 4);
}

// 4. Удаление узла с красным братом (принудительно вызываем случай с
// перекраской и ротацией)
TEST(SetEraseCasesTest, RemoveNodeWithRedBrother) {
  s21::set<int> s;
  // Вставка элементов, формирующих ситуацию с красным братом
  s.insert(20);
  s.insert(10);
  s.insert(30);
  s.insert(5);
  s.insert(15);
  s.insert(25);
  s.insert(35);
  s.insert(27);  // чтобы брат (25) был красным

  auto it = s.find(5);
  s.erase(it);

  EXPECT_FALSE(s.contains(5));
  EXPECT_EQ(s.size(), 7);
}

// 5. Удаление с черным братом без красных детей — проверяем "Case 2" из
// rebalanceAfterErase
TEST(SetEraseCasesTest, RemoveNodeWithBlackBrotherNoRedChildren) {
  s21::set<int> s;
  s.insert(10);
  s.insert(5);
  s.insert(15);
  s.insert(3);
  s.insert(7);

  auto it = s.find(3);
  s.erase(it);

  EXPECT_FALSE(s.contains(3));
  EXPECT_EQ(s.size(), 4);
}

// 6. Удаление с черным братом с красным левым ребенком
TEST(SetEraseCasesTest, RemoveNodeWithBlackBrotherRedLeftChild) {
  s21::set<int> s;
  s.insert(20);
  s.insert(10);
  s.insert(30);
  s.insert(25);
  s.insert(27);  // левый красный ребенок у брата
  s.insert(35);

  auto it = s.find(35);
  s.erase(it);

  EXPECT_FALSE(s.contains(35));
  EXPECT_EQ(s.size(), 5);
}

// 7. Удаление с черным братом с красным правым ребенком
TEST(SetEraseCasesTest, RemoveNodeWithBlackBrotherRedRightChild) {
  s21::set<int> s;
  s.insert(20);
  s.insert(10);
  s.insert(30);
  s.insert(25);
  s.insert(28);  // правый красный ребенок у брата
  s.insert(35);

  auto it = s.find(35);
  s.erase(it);

  EXPECT_FALSE(s.contains(35));
  EXPECT_EQ(s.size(), 5);
}

// 8. Большое дерево — удаление нескольких элементов подряд с разными случаями
TEST(SetEraseCasesTest, RemoveMultipleElementsInBigTree) {
  s21::set<int> s;
  for (int i = 1; i <= 50; ++i) {
    s.insert(i);
    std::cout << s.size() << std::endl;
  }
  // Удаляем узлы с разной позицией: листы, с 1 и 2 детьми
  std::cout << "deleted----" << std::endl;

  s.erase(s.find(1));
  std::cout << s.size() << std::endl;
  s.erase(s.find(25));
  std::cout << s.size() << std::endl;
  s.erase(s.find(50));
  std::cout << s.size() << std::endl;
  s.erase(s.find(30));
  std::cout << s.size() << std::endl;
  s.erase(s.find(10));
  std::cout << s.size() << std::endl;

  EXPECT_EQ(s.size(), 45);
  EXPECT_FALSE(s.contains(1));
  EXPECT_FALSE(s.contains(25));
  EXPECT_FALSE(s.contains(50));
  EXPECT_FALSE(s.contains(30));
  EXPECT_FALSE(s.contains(10));
}

TEST(SetTest, LRRotate_GrandIsLeftChild) {
  s21::set<int> set;
  set.insert(20);
  set.insert(10);
  set.insert(30);
  set.insert(5);
  set.insert(15);
  set.insert(13);  // Здесь произойдет LR-ротация

  // Ожидаемая структура:
  //        20
  //       /  \
  //     13    30
  //    /  \
  //  10   15
  //  /
  // 5

  set.print();
}

TEST(SetTest, LRRotate_SunHasRightChild) {
  s21::set<int> set;
  set.insert(20);
  set.insert(10);
  set.insert(30);
  set.insert(5);
  set.insert(15);
  set.insert(13);
  set.insert(14);  // У sun (13) появился правый потомок

  // Ожидаемая структура (после ротации и перекрасок):
  //        20
  //       /  \
  //     13    30
  //    /  \
  //  10   15
  //  /     /
  // 5     14

  set.print();
}
