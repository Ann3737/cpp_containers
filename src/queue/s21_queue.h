#ifndef __S21_QUEUE_H__
#define __S21_QUEUE_H__

#include "../vector/s21_vector.h"

namespace s21 {
template<typename T, typename Container = s21::vector<T>>
class queue {
public:
    using value_type = T;
    using container_type = Container;
    using reference = typename container_type::reference;
    using const_reference = typename container_type::const_reference;
    using size_type = typename container_type::size_type;

    queue() = default;
    queue(std::initializer_list<value_type> const &items) 
            : container_(items) {}
    queue(const queue &q) = default;
    queue(queue &&q) = default;
    ~queue() = default;
    queue& operator=(const queue& q) = default;
    queue& operator=(queue &&q) {
        if (this != &q) {
            this->container_ = std::move(q.container_);
        }
        return *this;
    }

    reference front() {
        return container_.front();
    }
    const_reference front() const {
        return container_.front();
    }

    reference back() {
        return container_.back();
    }
    const_reference back() const {
        return container_.back();
    }

    bool empty() const {
        return container_.empty();
    }
    size_type size() const {
        return container_.size();
    }

    void push(const_reference value) {
        container_.push_back(value);
    }
    void pop() {
        container_.erase(container_.begin());
    }
    void swap(queue& other) {
        container_.swap(other.container_);
    }

private:
    container_type container_;
};
}

#endif