#include <gtest/gtest.h>
#include "s21_multiset.h"  // твой заголовок с multiset
#include <algorithm>  // std::sort
#include <random>     // std::shuffle
#include <vector>

using namespace s21;


TEST(MultisetInsertMany, InsertMultipleInts) {
    s21::multiset<int> ms;
    auto results = ms.insert_many(10, 20, 30);

    // Проверяем, что вставлено 3 элемента
    ASSERT_EQ(results.size(), 3);

    // Проверяем, что все вставки успешны (true в bool)
    for (const auto& p : results) {
        EXPECT_TRUE(p.second);
    }

    // Проверяем, что в контейнере теперь 3 элемента
    EXPECT_EQ(ms.size(), 3);
}

TEST(MultisetInsertMany, InsertSingleInt) {
    s21::multiset<int> ms;
    auto results = ms.insert_many(42);

    ASSERT_EQ(results.size(), 1);
    EXPECT_TRUE(results[0].second);
    EXPECT_EQ(ms.size(), 1);
}


// Эти тесты не компилируются, это ожидаемо — static_assert ловит ошибки

TEST(MultisetInsertMany, InsertNoArgs) {
    s21::multiset<int> ms;
    // Ошибка компиляции: sizeof...(args) == 0
    // auto results = ms.insert_many();
}

TEST(MultisetInsertMany, InsertWrongType) {
    s21::multiset<int> ms;
    // Ошибка компиляции: const char* не конвертируется в int
    // auto results = ms.insert_many(1, 2, "text");
}

TEST(RBTreeTest, NewTree) {
  multiset<int> tree;
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
  multiset<int> tree;
  tree.insert(1);
  tree.insert(2);
  auto it = tree.find(1);
  tree.erase(it);
  auto next = tree.begin();

  ASSERT_NE(next, tree.end());
}

TEST(RBTreeTest, EraseLastElementReturnsEnd1) {
  multiset<int> tree;
  tree.insert(1);
  auto it = tree.find(1);
  tree.erase(it);
  auto next = tree.begin();
  EXPECT_EQ(next, tree.end());
}

TEST(RBTreeTest, EraseLeafNode1) {
  multiset<int> tree;
  tree.insert(10);
  tree.insert(5);
  auto it = tree.find(5);
  tree.erase(it);
  it = tree.find(5);
  EXPECT_EQ(it, tree.end());
}

TEST(RBTreeTest, EraseNodeWithOneChild1) {
  multiset<int> tree;
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
  multiset<int> tree;
  tree.insert(10);
  tree.insert(5);
  tree.insert(2);
  auto it = tree.find(5);
  tree.erase(it);
  auto iter = tree.find(5);
  EXPECT_EQ(iter, tree.end());
}

TEST(RBTreeTest, EraseNodeWithTwoChildren1) {
  multiset<int> tree;
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
  multiset<int> tree;
  tree.insert(10);
  tree.erase(tree.find(10));
  auto it = tree.find(10);
  EXPECT_EQ(it, tree.end());
}

TEST(RBTreeTest, EraseSingleElement) {
  multiset<int> tree;
  auto it = tree.insert(42);
  EXPECT_EQ(tree.size(), 1);
  tree.erase(it);
  EXPECT_EQ(tree.size(), 0);
  EXPECT_TRUE(tree.empty());
  it = tree.find(42);
  EXPECT_EQ(it, tree.end());
}

TEST(RBTreeTest, EraseRootWithTwoChildren) {
  multiset<int> tree;
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
  multiset<int> tree;
  tree.insert(10);
  tree.insert(5);
  tree.erase(tree.find(5));
  auto it = tree.find(5);
  EXPECT_EQ(it, tree.end());
  EXPECT_EQ(tree.size(), 1);
}

