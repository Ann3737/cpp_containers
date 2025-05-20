#ifndef __S21_QUEUE_H__
#define __S21_QUEUE_H__

#include "../vector/s21_vector.h"

namespace s21 {
template<typename T, typename Container = s21::vector<T>>
class queue {
public:

    // Переопределения типов
    using value_type = T;
    using container_type = Container;
    using reference = typename container_type::reference;
    using const_reference = typename container_type::const_reference;
    using size_type = typename container_type::size_type;

    // *-----КОНСТРУКТОРЫ И ДЕСТРУКТОР-----*

    // Конструктор по умолчанию
    queue() = default;

    // Конструтор со списком инициализации
    queue(std::initializer_list<value_type> const &items) 
            : container_(items) {}

    // Конструктор копирования
    queue(const queue &q) = default;

    // Конструктор перемещения
    queue(queue &&q) = default;

    // Деструктор
    ~queue() = default;

    // Оператор копирования
    queue& operator=(const queue& q) = default;

    // Оператор присваивания
    queue& operator=(queue &&q) {
        if (this != &q) {
            this->container_ = std::move(q.container_);
        }
        return *this;
    }


    // *-----МЕТОДЫ ДОСТУПА К ЭЛЕМЕНТАМ ОЧЕРЕДИ-----*

    // Доступ к первому элементу
    reference front() {
        return container_.front();
    }
    const_reference front() const {
        return container_.front();
    }

    // Доступ к последнему элементу
    reference back() {
        return container_.back();
    }
    const_reference back() const {
        return container_.back();
    }

    // *-----МЕТОДЫ ДОСТУПА К ИНФОРМАЦИИ О ЗАПОЛНЕННОСТИ ОЧЕРЕДИ-----*

    // Проверяет, пуста ли очередь
    bool empty() const {
        return container_.empty();
    }

    // Возвращает количество элементов в очереди
    size_type size() const {
        return container_.size();
    }


    // *-----МЕТОДЫ ИЗМЕНЕНИЯ ОЧЕРЕДИ-----*

    // Добавляет элемент в конец очереди
    void push(const_reference value) {
        container_.push_back(value);
    }

    // Удаляет элемент из начала очереди
    void pop() {
        container_.erase(container_.begin());
    }

    // Меняет местами содержимое очередей
    void swap(queue& other) {
        container_.swap(other.container_);
    }

private:
    container_type container_;
};
}

#endif