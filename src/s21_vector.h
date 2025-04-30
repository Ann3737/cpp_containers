#ifndef __S21_VECTOR_H__
#define __S21_VECTOR_H__

#include <iostream>
#include <initializer_list>

template <typename T>
class S21Vector {
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using iterator = T*;
    using const_iterator = const T*;
    using size_type = size_t;

    // Constructors & destructor
    S21Vector();
    S21Vector(size_type n);
    S21Vector(std::initializer_list<value_type> const &items);
    S21Vector(const S21Vector &v);
    S21Vector(S21Vector &&v);
    ~S21Vector();

    // Element access
    reference at(size_type pos);
    S21Vector& operator=(S21Vector &&v);
    reference operator[](size_type pos);
    const_reference front() const;
    const_reference back() const;
    S21Vector T* data();

    // Iterators
    iterator begin();
    iterator end();

    // Capacity
    bool empty();
    size_type size();
    size_type max_size();
    void reserve(size_type size);
    size_type capacity();
    void shrink_to_fit();

    // Modifiers
    void clear();
    iterator insert(iterator pos, const_reference value);
    void erase(iterator pos);
    void push_back(const_reference value);
    void pop_back();
    void swap(S21Vector& other);

private:
    size_type pos;
    T* data;
};




#endif