TEST(RBTreeTest, EraseNodeWithOneChild) {
  multiset<int> tree;
  tree.insert(10);
  tree.insert(5);
  tree.erase(tree.find(10));
  EXPECT_EQ(tree.size(), 1);
  auto it = tree.find(10);
  EXPECT_EQ(it, tree.end());
}

TEST(RBTreeTest, EraseNodeWithTwoChildren_SuccessorIsRightChild) {
  multiset<int> tree;
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
  multiset<int> tree;
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
  multiset<int> tree;
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
  multiset<int> tree;
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
  multiset<int> tree;
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
  multiset<int> tree;
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
  multiset<int> tree;
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
  multiset<int> tree;
  tree.insert(10);
  tree.insert(5);
  tree.insert(15);

  auto it = tree.find(10);
  ASSERT_NE(it, tree.end());
  tree.erase(it);
  auto it1 = tree.find(10);
  ASSERT_EQ(it1, tree.end());
}


TEST(mSetTest, InsertAscending) {
  s21::multiset<int> set;
  for (int i = 1; i <= 50; ++i) {
    set.insert(i);
    // set.print(); // можно включить для отладки
  }

  ASSERT_EQ(set.size(), 50);
  for (int i = 1; i <= 50; ++i) {
    ASSERT_NE(set.find(i), set.end());
  }
}

TEST(mSetTest, InsertDescending) {
  s21::multiset<int> set;
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
  s21::multiset<int> s;
  s.insert(10);
  s.insert(20);
  s.insert(30);  // здесь должен сработать RR-поворот на узле 10

  // Проверяем, что элементы отсортированы (in-order traversal)
  std::vector<int> result(s.begin(), s.end());
  std::vector<int> expected = {10, 20, 30};

  EXPECT_EQ(result, expected);

  // Можно добавить вывод дерева, если есть метод print()
}

// 1
TEST(SetInsertTest, InsertRootOnly) {
  s21::multiset<int> s;
  s.insert(10);
  std::vector<int> result(s.begin(), s.end());
  EXPECT_EQ(result, std::vector<int>{10});
}

// 2
TEST(SetInsertTest, LLRotate) {
  s21::multiset<int> s;
  s.insert(30);
  s.insert(20);
  s.insert(10);  // LL-поворот на 30
  std::vector<int> result(s.begin(), s.end());
  std::vector<int> expected = {10, 20, 30};
  EXPECT_EQ(result, expected);
}

// 3
TEST(SetInsertTest, RRRotate) {
  s21::multiset<int> s;
  s.insert(30);
  s.insert(10);
  s.insert(20);  // RR-поворот на 10
  std::vector<int> result(s.begin(), s.end());
  std::vector<int> expected = {10, 20, 30};
  EXPECT_EQ(result, expected);
}

// 4
TEST(SetInsertTest, LRRotate) {
  s21::multiset<int> s;
  s.insert(10);
  s.insert(30);
  s.insert(20);  // LR-поворот на 30
  std::vector<int> result(s.begin(), s.end());
  std::vector<int> expected = {10, 20, 30};
  EXPECT_EQ(result, expected);
}

// 5
TEST(SetInsertTest, RLRotate) {
  s21::multiset<int> s;
  s.insert(10);
  s.insert(30);
  s.insert(20);  // RL-поворот на 10
  std::vector<int> result(s.begin(), s.end());
  std::vector<int> expected = {10, 20, 30};
  EXPECT_EQ(result, expected);
}

// 6
TEST(SetInsertTest, RecoloringUncleRed) {
  s21::multiset<int> s;
  s.insert(10);
  s.insert(5);
  s.insert(20);
  s.insert(1);
  s.insert(6);  // дядя (20) красный, будет перекраска
  std::vector<int> result(s.begin(), s.end());
  EXPECT_EQ(result, std::vector<int>({1, 5, 6, 10, 20}));
}

