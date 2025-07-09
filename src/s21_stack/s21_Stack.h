#include "../s21_List/s21_list.h"

namespace s21 {
/*
T — тип элементов, которые будут храниться в стеке;
Container — используемый контейнер, по умолчанию это s21::List<T>
*/
template <typename T, typename Container = s21::List<T>>
class Stack {
 public:
  // типы
  using value_type = T;
  using container_type = Container;
  using reference = typename Container::reference;
  using const_reference = typename Container::const_reference;
  using size_type = typename Container::size_type;

  // конструкторы
  Stack() = default;
  explicit Stack(const Container& cont) : c_(cont) {}
  Stack(const Stack& other) = default;
  Stack(Stack&& other) noexcept = default;
  Stack& operator=(const Stack& other) = default;
  Stack& operator=(Stack&& other) noexcept = default;
  Stack(std::initializer_list<value_type> const& items) {
    for (auto it = items.begin(); it != items.end(); ++it) {
      this->push(*it);
    }
  }
  ~Stack() = default;

  // методы
  bool empty() const { return c_.empty(); }
  size_type size() const { return c_.size(); }
  reference top() { return c_.back(); }
  const_reference top() const { return c_.back(); }

  void push(const value_type& value) { c_.push_back(value); }
  void push(value_type&& value) { c_.push_back(std::move(value)); }

  void pop() { c_.pop_back(); }

  void swap(Stack& other) { c_.swap(other.c_); }

 private:
  Container c_;
};

}  // namespace s21
