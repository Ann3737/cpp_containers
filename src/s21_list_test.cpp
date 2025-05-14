#include <gtest/gtest.h>
#include "s21_list.cpp"  // Подключаем сам класс

TEST(ListTest, PushBackWorks) {
  s21::List<int> list;
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);

  testing::internal::CaptureStdout();
  list.print();  // должно вывести: 10 20 30
  std::string output = testing::internal::GetCapturedStdout();

  EXPECT_EQ(output, "10 20 30 \n");
}

TEST(ListTest, PushFrontWorks) {
  s21::List<int> list;
  list.push_front(10);
  list.push_front(20);
  list.push_front(30);

  testing::internal::CaptureStdout();
  list.print();  // должно вывести: 30 20 10
  std::string output = testing::internal::GetCapturedStdout();

  EXPECT_EQ(output, "30 20 10 \n");
}

TEST(ListTest, PopBackWorks) {
  s21::List<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.pop_back();  // должно удалить 3

  testing::internal::CaptureStdout();
  list.print();  // ожидаем: 1 2
  std::string output = testing::internal::GetCapturedStdout();

  EXPECT_EQ(output, "1 2 \n");
}

TEST(ListTest, PopFrontWorks) {
  s21::List<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.pop_front();  // должно удалить 1

  testing::internal::CaptureStdout();
  list.print();  // ожидаем: 2 3
  std::string output = testing::internal::GetCapturedStdout();

  EXPECT_EQ(output, "2 3 \n");
}

TEST(ListTest, PopBackSingleElement) {
  s21::List<int> list;
  list.push_back(42);
  list.pop_back();  // удаляем единственный элемент

  testing::internal::CaptureStdout();
  list.print();  // ожидаем пустую строку
  std::string output = testing::internal::GetCapturedStdout();

  EXPECT_EQ(output, "\n");
}

TEST(ListTest, PopFrontSingleElement) {
  s21::List<int> list;
  list.push_front(99);
  list.pop_front();  // удаляем единственный элемент

  testing::internal::CaptureStdout();
  list.print();  // ожидаем пустую строку
  std::string output = testing::internal::GetCapturedStdout();

  EXPECT_EQ(output, "\n");
}

TEST(ListTest, BeginEndEmptyList) {
  s21::List<int> list;
  EXPECT_EQ(list.begin(), list.end());
}

TEST(ListTest, BeginPointsToFirstElement) {
  s21::List<int> list;
  list.push_back(42);
  list.push_back(77);

  auto it = list.begin();
  EXPECT_EQ(*it, 42);
}

TEST(ListTest, RangeBasedIterationWorks) {
  s21::List<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);

  int expected = 1;
  for (auto it = list.begin(); it != list.end(); ++it) {
    EXPECT_EQ(*it, expected);
    ++expected;
  }
}

TEST(ListTest, FrontReturnsReferenceToFirstElement) {
  s21::List<int> lst = {10, 20, 30};
  int& ref = lst.front();
  ref = 100;
  EXPECT_EQ(lst.front(), 100);  // Проверка, что это действительно ссылка
}

TEST(ListTest, BackReturnsReferenceToLastElement) {
  s21::List<int> lst = {5, 15, 25};
  int& ref = lst.back();
  ref = 99;
  EXPECT_EQ(lst.back(), 99);  // Проверка, что это действительно ссылка
}

TEST(ListTest, FrontThrowsOnEmptyList) {
  s21::List<int> lst;
  EXPECT_THROW(lst.front(), std::out_of_range);
}

TEST(ListTest, BackThrowsOnEmptyList) {
  s21::List<int> lst;
  EXPECT_THROW(lst.back(), std::out_of_range);
}

TEST(ListTest, MaxSize) {
  s21::List<int> list;
  auto max_size = list.max_size();

  // Просто проверяем, что max_size больше нуля и не меньше текущего размера
  EXPECT_GT(max_size, 0u);
  EXPECT_GE(max_size, list.size());
}

TEST(ListTest, ConstructorWithSizeCreatesDefaultElements) {
  const size_t n = 5;
  s21::List<int> list(n);

  // Проверяем размер
  EXPECT_EQ(list.size(), n);

  // Проверяем, что каждый элемент равен 0 (по умолчанию для int)
  for (auto value : list) {
    EXPECT_EQ(value, 0);
  }
}

TEST(ListTest, ConstructorWithSizeStringDefaultValues) {
  s21::List<std::string> list(2);
  EXPECT_EQ(list.size(), 2u);
  for (const auto& val : list) {
    EXPECT_EQ(val, "");
  }
}

TEST(ListTest, ConstructorWithSizeDoubleDefaultValues) {
  s21::List<double> list(4);
  EXPECT_EQ(list.size(), 4u);
  for (auto val : list) {
    EXPECT_EQ(val, 0.0);
  }
}

