#ifndef __S21_VECTOR_H__
#define __S21_VECTOR_H__

#include <iostream>
#include <initializer_list>
#include <cstddef>
#include <new>

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
            : size_(n), capacity_(n), data_(_AllocateData()) {
        for (size_type i = 0; i < n; ++i) {
            new (this->data_ + i) value_type();
        }
    }

    // Initializer list constructor
    vector(std::initializer_list<value_type> const &items)
            : size_(items.size()), capacity_(items.size()), data_(_AllocateData()) {
        size_type i = 0;
        for (const auto &item : items) {
            new (this->data_ + i) value_type(item);
            ++i;
        }
    }
    
    // Copy constructor
    vector(const vector &v)
            : size_(v.size_), capacity_(v.size_), data_(_AllocateData()) {
        for (size_type i = 0; i < this->size_; ++i) {
            new (this->data_ + i) value_type(v.data_[i]);
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
    vector& operator=(vector &&v);
    reference operator[](size_type pos);
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
    bool empty();
    size_type size() const;
    size_type max_size() const;
    void reserve(size_type size);
    size_type capacity() const;
    void shrink_to_fit();

    // Modifiers
    void clear();
    iterator insert(iterator pos, const_reference value);
    void erase(iterator pos);
    void push_back(const_reference value);
    void pop_back();
    void swap(vector& other);

private:
    size_type size_;
    size_type capacity_;
    iterator data_;

    iterator _AllocateData() {
        return reinterpret_cast<iterator>(::operator new(sizeof(value_type) * this->capacity_));
    }
};


#endif