// 7
TEST(SetInsertTest, RecoloringWithRecursion) {
  s21::multiset<int> s;
  s.insert(10);
  s.insert(5);
  s.insert(20);
  s.insert(1);
  s.insert(6);
  s.insert(0);  // приведёт к перекраске деда (5), и пойдёт вверх к 10
  std::vector<int> result(s.begin(), s.end());
  EXPECT_EQ(result, std::vector<int>({0, 1, 5, 6, 10, 20}));
}

// 8
TEST(SetInsertTest, RLRotateAtRoot) {
  s21::multiset<int> s;
  s.insert(50);
  s.insert(70);
  s.insert(60);  // RL-поворот на корне
  std::vector<int> result(s.begin(), s.end());
  EXPECT_EQ(result, std::vector<int>({50, 60, 70}));
}

TEST(SetInsertTest, RLRotate_SunWithChildren) {
  s21::multiset<int> s;
  s.insert(10);
  s.insert(60);
  s.insert(30);
  s.insert(20);
  s.insert(40);
  s.insert(35);  // RL-поворот, sun = 30, есть 20 и 40
  std::vector<int> result(s.begin(), s.end());
  EXPECT_EQ(result, std::vector<int>({10, 20, 30, 35, 40, 60}));
}

TEST(SetInsertTest, RLRotate_SunHasLeftChildOnly) {
  s21::multiset<int> s;
  s.insert(50);
  s.insert(80);
  s.insert(60);
  s.insert(55);  // sun = 60, есть только левый
  std::vector<int> result(s.begin(), s.end());
  EXPECT_EQ(result, std::vector<int>({50, 55, 60, 80}));
}

TEST(SetInsertTest, RLRotate_SunHasRightChildOnly) {
  s21::multiset<int> s;
  s.insert(50);
  s.insert(80);
  s.insert(60);
  s.insert(65);  // sun = 60, есть только правый
  std::vector<int> result(s.begin(), s.end());
  EXPECT_EQ(result, std::vector<int>({50, 60, 65, 80}));
}

TEST(SetInsertTest, RLRotate_SunHasBothChildren) {
  s21::multiset<int> s;
  s.insert(50);
  s.insert(80);
  s.insert(60);
  s.insert(55);  // sun->left_
  s.insert(65);  // sun->right_
  std::vector<int> result(s.begin(), s.end());
  EXPECT_EQ(result, std::vector<int>({50, 55, 60, 65, 80}));
}

TEST(SetEraseTest, Case3_LeftBrotherLeftRedRightBlack) {
  s21::multiset<int> set;

  // Сконструируем вручную, чтобы получить нужную ситуацию:
  // Вставка специально для построения нужной конфигурации
  set.insert(10);
  set.insert(5);
  set.insert(15);
  set.insert(3);
  set.insert(7);
  set.insert(1);  // <-- Удалим его, чтобы current стал nullptr и был слева


  // Теперь удалим 1 (левый чёрный лист)
  auto it = set.find(1);
  set.erase(it);

}

TEST(SetEraseTest, EraseRoot) {
  s21::multiset<int> set;
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
  s21::multiset<int> set;
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
  s21::multiset<int> set;
  set.insert(10);
  set.insert(5);
  set.insert(15);

  auto it = set.find(5);  // лист
  set.erase(it);

  ASSERT_EQ(set.size(), 2);
  ASSERT_EQ(set.find(5), set.end());
}

