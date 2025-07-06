#ifndef __S21_ARRAY_H__
#define __S21_ARRAY_H__

#include <cstddef>
#include <initializer_list>
#include <limits>
#include <new>
#include <stdexcept>
#include <utility>

namespace s21 {
template <typename T, size_t N>
class array {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;
  using iterator = T *;
  using const_iterator = const T *;
  using size_type = size_t;

  // Конструктор по умолчанию
  array() {
    for (size_type i = 0; i < N; ++i) {
      this->arr_[i] = T();
    }
  }

  // Конструтор со списком инициализации
  array(std::initializer_list<value_type> const &items) {
    if (items.size() > N) {
      throw std::out_of_range("Elements are more then size of array");
    }

    size_type i = 0;
    for (const auto &item : items) {
      this->arr_[i] = item;
      i++;
    }
    for (; i < N; i++) {
      this->arr_[i] = T();
    }
  }

  // Конструктор копирования
  array(const array &a) {
    for (size_type i = 0; i < N; i++) {
      this->arr_[i] = a.arr_[i];
    }
  }

  // Конструктор перемещения
  array(array &&a) noexcept {
    for (size_type i = 0; i < N; i++) {
      this->arr_[i] = std::move(a.arr_[i]);
    }
  }

  // Деструктор
  ~array() = default;

  // Оператор присваивания
  array &operator=(array &&a) {
    if (this != &a) {
      for (size_type i = 0; i < N; ++i) {
        this->arr_[i] = std::move(a.arr_[i]);
      }
    }
    return *this;
  }

  // Оператор копирования
  array &operator=(const array &a) {
    if (this != &a) {
      for (size_type i = 0; i < N; ++i) {
        this->arr_[i] = a.arr_[i];
      }
    }
    return *this;
  }

  // *-----МЕТОДЫ ДОСТУПА К ЭЛЕМЕНТАМ МАССИВА-----*

  // Доступ к указанному элементу с проверкой границ
  constexpr reference at(size_type pos) {
    if (pos >= N) {
      throw std::out_of_range("Index out of range");
    } else {
      return this->arr_[pos];
    }
  }

  constexpr const_reference at(size_type pos) const {
    if (pos >= N) {
      throw std::out_of_range("Index out of range");
    } else {
      return this->arr_[pos];
    }
  }

  // Доступ к указанному элементу
  constexpr reference operator[](size_type pos) { return this->arr_[pos]; }

  constexpr const_reference operator[](size_type pos) const {
    return this->arr_[pos];
  }

  // Доступ к первому элементу
  constexpr reference front() { return this->arr_[0]; }

  constexpr const_reference front() const { return this->arr_[0]; }

  // Доступ к последнему элементу
  constexpr reference back() { return this->arr_[N - 1]; }

  constexpr const_reference back() const { return this->arr_[N - 1]; }

  // Возвращает указатель на массив
  constexpr pointer data() noexcept { return this->arr_; }

  constexpr const_pointer data() const noexcept { return this->arr_; }

  // *-----ИТЕРАТОРЫ-----*

  // Возвращает итератор на начало массива
  constexpr iterator begin() noexcept { return this->arr_; }

  constexpr const_iterator begin() const noexcept { return this->arr_; }

  // Возвращает итератор на конец массива
  constexpr iterator end() noexcept { return this->arr_ + this->size(); }

  constexpr const_iterator end() const noexcept {
    return this->arr_ + this->size();
  }

  // *-----МЕТОДЫ ДОСТУПА К ИНФОРМАЦИИ О ЗАПОЛНЕННОСТИ МАССИВА-----*

  // Проверяет, пуст ли массив
  constexpr bool empty() const noexcept { return N == 0; }

  // Возвращает количество элементов в массиве
  constexpr size_type size() const noexcept { return N; }

  // Возвращает максимально допустимое количество элементов в массиве
  constexpr size_type max_size() const noexcept { return N; }

  // Меняет местами содержимое массивов
  constexpr void swap(array &other) noexcept {
    if (this != &other) {
      for (size_type i = 0; i < N; ++i) {
        std::swap(this->arr_[i], other.arr_[i]);
      }
    }
  }

  // Присваивает заданное значение value всем элементам в массиве
  constexpr void fill(const_reference value) {
    for (size_type i = 0; i < N; ++i) {
      this->arr_[i] = value;
    }
  }

 private:
  value_type arr_[N];
};
};  // namespace s21

#endif