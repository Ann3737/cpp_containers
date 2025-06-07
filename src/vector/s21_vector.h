#ifndef __S21_VECTOR_H__
#define __S21_VECTOR_H__

#include <initializer_list>
#include <stdexcept>
#include <cstddef>
#include <limits>
#include <new>
#include <utility>

namespace s21 {
template <typename T>
class vector {
public:

    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using size_type = size_t;

    // Конструктор по умолчанию
    vector() noexcept
            : size_(0), capacity_(0), data_(nullptr) {}

    // Конструктор с параметрами
    vector(size_type n)
            : size_(n), capacity_(n), data_(_AllocateData(this->capacity_)) {
        size_type i = 0;
        try {
            for (; i < n; ++i) {
                new (this->data_ + i) value_type();
            }
        } catch (...) {
            this->size_ = i;
            _DeallocateData();
            throw;
        }
    }

    // Конструтор со списком инициализации
    vector(std::initializer_list<value_type> const &items)
            : size_(items.size()), capacity_(items.size()), data_(_AllocateData(this->capacity_)) {
        size_type i = 0;
        try {
            for (const auto &item : items) {
                new (this->data_ + i) value_type(std::move_if_noexcept(item));
                ++i;
            }
        } catch (...) {
            this->size_ = i;
            _DeallocateData();
            throw;
        }
    }
    
    // Конструктор копирования
    vector(const vector &v)
            : size_(v.size_), capacity_(v.size_), data_(_AllocateData(v.capacity_)) {
        size_type i = 0;
        try {
            for (; i < this->size_; ++i) {
                new (this->data_ + i) value_type(v.data_[i]);
            }
        } catch (...) {
            this->size_ = i;
            _DeallocateData();
            throw;
        }
    }

    // Конструктор перемещения
    constexpr vector(vector &&v) noexcept
            : size_(v.size_), capacity_(v.capacity_), data_(v.data_) {
        v.size_ = 0;
        v.capacity_ = 0;
        v.data_ = nullptr;
    }

    constexpr ~vector() {
        _DeallocateData();
    }


    // *-----МЕТОДЫ ДОСТУПА К ЭЛЕМЕНТАМ ВЕКТОРА-----*

    // Доступ к указанному элементу с проверкой границ
    reference at(size_type pos) {
        if (pos >= this->size_) {
            throw std::out_of_range("Index out of range");
        }
        return this->data_[pos];
    }

    const_reference at(size_type pos) const {
        if (pos >= this->size_) {
            throw std::out_of_range("Index out of range");
        }
        return this->data_[pos];
    }

    // Оператор присваивания
    constexpr vector& operator=(vector &&v) noexcept {
        if (this != &v) {
            _DeallocateData();
            this->size_ = v.size_;
            this->capacity_ = v.capacity_;
            this->data_ = v.data_;

            v.size_ = 0;
            v.capacity_ = 0;
            v.data_ = nullptr;
        }
        return *this;
    }

    // Оператор копирования
    constexpr vector& operator=(const vector &v) {        
        if (this != &v) {
            vector temp(v);
            this->swap(temp);
        }
        return *this;
    }

    // Доступ к указанному элементу
    constexpr reference operator[](size_type pos) {
        return this->data_[pos];
    }

    constexpr const_reference operator[](size_type pos) const {
        return this->data_[pos];
    }

    // Доступ к первому элементу
    constexpr reference front() {
        return this->data_[0];
    }

    constexpr const_reference front() const {
        return this->data_[0];
    }

    // Доступ к последнему элементу
    constexpr reference back() {
        return this->data_[this->size_ - 1];
    }

    constexpr const_reference back() const {
        return this->data_[this->size_ - 1];
    }

    // Доступ к указателю на массив
    constexpr pointer data() noexcept {
        return this->data_;
    }

    constexpr const_pointer data() const noexcept {
        return this->data_;
    }


    // *-----ИТЕРАТОРЫ-----*

    // Возвращает итератор на начало вектора
    constexpr iterator begin() noexcept {
        return this->data_;
    }

    constexpr const_iterator begin() const noexcept {
        return this->data_;
    }

    // Возвращает итератор на конец вектора
    constexpr iterator end() noexcept {
        return this->data_ + this->size_;
    }

    constexpr const_iterator end() const noexcept {
        return this->data_ + this->size_;
    }


    // *-----МЕТОДЫ ДОСТУПА К ИНФОРМАЦИИ О ЗАПОЛНЕННОСТИ ВЕКТОРА-----*

    // Проверяет, пуст ли вектор
    constexpr bool empty() const noexcept {
        return (this->size_ == 0);
    }
    
