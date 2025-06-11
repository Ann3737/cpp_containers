#ifndef __S21_MAP_H__
#define __S21_MAP_H__

#include <iostream>
#include <initializer_list>
#include <stdexcept>
#include <cstddef>
#include "rb_tree.h"

namespace s21 {
template<typename Key, typename T>
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


    // *-----КОНСТРУКТОРЫ И ДЕСТРУКТОР-----*

    // Конструктор по умолчанию
    map();

    // Конструтор со списком инициализации
    map(std::initializer_list<value_type> const &items);

    // Конструктор копирования
    map(const map &m);

    // Конструктор перемещения
    map(map &&m);

    // Деструктор
    ~map();

    // Оператор присваивания
    map&& operator=(map &&m);

    // Оператор копирования
    map& operator=(const map& m);


    // *-----МЕТОДЫ ДОСТУПА К ЭЛЕМЕНТАМ СЛОВАРЯ-----*

    // Доступ к указанному элементу с проверкой границ
    T& at(const key_type& key);

    const T& at(const key_type& key) const;

    // Доступ или вставка указанного элемента
    T& operator[](const key_type& key);


    // *-----ИТЕРАТОРЫ-----*

    // Возвращает итератор на начало словаря
    iterator begin();

    const_iterator begin() const;

    // Возвращает итератор на конец словаря
    iterator end();

    const_iterator end() const;


    // *-----МЕТОДЫ ДОСТУПА К ИНФОРМАЦИИ О ЗАПОЛНЕННОСТИ СЛОВАРЯ-----*

    // Проверяет, пуст ли словарь
    bool empty();

    // Возвращает количество элементов словаря
    size_type size();

    // Возвращает максимально возможное количество элементов словаря
    size_type max_size();


    // *-----МЕТОДЫ ИЗМЕНЕНИЯ СЛОВАРЯ-----*

    // Очищает словарь
    void clear();

    // Вставляет узел и возвращает итератор, 
    // указывающий на местонахождение элемента в контейнере,
    // а так же возвращает логическое значение, указывающее, произошла ли вставка
    std::pair<iterator, bool> insert(const value_type& value);

    // Вставляет значение по ключу и возвращает итератор, 
    // указывающий на местонахождение элемента в контейнере,
    // а так же возвращает логическое значение, указывающее, произошла ли вставка
    std::pair<iterator, bool> insert(const key_type& key, 
                const mapped_type& obj);

    // Вставляет элемент или назначает его текущему элементу, если ключ уже существует
    std::pair<iterator, bool> insert_or_assign(const key_type& key, 
                const mapped_type& obj);

    // Удаляет элемент в указанной позиции
    void erase(iterator pos);

    // Меняет местами содержимое словарей
    void swap(map& other);

    // Сращивает узлы из другого контейнера
    void merge(map& other);


    // *-----МЕТОДЫ ДЛЯ ПРОСМОТРА СОДЕРЖИМОГО СЛОВАРЯ-----*

    // Проверяет, есть ли элемент с ключом, эквивалентным ключу в словаре
    bool contains(const key_type& key);


private:
    rbtree<key_type, mapped_type> tree_;
    size_type size_;

};
}

#endif