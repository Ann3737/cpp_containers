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
