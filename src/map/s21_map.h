#ifndef __S21_MAP_H__
#define __S21_MAP_H__

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

#include "rb_tree.h"

namespace s21 {
template <typename Key, typename T>
class map {
 public:
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const key_type, mapped_type>;
  using reference = value_type&;
  using const_reference = const value_type&;
  using iterator = typename rbtree<Key, T>::iterator;
  using const_iterator = typename rbtree<Key, T>::const_iterator;
  using size_type = size_t;

  // Конструктор по умолчанию
  map() = default;

  // Конструтор со списком инициализации
  map(std::initializer_list<value_type> const& items) : map() {
    for (const auto& item : items) {
      tree_.insert(item);
    }
  }

  // Конструктор копирования
  map(const map& m) : tree_(m.tree_) {}

  // Конструктор перемещения
  map(map&& m) noexcept : tree_(std::move(m.tree_)) {}

  ~map() { tree_.clear(); }

  // Оператор копирования
  map& operator=(const map& m) {
    if (this != &m) {
      this->clear();
      this->tree_ = m.tree_;
    }
    return *this;
  }

  // Оператор присваивания
  map& operator=(map&& m) noexcept {
    if (this != &m) {
      this->clear();
      this->tree_ = std::move(m.tree_);
      m.tree_.clear();
    }
    return *this;
  }

  // *-----МЕТОДЫ ДОСТУПА К ЭЛЕМЕНТАМ СЛОВАРЯ-----*

  // Доступ к указанному элементу с проверкой границ
  mapped_type& at(const key_type& key) {
    auto it = tree_.find(key);
    if (it == tree_.end()) {
      throw std::out_of_range("Key not found");
    }
    return it->second;
  }

  const mapped_type& at(const key_type& key) const {
    auto it = tree_.find(key);
    if (it == tree_.end()) {
      throw std::out_of_range("Key not found");
    }
    return it->second;
  }

  // Доступ или вставка указанного элемента
  mapped_type& operator[](const key_type& key) {
    auto it = tree_.find(key);
    if (it != tree_.end()) {
      return it->second;
    } else {
      auto [newNode, success] = tree_.insert({key, mapped_type()});
      return newNode->second;
    }
  }

  // *-----ИТЕРАТОРЫ-----*

  // Возвращает итератор на начало словаря
  iterator begin() const noexcept { return tree_.begin(); }

  const_iterator cbegin() const noexcept { return tree_.cbegin(); }

  // Возвращает итератор на конец словаря
  iterator end() const noexcept { return tree_.end(); }

  const_iterator cend() const noexcept { return tree_.cend(); }

  iterator find(const key_type& key) { return tree_.find(key); }

  const_iterator find(const key_type& key) const { return tree_.find(key); }

  // *-----МЕТОДЫ ДОСТУПА К ИНФОРМАЦИИ О ЗАПОЛНЕННОСТИ СЛОВАРЯ-----*

  // Проверяет, пуст ли словарь
  bool empty() const noexcept { return tree_.empty(); }

  // Возвращает количество элементов словаря
  size_type size() const noexcept { return tree_.size(); }

  // Возвращает максимально возможное количество элементов словаря
  size_type max_size() const noexcept { return tree_.max_size(); }

  // *-----МЕТОДЫ ИЗМЕНЕНИЯ СЛОВАРЯ-----*

  // Очищает словарь
  void clear() noexcept { tree_.clear(); }

  // Вставляет узел и возвращает итератор,
  // указывающий на местонахождение элемента в контейнере,
  // а так же возвращает логическое значение, указывающее, произошла ли вставка
  std::pair<iterator, bool> insert(const value_type& value) {
    return tree_.insert(value);
  }

  // Вставляет значение по ключу и возвращает итератор,
  // указывающий на местонахождение элемента в контейнере,
  // а так же возвращает логическое значение, указывающее, произошла ли вставка
  std::pair<iterator, bool> insert(const key_type& key,
                                   const mapped_type& obj) {
    return tree_.insert(key, obj);
  }

  // Вставляет элемент или назначает его текущему элементу, если ключ уже
  // существует
  std::pair<iterator, bool> insert_or_assign(const key_type& key,
                                             const mapped_type& obj) {
    return tree_.insert_or_assign(key, obj);
  }

  // Удаляет элемент в указанной позиции
  iterator erase(iterator pos) { return tree_.erase(pos); }

  const_iterator erase(const_iterator pos) { return tree_.erase(pos); }

  // Меняет местами содержимое словарей
  void swap(map& other) noexcept { std::swap(this->tree_, other.tree_); }

  // Сращивает узлы из другого контейнера
  void merge(map& other) {
    for (auto it = other.begin(); it != other.end();) {
      if (!this->contains(it->first)) {
        this->insert(std::move(*it));
        it = other.erase(it);
      } else {
        ++it;
      }
    }
  }

  // *-----МЕТОДЫ ДЛЯ ПРОСМОТРА СОДЕРЖИМОГО СЛОВАРЯ-----*

  // Проверяет, есть ли элемент с ключом, эквивалентным ключу в словаре
  bool contains(const key_type& key) const {
    auto it = tree_.find(key);
    return (it != tree_.end());
  }

 private:
  rbtree<key_type, mapped_type> tree_;
};
}

#endif