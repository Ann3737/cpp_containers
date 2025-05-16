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
    using reference = value_type &;
    using const_reference = const value_type &;
    using iterator = MapIterator<K, T>; // BinaryTree::iterator
    using const_iterator = MapConstIterator<K, T>; // BinaryTree::const_iterator
    using size_type = size_t;

    // *-----Constructors & destructor-----*

    // Default constructor
    map();

    // Initializer list constructor
    map(std::initializer_list<value_type> const &items);

    // Copy constructor
    map(const map &m);

    // Move constructor
    map(map &&m);

    // Destructor
    ~map();

    // Assignment operator overload for moving object
    operator=(map &&m);


    // *-----Accessors-----*

    // Access specified element with bounds checking
    T& at(const Key& key);

    // Access or insert specified element
    T& operator[](const Key& key);


    // *-----Iterators-----*

    // Returns an iterator to the beginning
    iterator begin();

    // Returns an iterator to the end
    iterator end();


    // *-----Capacity-----*

    // Checks whether the container is empty
    bool empty();

    // Returns the number of elements
    size_type size();

    // Returns the maximum possible number of elements
    size_type max_size();


    // *-----Modifiers-----*

    // Clears the contents
    void clear();

    // Inserts node and returns iterator to where the element is in the container 
    // and bool denoting whether the insertion took place
    std::pair<iterator, bool> insert(const value_type& value);

    // Inserts value by key and returns iterator to where the element is in the container 
    // and bool denoting whether the insertion took place
    std::pair<iterator, bool> insert_or_assign(const Key& key, const T& obj);

    // Erases element at pos
    void erase(iterator pos);

    // Swaps the contents
    void swap(map& other);

    // Splices nodes from another container
    void merge(map& other);


    // *-----Lookup-----*

    // Checks if there is an element with key equivalent to key in the container
    bool contains(const Key& key);


private:


};
}

#endif