TEST(SETEraseTest, EraseNodeWithOneChild) {
  s21::multiset<int> set;
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
  s21::multiset<int> set;
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
  s21::multiset<int> set;
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
  s21::multiset<int> set;
  // Вставим такие значения, чтобы при удалении были разные случаи балансировки
  std::vector<int> values = {10, 5, 15, 1, 6, 12, 17, 0, 2, 7};
  for (int val : values) set.insert(val);


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

TEST(SetIteratorTest, IteratorDereferenceThrowsOnEnd) {
  multiset<int> s;
  auto it = s.end();

  EXPECT_THROW(*it, std::out_of_range);
}

TEST(SetConstIteratorTest, ConstIteration) {
  multiset<int> s;
  s.insert(8);
  s.insert(3);
  s.insert(10);

  const multiset<int>& const_s = s;

  std::vector<int> result;
  for (auto it = const_s.begin(); it != const_s.end(); ++it) {
    result.push_back(*it);
  }

  std::vector<int> expected = {3, 8, 10};
  EXPECT_EQ(result, expected);
}

TEST(ConstIteratorTest, IncrementCoversLeftTraversalAndParentTraversal) {
  multiset<int> s = {20, 10, 30, 25, 35};  // 30 has left 25, right 35

  auto it = s.find(20);
  ++it;
  EXPECT_EQ(*it, 25);  // идёт вправо, потом влево

  it = s.find(35);
  ++it;
  EXPECT_EQ(it, s.end());  // переход к end
}



TEST(ConstIteratorTest, DereferenceThrowsWhenEnd) {
  multiset<int> s = {1, 2, 3};
  multiset<int>::const_iterator it = s.end();
  EXPECT_THROW(*it, std::out_of_range);
}

TEST(SetConstructorTest, MoveConstructor) {
  multiset<int> s1 = {10, 20};
  multiset<int> s2(std::move(s1));
  EXPECT_EQ(s2.size(), 2);
  EXPECT_TRUE(s2.contains(10));
  EXPECT_TRUE(s1.empty());
}

TEST(SetOperatorTest, CopyAssignment) {
  multiset<int> a = {1, 2, 3};
  multiset<int> b;
  b = a;
  EXPECT_EQ(b.size(), 3);
  EXPECT_TRUE(b.contains(1));
}

TEST(SetOperatorTest, MoveAssignment) {
  multiset<int> a = {4, 5, 6};
  multiset<int> b;
  b = std::move(a);
  EXPECT_EQ(b.size(), 3);
  EXPECT_TRUE(b.contains(5));
  EXPECT_TRUE(a.empty());
}

TEST(SetBalanceTest, EraseCausesRebalanceLR) {
  multiset<int> s = {50, 20, 70, 10, 30, 25};
  s.erase(s.find(70));  // LR дисбаланс после удаления
  EXPECT_TRUE(s.contains(25));
}

TEST(SetBalanceAndEraseTest, InsertLRRotation) {
  multiset<int> s;
  s.insert(30);
  s.insert(10);
  s.insert(20);  // LR
  EXPECT_TRUE(s.contains(10));
  EXPECT_TRUE(s.contains(20));
  EXPECT_TRUE(s.contains(30));
}

TEST(SetBalanceAndEraseTest, InsertRLRotation) {
  multiset<int> s;
  s.insert(10);
  s.insert(30);
  s.insert(20);  // RL
  EXPECT_TRUE(s.contains(10));
  EXPECT_TRUE(s.contains(20));
  EXPECT_TRUE(s.contains(30));
}

TEST(SetRBTreeEraseBalance, Case3_BlackBrotherAndRedLeftChild) {
  multiset<int> s;
  s.insert(50);
  s.insert(30);
  s.insert(70);
  s.insert(60);
  s.insert(80);
  s.insert(55);  // Левый ребёнок у брата (60) => case 3

  s.erase(s.find(80));  // вызовет RB Case 3
  EXPECT_FALSE(s.contains(80));
}

TEST(SetEraseCasesTest, RemoveNodeWithTwoChildren) {
  s21::multiset<int> s;
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

// 8. Большое дерево — удаление нескольких элементов подряд с разными случаями
TEST(SetEraseCasesTest, RemoveMultipleElementsInBigTree) {
  s21::multiset<int> s;
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
  s21::multiset<int> set;
  set.insert(20);
  set.insert(10);
  set.insert(30);
  set.insert(5);
  set.insert(15);
  set.insert(13);  // Здесь произойдет LR-ротация
}

TEST(SetTest, LRRotate_SunHasRightChild) {
  s21::multiset<int> set;
  set.insert(20);
  set.insert(10);
  set.insert(30);
  set.insert(5);
  set.insert(15);
  set.insert(13);
  set.insert(14);  // У sun (13) появился правый потомок

}

TEST(MultiSetTest, InsertAndFindMultiple) {
    multiset<int> ms;
    ms.insert(10);
    ms.insert(10);
    ms.insert(20);
    ms.insert(20);
    ms.insert(20);
    ms.insert(5);

    // Проверка размера
    EXPECT_EQ(ms.size(), 6);

    // Проверяем, что элементы есть
    EXPECT_NE(ms.find(10), ms.end());
    EXPECT_NE(ms.find(20), ms.end());
    EXPECT_NE(ms.find(5), ms.end());

    // Проверяем, что элемент которого нет — возвращает end()
    EXPECT_EQ(ms.find(100), ms.end());
}

TEST(MultiSetTest, EqualRangeReturnsCorrectRange) {
    multiset<int> ms;
    ms.insert(10);
    ms.insert(10);
    ms.insert(10);
    ms.insert(20);
    ms.insert(30);

    auto range = ms.equal_range(10);
    int count = 0;
    for (auto it = range.first; it != range.second; ++it) {
        EXPECT_EQ(*it, 10);
        count++;
    }
    EXPECT_EQ(count, 3);  // ровно 3 одинаковых элемента 10

    // Проверка для элемента, которого нет
    auto empty_range = ms.equal_range(100);
    EXPECT_EQ(empty_range.first, empty_range.second);
}

TEST(MultiSetTest, LowerBoundAndUpperBound) {
    multiset<int> ms;
    ms.insert(10);
    ms.insert(15);
    ms.insert(20);
    ms.insert(25);

    auto lb = ms.lower_bound(15);
    EXPECT_NE(lb, ms.end());
    EXPECT_EQ(*lb, 15);

    auto ub = ms.upper_bound(15);
    EXPECT_NE(ub, ms.end());
    EXPECT_EQ(*ub, 20);

    // lower_bound для отсутствующего элемента
    auto lb2 = ms.lower_bound(17);
    EXPECT_NE(lb2, ms.end());
    EXPECT_EQ(*lb2, 20);

    // upper_bound для элемента больше максимального
    auto ub2 = ms.upper_bound(30);
    EXPECT_EQ(ub2, ms.end());
}

TEST(MultiSetTest, EraseSingleElement) {
    multiset<int> ms;
    ms.insert(10);
    ms.insert(20);
    ms.insert(30);

    auto it = ms.find(20);
    ASSERT_NE(it, ms.end());
    ms.erase(it);

    // Элемент 20 должен быть удалён
    EXPECT_EQ(ms.find(20), ms.end());
    EXPECT_EQ(ms.size(), 2);
}

TEST(MultiSetTest, EraseOneOfMultipleEqualElements) {
    multiset<int> ms;
    ms.insert(10);
    ms.insert(10);
    ms.insert(10);

    auto it = ms.find(10);
    ASSERT_NE(it, ms.end());
    ms.erase(it);  // удаляем один из трёх

    auto range = ms.equal_range(10);
    int count = 0;
    for (auto i = range.first; i != range.second; ++i) count++;
    EXPECT_EQ(count, 2);
    EXPECT_EQ(ms.size(), 2);
}

TEST(MultiSetTest, EraseAllElements) {
    multiset<int> ms;
    ms.insert(1);
    ms.insert(2);
    ms.insert(3);

    while (!ms.empty()) {
        ms.erase(ms.begin());
    }

    EXPECT_TRUE(ms.empty());
    EXPECT_EQ(ms.size(), 0);
}


using s21::multiset;

TEST(MultisetEraseTest, EraseSingleElement) {
  multiset<int> ms;
  ms.insert(10);
  auto it = ms.find(10);
  it = ms.erase(it);
  EXPECT_EQ(ms.size(), 0);
  EXPECT_EQ(ms.find(10), ms.end());
  EXPECT_EQ(it, ms.end());
}

TEST(MultisetEraseTest, EraseFirstOfDuplicates) {
  multiset<int> ms;
  ms.insert(10);
  ms.insert(10);
  auto it = ms.find(10);
  it = ms.erase(it);

  // Должен остаться один элемент 10
  EXPECT_EQ(ms.size(), 1);
  EXPECT_NE(ms.find(10), ms.end());
  EXPECT_EQ(*ms.find(10), 10);
}

TEST(MultisetEraseTest, EraseAllDuplicatesInLoop) {
  multiset<int> ms;
  ms.insert(10);
  ms.insert(10);
  ms.insert(10);

  while (true) {
    auto it = ms.find(10);
    if (it == ms.end()) break;
    ms.erase(it);
  }

  //EXPECT_EQ(ms.count(10), 0);
  EXPECT_EQ(ms.size(), 0);
}

TEST(MultisetEraseTest, EraseReturnsNextIterator) {
  multiset<int> ms;
  ms.insert(5);
  ms.insert(10);
  ms.insert(15);

  auto it = ms.find(10);
  auto next = ms.erase(it);

  EXPECT_EQ(*next, 15);
  //EXPECT_EQ(ms.count(10), 0);
}

TEST(MultisetEraseTest, EraseLastElementReturnsEnd) {
  multiset<int> ms;
  ms.insert(10);
  auto it = ms.find(10);
  auto result = ms.erase(it);

  EXPECT_EQ(result, ms.end());
  EXPECT_TRUE(ms.empty());
}

TEST(MultisetEraseTest, EraseFromEmpty) {
  multiset<int> ms;
  auto it = ms.end();  // итератор на конец
  auto result = ms.erase(it);
  EXPECT_EQ(result, it);  // должен вернуть тот же
}


using s21::multiset;

TEST(MultisetCountEraseTest, CountEmpty) {
  multiset<int> ms;
  EXPECT_EQ(ms.count(10), 0);
}

TEST(MultisetCountEraseTest, CountSingleElement) {
  multiset<int> ms;
  ms.insert(42);
  EXPECT_EQ(ms.count(42), 1);
}

TEST(MultisetCountEraseTest, CountMultipleElements) {
  multiset<int> ms;
  ms.insert(10);
  ms.insert(10);
  ms.insert(10);
  EXPECT_EQ(ms.count(10), 3);
}

TEST(MultisetCountEraseTest, EraseSingleExistingElement) {
  multiset<int> ms;
  ms.insert(5);
  ms.insert(5);
  ms.insert(6);
  EXPECT_EQ(ms.erase(5), 2);  // удалены оба
  EXPECT_EQ(ms.count(5), 0);
  EXPECT_EQ(ms.count(6), 1);
}

TEST(MultisetCountEraseTest, EraseNonExistentElement) {
  multiset<int> ms;
  ms.insert(1);
  ms.insert(2);
  EXPECT_EQ(ms.erase(99), 0);
  EXPECT_EQ(ms.count(1), 1);
  EXPECT_EQ(ms.count(2), 1);
}

TEST(MultisetCountEraseTest, EraseAllCopies) {
  multiset<int> ms;
  for (int i = 0; i < 5; ++i) {
    ms.insert(100);
  }
  EXPECT_EQ(ms.count(100), 5);
  EXPECT_EQ(ms.erase(100), 5);
  EXPECT_EQ(ms.count(100), 0);
  EXPECT_TRUE(ms.empty());
}

TEST(MultisetCountEraseTest, MixedValues) {
  multiset<int> ms;
  ms.insert(3);
  ms.insert(3);
  ms.insert(3);
  ms.insert(4);
  ms.insert(5);

  EXPECT_EQ(ms.count(3), 3);
  EXPECT_EQ(ms.count(4), 1);
  EXPECT_EQ(ms.count(5), 1);

  EXPECT_EQ(ms.erase(3), 3);
  EXPECT_EQ(ms.count(3), 0);
  EXPECT_EQ(ms.count(4), 1);
}

TEST(MultisetStressTest, InsertAndCountUpTo100) {
  multiset<int> ms;
  for (int i = 0; i < 100; ++i) {
    ms.insert(42);
  }

  EXPECT_EQ(ms.count(42), 100);
}



TEST(MultisetBoundTest, BoundsAndRange) {
  multiset<int> ms = {10, 20, 20, 20, 30, 40, 40, 50};

  auto lb = ms.lower_bound(20);
  auto ub = ms.upper_bound(20);
  auto range = ms.equal_range(20);

  EXPECT_EQ(*lb, 20);
  EXPECT_EQ(*range.first, 20);
  EXPECT_EQ(*range.second, 30);
  EXPECT_EQ(*ub, 30);
  EXPECT_EQ(ms.count(20), 3);
}

TEST(MultisetEraseTest, EraseAllByValue) {
  multiset<int> ms = {5, 5, 5, 7, 7, 9};

  EXPECT_EQ(ms.erase(5), 3);
  EXPECT_EQ(ms.count(5), 0);
  EXPECT_EQ(ms.size(), 3);

  EXPECT_EQ(ms.erase(7), 2);
  EXPECT_EQ(ms.count(7), 0);
  EXPECT_EQ(ms.erase(42), 0);
}

TEST(MultisetEraseTest, EraseByIterator) {
  multiset<int> ms = {1, 2, 2, 3, 4};

  auto it = ms.find(2);
  auto next = ms.erase(it);

  EXPECT_EQ(*next, 2);           // ещё одна 2 осталась
  EXPECT_EQ(ms.count(2), 1);

  next = ms.erase(ms.find(2));
  EXPECT_EQ(ms.count(2), 0);
}

TEST(MultisetEdgeCaseTest, InsertExtremeValues) {
  multiset<int> ms;
  ms.insert(INT_MIN);
  ms.insert(INT_MAX);
  ms.insert(INT_MAX);
  ms.insert(INT_MIN);

  EXPECT_EQ(ms.count(INT_MIN), 2);
  EXPECT_EQ(ms.count(INT_MAX), 2);
  EXPECT_EQ(ms.size(), 4);

  auto lb = ms.lower_bound(INT_MIN);
  auto ub = ms.upper_bound(INT_MAX);
  EXPECT_EQ(*lb, INT_MIN);
  EXPECT_EQ(ub, ms.end());
}




// Проверка count и contains
TEST(MultiSetTest, CountAndContains) {
    multiset<int> ms;
    ms.insert(5);
    ms.insert(5);
    ms.insert(10);

    EXPECT_EQ(ms.count(5), 2);
    EXPECT_EQ(ms.count(10), 1);
    EXPECT_EQ(ms.count(100), 0);

    EXPECT_TRUE(ms.contains(5));
    EXPECT_TRUE(ms.contains(10));
    EXPECT_FALSE(ms.contains(999));
}

// Проверка clear
TEST(MultiSetTest, ClearTest) {
    multiset<int> ms;
    ms.insert(1);
    ms.insert(2);
    ms.insert(3);

    ms.clear();
    EXPECT_TRUE(ms.empty());
    EXPECT_EQ(ms.size(), 0);

    // Повторный insert после очистки
    ms.insert(42);
    EXPECT_FALSE(ms.empty());
    EXPECT_EQ(ms.size(), 1);
}

// Проверка swap
TEST(MultiSetTest, SwapTest) {
    multiset<int> ms1;
    multiset<int> ms2;

    ms1.insert(1);
    ms1.insert(2);

    ms2.insert(10);
    ms2.insert(20);
    ms2.insert(30);

    ms1.swap(ms2);

    EXPECT_EQ(ms1.count(10), 1);
    EXPECT_EQ(ms1.size(), 3);

    EXPECT_EQ(ms2.count(1), 1);
    EXPECT_EQ(ms2.size(), 2);
}

// Проверка merge
TEST(MultiSetTest, MergeTest) {
    multiset<int> a;
    multiset<int> b;

    a.insert(1);
    a.insert(2);
    b.insert(2);
    b.insert(3);

    a.merge(b);

    EXPECT_EQ(a.count(2), 2);
    EXPECT_EQ(a.count(3), 1);
    EXPECT_EQ(b.size(), 0);  // b должен быть пуст после merge
}

// Проверка итераторов (begin, end)
TEST(MultiSetTest, IteratorTest) {
    multiset<int> ms;
    ms.insert(10);
    ms.insert(5);
    ms.insert(20);

    std::vector<int> values;
    for (auto it = ms.begin(); it != ms.end(); ++it) {
        values.push_back(*it);
    }

    // multiset хранит элементы по возрастанию
    std::vector<int> expected = {5, 10, 20};
    EXPECT_EQ(values, expected);
}

// Проверка const итераторов
TEST(MultiSetTest, ConstIteratorTest) {
    multiset<int> ms;
    ms.insert(10);
    ms.insert(20);

    const multiset<int>& const_ms = ms;
    int sum = 0;
    for (auto it = const_ms.begin(); it != const_ms.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 30);
}

// Проверка size и empty
TEST(MultiSetTest, SizeAndEmpty) {
    multiset<int> ms;
    EXPECT_TRUE(ms.empty());
    EXPECT_EQ(ms.size(), 0);

    ms.insert(1);
    ms.insert(2);
    EXPECT_FALSE(ms.empty());
    EXPECT_EQ(ms.size(), 2);
}

// Проверка вставки большого количества элементов
TEST(MultiSetTest, MassInsert) {
    multiset<int> ms;
    for (int i = 0; i < 1000; ++i) {
        ms.insert(i % 10);  // дубликаты
    }
    EXPECT_EQ(ms.size(), 1000);
    EXPECT_EQ(ms.count(0), 100);  // 100 элементов с значением 0
}

// Проверка вставки через итераторы (если реализовано)
TEST(MultiSetTest, InsertRangeIfSupported) {
    multiset<int> ms;
    std::vector<int> vec = {1, 2, 2, 3, 3, 3};
    for (int val : vec)
        ms.insert(val);

    EXPECT_EQ(ms.count(1), 1);
    EXPECT_EQ(ms.count(2), 2);
    EXPECT_EQ(ms.count(3), 3);
}

// Проверка поведения upper_bound и lower_bound на пустом контейнере
TEST(MultiSetTest, BoundsOnEmptyContainer) {
    multiset<int> ms;
    EXPECT_EQ(ms.lower_bound(5), ms.end());
    EXPECT_EQ(ms.upper_bound(5), ms.end());
}

// Проверка equal_range на уникальном значении
TEST(MultiSetTest, EqualRangeSingle) {
    multiset<int> ms;
    ms.insert(7);
    auto range = ms.equal_range(7);
    EXPECT_NE(range.first, ms.end());
    EXPECT_EQ(range.second, ms.end());
    EXPECT_EQ(std::distance(range.first, range.second), 1);
}

TEST(SetTest, InsertAscending) {
  s21::multiset<int> set;
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
  s21::multiset<int> set;
  for (int i = 50; i >= 1; --i) {
    set.insert(i);
    // set.print(); // можно включить для отладки
  }

  ASSERT_EQ(set.size(), 50);
  for (int i = 1; i <= 50; ++i) {
    ASSERT_NE(set.find(i), set.end());
  }
}