TEST(ListTest, InsertIntoEmptyList) {
  s21::List<int> list;
  auto it = list.begin();
  list.insert(it, 42);

  testing::internal::CaptureStdout();
  list.print();  // ожидаем: 42
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "42 \n");
}

TEST(ListTest, InsertAtBeginning) {
  s21::List<int> list;
  list.push_back(2);
  list.push_back(3);
  auto it = list.begin();  // позиция перед 2
  list.insert(it, 1);

  testing::internal::CaptureStdout();
  list.print();  // ожидаем: 1 2 3
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "1 2 3 \n");
}

TEST(ListTest, InsertInMiddle) {
  s21::List<int> list;
  list.push_back(1);
  list.push_back(3);
  auto it = ++list.begin();  // позиция перед 3
  list.insert(it, 2);

  testing::internal::CaptureStdout();
  list.print();  // ожидаем: 1 2 3
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "1 2 3 \n");
}

TEST(ListTest, InsertAtEnd) {
  s21::List<int> list;
  list.push_back(1);
  list.push_back(2);
  auto it = list.end();  // вставка перед end() == в конец
  list.insert(it, 3);

  testing::internal::CaptureStdout();
  list.print();  // ожидаем: 1 2 3
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "1 2 3 \n");
}

TEST(ListTest, EraseFirstElement) {
  s21::List<int> list;
  list.push_back(10);
  list.push_back(20);

  auto it = list.begin();  // итератор на 10
  list.erase(it);

  testing::internal::CaptureStdout();
  list.print();  // ожидаем: 20
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "20 \n");
}

TEST(ListTest, EraseMiddleElement) {
  s21::List<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);

  auto it = ++list.begin();  // итератор на 2
  list.erase(it);

  testing::internal::CaptureStdout();
  list.print();  // ожидаем: 1 3
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "1 3 \n");
}

TEST(ListTest, EraseLastElement) {
  s21::List<int> list;
  list.push_back(5);
  list.push_back(15);
  list.push_back(25);

  auto it = list.end();
  --it;  // итератор на последний элемент (25)
  list.erase(it);

  testing::internal::CaptureStdout();
  list.print();  // ожидаем: 5 15
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "5 15 \n");
}

TEST(ListTest, EraseSingleElement) {
  s21::List<int> list;
  list.push_back(100);

  auto it = list.begin();
  list.erase(it);

  EXPECT_EQ(list.size(), 0u);

  testing::internal::CaptureStdout();
  list.print();  // ожидаем: ничего
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "\n");
}

TEST(ListTest, EraseOnEmptyListDoesNothing) {
  s21::List<int> list;
  auto it = list.begin();  // может быть == list.end()
  list.erase(it);  // не должно быть падения

  EXPECT_EQ(list.size(), 0u);
}

TEST(ListTest, IteratorDecrementFromMiddle) {
  s21::List<int> lst;
  lst.push_back(10);
  lst.push_back(20);
  lst.push_back(30);

  auto it = lst.begin();
  ++it;  // теперь указывает на 20
  --it;  // должен вернуться на 10

  EXPECT_EQ(*it, 10);
}

// Тест 1: оба списка пустые
TEST(ListSwapTest, BothListsEmpty) {
  s21::List<int> list1, list2;
    
  list1.swap(list2);
    
  // Проверяем, что оба списка остаются пустыми
  EXPECT_TRUE(list1.empty());
  EXPECT_TRUE(list2.empty());
}

// Тест 2: один список пустой
TEST(ListSwapTest, OneListEmpty) {
  s21::List<int> list1;
  s21::List<int> list2;
  list2.push_back(10); // Добавляем элемент во второй список

  list1.swap(list2);
    
  // Проверяем, что элементы поменялись
  EXPECT_TRUE(list1.size() == 1);
  EXPECT_TRUE(list2.empty());
  EXPECT_EQ(list1.front(), 10);  // Убедитесь, что в list1 есть элемент
}

// Тест 3: оба списка содержат элементы
TEST(ListSwapTest, BothListsHaveElements) {
  s21::List<int> list1;
  s21::List<int> list2;
    
  list1.push_back(10);
  list1.push_back(20);
    
  list2.push_back(30);
  list2.push_back(40);
    
  list1.swap(list2);
    
  // Проверяем, что данные списков поменялись
  EXPECT_EQ(list1.front(), 30);
  EXPECT_EQ(list2.front(), 10);
  EXPECT_EQ(list1.size(), 2);
  EXPECT_EQ(list2.size(), 2);
}

// Тест 4: оба списка идентичны (ссылаются на один и тот же объект)
TEST(ListSwapTest, SameListSwap) {
  s21::List<int> list1;
    
  list1.push_back(10);
  list1.push_back(20);
    
  // Пытаемся вызвать swap на одном и том же списке
  list1.swap(list1);
    
  // Проверяем, что список не изменился
  EXPECT_EQ(list1.size(), 2);
  EXPECT_EQ(list1.front(), 10);
  EXPECT_EQ(list1.back(), 20);
}

