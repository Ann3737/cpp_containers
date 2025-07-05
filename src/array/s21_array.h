#ifndef __S21_VECTOR_H__
#define __S21_VECTOR_H__

#include <cstddef>
#include <initializer_list>
#include <limits>
#include <new>
#include <stdexcept>
#include <utility>

namespace s21 {
template <typename T>
class array {
public:
    using value_type = T;
    using reference = T &;
    using const_reference = const T &;
    using iterator = T *;
    using const_iterator = const T *;
    using size_type = size_t;

    // Конструктор по умолчанию
    array();

    // Конструтор со списком инициализации
    array(std::initializer_list<value_type> const &items);

    // Конструктор копирования
    array(const array &a);

    // Конструктор перемещения
    array(array &&a);

    // Деструктор
    ~array();

    // Оператор присваивания
    array &operator=(array &&a);

    // *-----МЕТОДЫ ДОСТУПА К ЭЛЕМЕНТАМ МАССИВА-----*

    // Доступ к указанному элементу с проверкой границ
    reference at(size_type pos);

    // Доступ к указанному элементу
    reference operator[](size_type pos);

    // Доступ к первому элементу
    const_reference front();

    // Доступ к последнему элементу
    const_reference back();

    // *-----ИТЕРАТОРЫ-----*

    // Возвращает итератор на массив
    iterator data();

    // Возвращает итератор на начало массива
    iterator begin();

    // Возвращает итератор на конец массива
    iterator end();

    // *-----МЕТОДЫ ДОСТУПА К ИНФОРМАЦИИ О ЗАПОЛНЕННОСТИ МАССИВА-----*

    // Проверяет, пуст ли массив
    bool empty();

    // Возвращает количество элементов в массиве
    size_type size();

    // Возвращает максимально допустимое количество элементов в массиве
    size_type max_size();

    // меняет местами содержимое массивов
    void swap(array& other);

    // присваивает заданное значение value всем элементам в массиве
    void fill(const_reference value);
    
private:
    size_type size_;
    T* data_;
};
};

#endif    