#ifndef __S21_MAP_H__
#define __S21_MAP_H__

#include <iostream>
#include <initializer_list>
#include <stdexcept>
#include <cstddef>

namespace s21 {
template<typename Key, typename T>
class map {
public:
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const key_type, mapped_type>;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = MapIterator<K, T>; // BinaryTree::iterator
    using const_iterator = MapConstIterator<K, T>; // BinaryTree::const_iterator
    using size_type = size_t;

     struct BaseNode {
        BaseNode* left;
        BaseNode* right;
        BaseNode* parent;
    };

    struct Node : BaseNode {
        std::pair<const Key, T> kt;
        bool red;
    };

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
    operator=(map &&m);


    // *-----МЕТОДЫ ДОСТУПА К ЭЛЕМЕНТАМ СЛОВАРЯ-----*

    // Доступ к указанному элементу с проверкой границ
    T& at(const Key& key);

    const T& at(const Key& key) const;

    // Доступ или вставка указанного элемента
    T& operator[](const Key& key);


    // *-----ИТЕРАТОРЫ-----*

    // Возвращает итератор на начало словаря
    iterator begin();

    // Возвращает итератор на конец словаря
    iterator end();


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
    std::pair<iterator, bool> insert(const Key& key, const T& obj);

    // Вставляет элемент или назначает его текущему элементу, если ключ уже существует
    std::pair<iterator, bool> insert_or_assign(const Key& key, const T& obj);

    // Удаляет элемент в указанной позиции
    void erase(iterator pos);

    // Меняет местами содержимое словарей
    void swap(map& other);

    // Сращивает узлы из другого контейнера
    void merge(map& other);


    // *-----МЕТОДЫ ДЛЯ ПРОСМОТРА СОДЕРЖИМОГО СЛОВАРЯ-----*

    // Проверяет, есть ли элемент с ключом, эквивалентным ключу в словаре
    bool contains(const Key& key);


private:
    BaseNode* fakeNode_;
    BaseNode* begin_;
    size_type size_;

};
}

#endif