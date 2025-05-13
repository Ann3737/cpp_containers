#ifndef __S21_VECTOR_H__
#define __S21_VECTOR_H__

#include <iostream>
#include <initializer_list>
#include <stdexcept>
#include <cstddef>
#include <new>

namespace s21{
template <typename T>
class vector {
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using iterator = T*;
    using const_iterator = const T*;
    using size_type = size_t;

    // Constructors & destructor

    // Default constructor
    vector()
            : size_(0), capacity_(0), data_(nullptr) {}

    // Parameterized constructor
    vector(size_type n)
            : size_(n), capacity_(n), data_(_AllocateData(capacity_)) {
        size_type i = 0;
        try {
            for (; i < n; ++i) {
                new (this->data_ + i) value_type();
            }
        } catch (...) {
            for (size_type new_i = 0; new_i < i; ++i) {
                this->data_[new_i].~value_type();
            }
            ::operator delete(this->data_);
            throw;
        }
    }

    // Initializer list constructor
    vector(std::initializer_list<value_type> const &items)
            : size_(items.size()), capacity_(items.size()), data_(_AllocateData(capacity_)) {
        size_type i = 0;
        try {
            for (const auto &item : items) {
                new (this->data_ + i) value_type(item);
                ++i;
            }
        } catch (...) {
            for (size_type new_i = 0; new_i < i; ++i) {
                this->data_[new_i].~value_type();
            }
            ::operator delete(this->data_);
            throw;
        }
    }
    
    // Copy constructor
    vector(const vector &v)
            : size_(v.size_), capacity_(v.size_), data_(_AllocateData(capacity_)) {
        size_type i = 0;
        try {
            for (; i < this->size_; ++i) {
                new (this->data_ + i) value_type(v.data_[i]);
            }
        } catch (...) {
            for (size_type new_i = 0; new_i < i; ++i) {
                this->data_[new_i].~value_type();
            }
            ::operator delete(this->data_);
            throw;

        }
    }

    // Move constructor
    vector(vector &&v) noexcept
            : size_(v.size_), capacity_(v.capacity_), data_(v.data_) {
        v.size_ = 0;
        v.capacity_ = 0;
        v.data_ = nullptr;
    }

    // Destructor
    ~vector() noexcept {
        if (this->data_ != nullptr) {
            for (size_type i = 0; i < this->size_; ++i) {
                this->data_[i].~value_type();
            }
            ::operator delete(this->data_);
        }
    }

    // Element access
    reference at(size_type pos);
    vector& operator=(vector &&v) noexcept {
        if (this != &v) {

        }
    }

    // vector& operator=(const vector &v) noexcept {
    //     if (this != &v) {
    //         vector temp(v);
    //         swap(temp);
    //     }
    //     return *this;
    // }

    reference operator[](size_type pos) {
        if (pos >= this->size_) {
            throw std::out_of_range("Index out of range");
        }
        return this->data_[pos];
    }
    // const_reference operator[](size_type pos) const;
    const_reference front() const;
    const_reference back() const;

    // Iterators
    iterator data();
    iterator begin();
    // const_iterator cbegin() const;
    iterator end();
    // const_iterator cend() const;

    // Capacity
    bool empty() { return (this->size_ == 0); }
    size_type size() const { return this->size_; }
    size_type max_size() const;

    // Reserve capacity memory
    void reserve(size_type size) {
        if (size <= this->capacity_) {
            return;
        }
        iterator newData = _AllocateData(size);
        size_type i = 0;
        try {
            for (; i < this->size_; ++i) {
                new(newData + i) value_type(this->data_[i]);
            }
        } catch (...) {
            for (size_type new_i = 0; new_i < i; ++new_i) {
                newData[new_i].~value_type();
            }
            ::operator delete(newData);
            throw;
        };
        
        for (size_type i = 0; i < this->size_; ++i) {
            this->data_[i].~value_type();
        }

        ::operator delete(this->data_);
        this->data_ = newData;
        this->capacity_ = size;
    }

    size_type capacity() const { return this->capacity_; }
    // void shrink_to_fit();

    // // Modifiers
    // void clear();
    // iterator insert(iterator pos, const_reference value);
    // void erase(iterator pos);
    void push_back(const_reference value) {
        if (this->size_ == this->capacity_) {
            reserve(this->capacity_ > 0 ? this->capacity_ * 2 : 1);
        }
        try {
            new (this->data_ + this->size_) value_type(value);
        } catch (...) {
            throw;
        }

        ++this->size_;
    }
    // void pop_back();
    void swap(vector& other) {
        if (this != other) {
            std::swap(this->size_, other.size_);
            std::swap(this->capacity_, other.capacity_);
            std::swap(this->data_, other.data_);
        }
    }

private:
    size_type size_;
    size_type capacity_;
    iterator data_;

    iterator _AllocateData(size_type cap) {
        return reinterpret_cast<iterator>(::operator new(sizeof(value_type) * cap));
    }
};
}

#endif