// Тест 5: список с нулевым размером и другим списком
TEST(ListSwapTest, SwapEmptyWithNonEmpty) {
  s21::List<int> list1;
  s21::List<int> list2;
    
  list2.push_back(50);
    
  list1.swap(list2);
    
  // Проверяем, что данные списков поменялись
  EXPECT_EQ(list1.size(), 1);
  EXPECT_EQ(list2.size(), 0);
  EXPECT_EQ(list1.front(), 50);
}


// Пример: слияние двух непустых списков
TEST(ListTest, MergeBasic) {
  s21::List<int> list1;
  list1.push_back(1);
  list1.push_back(3);
  list1.push_back(5);

  s21::List<int> list2;
  list2.push_back(2);
  list2.push_back(4);
  list2.push_back(6);

  list1.merge(list2);

  testing::internal::CaptureStdout();
  list1.print();  // Ожидаем: 1 2 3 4 5 6
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "1 2 3 4 5 6 \n");

  EXPECT_TRUE(list2.empty());
}

// Пример: list2 пуст, list1 — нет
TEST(ListTest, MergeWithEmptyList) {
  s21::List<int> list1;
  list1.push_back(1);
  list1.push_back(2);
  list1.push_back(3);

  s21::List<int> list2;

  list1.merge(list2);

  testing::internal::CaptureStdout();
  list1.print();  // Ожидаем: 1 2 3
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "1 2 3 \n");

  EXPECT_TRUE(list2.empty());
}


// Пример: list1 пуст, list2 — нет
TEST(ListTest, MergeIntoEmptyList) {
  s21::List<int> list1;
  s21::List<int> list2;
  list2.push_back(4);
  list2.push_back(5);
  list2.push_back(6);

  list1.merge(list2);

  testing::internal::CaptureStdout();
  list1.print();  // Ожидаем: 4 5 6
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "4 5 6 \n");

  EXPECT_TRUE(list2.empty());
}


// Пример: слияние с дубликатами
TEST(ListTest, MergeWithDuplicates) {
  s21::List<int> list1;
  list1.push_back(1);
  list1.push_back(3);

  s21::List<int> list2;
  list2.push_back(1);
  list2.push_back(2);
  list2.push_back(3);

  list1.merge(list2);

  testing::internal::CaptureStdout();
  list1.print();  // Ожидаем: 1 1 2 3 3
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "1 1 2 3 3 \n");

  EXPECT_TRUE(list2.empty());
}

TEST(ListTest, SpliceInMiddle) {
    s21::List<int> a;
    s21::List<int> b;
    a.push_back(1);
    a.push_back(4);
    b.push_back(2);
    b.push_back(3);

    auto it = ++a.begin();  // перед 4
    a.splice(it, b);

    testing::internal::CaptureStdout();
    a.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1 2 3 4 \n");
    EXPECT_TRUE(b.empty());
}

TEST(ListTest, SpliceAtBeginning) {
    s21::List<int> a;
    s21::List<int> b;
    a.push_back(3);
    a.push_back(4);
    b.push_back(1);
    b.push_back(2);

    auto it = a.begin();  // перед 3
    a.splice(it, b);

    testing::internal::CaptureStdout();
    a.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1 2 3 4 \n");
    EXPECT_TRUE(b.empty());
}

TEST(ListTest, SpliceAtEnd) {
    s21::List<int> a;
    s21::List<int> b;
    a.push_back(1);
    a.push_back(2);
    b.push_back(3);
    b.push_back(4);

    auto it = a.end();  // вставим в конец
    a.splice(it, b);

    testing::internal::CaptureStdout();
    a.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1 2 3 4 \n");
    EXPECT_TRUE(b.empty());
}

TEST(ListTest, SpliceEmptyIntoNonEmpty) {
    s21::List<int> a;
    s21::List<int> b;
    a.push_back(1);
    a.push_back(2);

    auto it = a.begin();  // не важно, b пустой
    a.splice(it, b);

    testing::internal::CaptureStdout();
    a.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1 2 \n");
    EXPECT_TRUE(b.empty());
}

TEST(ListTest, SpliceNonEmptyIntoEmpty) {
    s21::List<int> a;
    s21::List<int> b;
    b.push_back(1);
    b.push_back(2);

    auto it = a.begin();  // a пустой, вставляем в начало (end == begin)
    a.splice(it, b);

    testing::internal::CaptureStdout();
    a.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1 2 \n");
    EXPECT_TRUE(b.empty());
}

TEST(ListTest, SpliceSelf) {
    s21::List<int> a;
    a.push_back(1);
    a.push_back(2);

    auto it = a.begin();
    a.splice(it, a);  // вставляем сам в себя (в начало)

    testing::internal::CaptureStdout();
    a.print();
    std::string output = testing::internal::GetCapturedStdout();

    // В зависимости от реализации splice, допустимо поведение без изменений
    EXPECT_EQ(output, "1 2 \n");  // Или что ты считаешь корректным
}