    // Возвращает количество элементов в векторе
    constexpr size_type size() const noexcept {
        return this->size_;
    }

    // Возвращает максимально допустимое количество элементов в векторе
    constexpr size_type max_size() const noexcept {
        return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }

    // Возвращает количество элементов, которые могут на данный момент храниться в векторе
    constexpr size_type capacity() const noexcept {
        return this->capacity_;
    }

    // Резервирует память для вектора
    void reserve(size_type size) {
        if (size <= this->capacity_) {
            return;
        }
        pointer newData = _AllocateData(size);
        size_type i = 0;
        try {
            for (; i < this->size_; ++i) {
                new(newData + i) value_type(std::move_if_noexcept(this->data_[i]));
            }
        } catch (...) {
            for (size_type new_i = 0; new_i < i; ++new_i) {
                newData[new_i].~value_type();
            }
            ::operator delete(newData);
            throw;
        };
        
        _DeallocateData();
        this->data_ = newData;
        this->capacity_ = size;
    }

    // Удаляет неиспользуемую память в векторе
    void shrink_to_fit() {
        if (this->capacity_ > this->size_) {
            pointer newData = _AllocateData(this->size_);
            size_type i = 0;
            try {
                for (; i < this->size_; ++i) {
                    new(newData + i) value_type(std::move_if_noexcept(this->data_[i]));
                }
            } catch (...) {
                for (size_type new_i = 0; new_i < i; ++new_i) {
                    newData[new_i].~value_type();
                }
                ::operator delete(newData);
                throw;
            }

            this->capacity_ = this->size_;
            _DeallocateData();
            this->data_ = newData;
        }
    }


    // *-----МЕТОДЫ ИЗМЕНЕНИЯ ВЕКТОРА-----*

    // Очищает содержимое вектора
    constexpr void clear() noexcept {
        if (this->data_ != nullptr) {
            for (size_type i = 0; i < this->size_; ++i) {
                this->data_[i].~value_type();
            }
        }
        this->size_ = 0;
    }

    // Вставляет элемент в вектор на указанную позицию
    constexpr iterator insert(const_iterator pos, const_reference value) {
        size_type index_pos = pos - this->begin();
        if (this->size_ == this->capacity_) {
            this->reserve(_CalculateCapacity());
        }
        for (size_type i = this->size_; i > index_pos; --i) {
            new (this->data_ + i) value_type(std::move_if_noexcept(this->data_[i - 1]));
            this->data_[i - 1].~value_type();
        }
        new (this->data_ + index_pos) value_type(value);
        ++this->size_;
        return this->data_ + index_pos;
    }

    // Удаляет элемент в указанной позиции
    iterator erase(iterator pos) {
        if (this->data_ == nullptr) {
            return this->data_;
        }
        size_type index_pos = pos - this->begin();
        this->data_[index_pos].~value_type();
        for (size_type i = index_pos; i < this->size_ - 1; ++i) {
            new (this->data_ + i) value_type(std::move_if_noexcept(this->data_[i + 1]));
            this->data_[i + 1].~value_type();
        }
        --this->size_;
        return this->data_ + index_pos;
    }

    // Вставляет элемент в конец вектора
    constexpr void push_back(const_reference value) {
        if (this->size_ == this->capacity_) {
            this->reserve(_CalculateCapacity());
        }
        new (this->data_ + this->size_) value_type(value);
        ++this->size_;
    }

    // Удаляет последний элемент вектора
    constexpr void pop_back() {
        if (this->data_ == nullptr) {
            return;
        }
        this->back().~value_type();
        --this->size_;
    }

    // Меняет местами содержимое векторов
    constexpr void swap(vector& other) noexcept {
        if (this != &other) {
            std::swap(this->size_, other.size_);
            std::swap(this->capacity_, other.capacity_);
            std::swap(this->data_, other.data_);
        }
    }

private:
    size_type size_;
    size_type capacity_;
    pointer data_;

    // Выделяет память для вектора
    static pointer _AllocateData(size_type cap) {
        return reinterpret_cast<iterator>(::operator new(sizeof(value_type) * cap));
    }

    // Очищает память в векторе
    void _DeallocateData() {
        if (this->data_ != nullptr) {
            for (size_type i = 0; i < this->size_; ++i) {
                this->data_[i].~value_type();
            }
            ::operator delete(this->data_);
        }
    }

    constexpr size_type _CalculateCapacity() const noexcept {
        return (this->capacity_ > 0 ? this->capacity_ * 2 : 1);
    }
};
}

#endif
