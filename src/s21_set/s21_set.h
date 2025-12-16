#include <algorithm>  // для std::swap
#include <cstddef>    // для size_t
#include <iostream>   // для std::cout, std::endl
#include <limits>
#include <stdexcept>  // для std::out_of_range
#include <string>     // для std::string
#include <utility>    // для std::pair
#include <vector>
namespace s21 {

template <typename T>
class set {
 private:
  enum class Color { RED, BLACK };

  struct Node {
    T data_;
    Node* left_;
    Node* right_;
    Node* parent_;
    Color color_;
    Node(const T& value)
        : data_(value),
          left_(nullptr),
          right_(nullptr),
          parent_(nullptr),
          color_(Color::RED) {}
  };

  Node* root_;
  size_t size_;

  // приватная рекурсивная функция вставки
  Node* insertNode(Node*& current, Node* parent, const T& value,
                   bool& inserted_flag) {
    if (!current) {
      current = new Node(value);
      current->parent_ = parent;
      inserted_flag = true;
      return current;
    }

    if (value < current->data_) {
      return insertNode(current->left_, current, value, inserted_flag);
    } else if (value > current->data_) {
      return insertNode(current->right_, current, value, inserted_flag);
    }

    return current;
  }

  void printTree(Node* node, std::string prefix = "",
                 bool isLeft = true) const {
    std::cout << prefix;

    // Для последнего узла └──, для остальных ├──
    std::cout << (isLeft ? "├──" : "└──");

    if (node == nullptr) {
      // Отображаем пустой узел
      std::cout << "ø" << "\n";
      return;
    }

    // Выводим значение и цвет узла
    std::cout << node->data_ << (node->color_ == Color::RED ? " (R)" : " (B)")
              << "\n";

    // Рекурсивно выводим детей с увеличением отступа
    // Даже если один из детей nullptr, всё равно вызываем printTree для него
    printTree(node->left_, prefix + (isLeft ? "│   " : "    "), true);
    printTree(node->right_, prefix + (isLeft ? "│   " : "    "), false);
  }

  void copyTree(Node* node) {
    if (node) {
      insert(node->data_);  // используем insert с балансировкой
      copyTree(node->left_);
      copyTree(node->right_);
    }
  }

  void RL_rotate(Node*& current) {
    Node* grand = current;
    Node* father = current->right_;
    Node* sun = father->left_;

    sun->parent_ = nullptr;
    root_ = sun;

    grand->parent_ = sun;

    grand->right_ = nullptr;
    sun->left_ = grand;

    father->left_ = nullptr;
    sun->right_ = father;
    father->parent_ = sun;

    sun->color_ = Color::BLACK;
    sun->left_->color_ = Color::RED;
    sun->right_->color_ = Color::RED;
  }

  void LR_rotate(Node*& current) {
    Node* grand = current;
    Node* father = current->left_;
    Node* sun = father->right_;

    if (grand->parent_) {
      sun->parent_ = grand->parent_;
      grand->parent_->right_ = sun;

    } else {
      sun->parent_ = nullptr;
      root_ = sun;
    }

    grand->parent_ = sun;

    grand->left_ = nullptr;
    sun->right_ = grand;
    father->right_ = nullptr;
    sun->left_ = father;
    father->parent_ = sun;

    sun->color_ = Color::BLACK;
    sun->left_->color_ = Color::RED;
    sun->right_->color_ = Color::RED;
  }

  void RR_rotate(Node*& current) {
    Node* sun = current->right_;
    Node* tem = current;

    if (current->parent_) {
      if (current->parent_->left_ == current) {
        current->parent_->left_ = sun;

      } else {
        current->parent_->right_ = sun;
      }
      sun->parent_ = current->parent_;
    } else {
      sun->parent_ = nullptr;
      root_ = sun;
    }

    if (sun->left_) {
      current->right_ = sun->left_;
      sun->left_->parent_ = current;
      current->parent_ = sun;
      sun->left_ = current;

    } else {
      tem->right_ = sun->left_;
      sun->left_ = tem;
      tem->parent_ = sun;
    }
    sun->color_ = Color::BLACK;
    tem->color_ = Color::RED;
  }

  void LL_rotate(Node*& current) {
    Node* sun = current->left_;
    Node* tem = current;

    if (current->parent_) {
      if (current->parent_->left_ == current) {
        current->parent_->left_ = sun;

      } else {
        current->parent_->right_ = sun;
      }
      sun->parent_ = current->parent_;

    } else {
      sun->parent_ = nullptr;
      root_ = sun;
    }

    if (sun->right_) {
      current->left_ = sun->right_;
      sun->right_->parent_ = current;
      sun->right_ = current;
      current->parent_ = sun;

    } else {
      tem->left_ = sun->right_;
      sun->right_ = tem;
      tem->parent_ = sun;
    }
    sun->color_ = Color::BLACK;
    tem->color_ = Color::RED;
  }

  void balance_set(Node*& current) {
    if (!current || !current->parent_) {
      return;  // защита, если корень или parent = nullptr
    }
    if (current->color_ == Color::RED &&
        current->parent_->color_ == Color::RED) {
      //  Нарушено правило — запускаем балансировку
      Node* grandparent =
          current->parent_->parent_;  // создаем указатель на деда

      Node* parent = current->parent_;  // создаем указатель на родителя

      // создал указатель на дядю
      Node* uncle = (parent == grandparent->left_) ? grandparent->right_
                                                   : grandparent->left_;

      if (parent == grandparent->left_) {  // родитель — левый
        if (uncle != nullptr && uncle->color_ == Color::RED) {
          uncle->color_ = Color::BLACK;
          parent->color_ = Color::BLACK;
          grandparent->color_ = Color::RED;
          if (grandparent == this->root_) {
            grandparent->color_ = Color::BLACK;
          } else {
            balance_set(grandparent);  // рекурсия для деда
          }

        } else {
          if (current == parent->left_) {
            LL_rotate(grandparent);
          } else {
            LR_rotate(grandparent);
          }
        }

      } else {  // родитель — правый

        if (uncle != nullptr && uncle->color_ == Color::RED) {
          uncle->color_ = Color::BLACK;
          parent->color_ = Color::BLACK;
          grandparent->color_ = Color::RED;
          if (grandparent == this->root_) {
            grandparent->color_ = Color::BLACK;
          } else {
            balance_set(grandparent);  // рекурсия для деда
          }

        } else {
          if (current == parent->right_) {
            RR_rotate(grandparent);
          } else {
            RL_rotate(current->parent_->parent_);
          }
        }
      }
    }
  }

 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;

  using key_type = T;

  /*---- Конструкторы -----*/
  set() : root_(nullptr), size_(0) {}

  set(const T& value) : root_(nullptr), size_(0) { insert(value); }

  set(const set& other) : root_(nullptr), size_(0) { copyTree(other.root_); }

  set(set&& other) noexcept : root_(other.root_), size_(other.size_) {
    other.root_ = nullptr;
    other.size_ = 0;
  }

  ~set() {
    clearTree(this->root_);
    this->root_ = nullptr;
    this->size_ = 0;
  }

  set(std::initializer_list<value_type> const& items)
      : root_(nullptr), size_(0) {
    for (auto& item : items) {
      insert(item);
    }
  }

  void clearTree(Node* tree) {
    if (tree) {
      clearTree(tree->left_);
      clearTree(tree->right_);
      delete tree;
    }
  }

  void clear() {
    clearTree(root_);
    root_ = nullptr;
    size_ = 0;
  }

  size_t max_size() const {
    return std::numeric_limits<size_t>::max() / sizeof(Node);
  }

  void print() const { printTree(root_, "", false); }

  /*---- Проверить, пустое ли множество ----*/
  bool empty() const { return this->size_ == 0; }

  size_t size() const { return size_; }

  void swap(set& other) {
    std::swap(root_, other.root_);
    std::swap(size_, other.size_);
  }

  /* ----Класс итератор ---- */
  class iterator {
    friend class set<T>;

   private:
    Node* current_;  // текущий узел
    Node* root;

   public:
    // Типы для поддержки STL
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    /*констуктор*/
    iterator(Node* node, Node* root = nullptr) : current_(node), root(root) {}

    // Разыменование — получаем значение
    T& operator*() const {
      if (current_ == nullptr) {
        throw std::out_of_range("Dereferencing end() iterator");
      }
      return current_->data_;
    }  // *it — получить данные

    /*префиксный инкремент*/
    iterator& operator++() {
      // здесь должен быть код
      if (current_ == nullptr) return *this;  // end()
      if (current_->right_ != nullptr) {
        // Переходим к минимальному элементу в правом поддереве
        current_ = current_->right_;
        while (current_->left_ != nullptr) {
          current_ = current_->left_;
        }
      } else {
        // Поднимаемся вверх, пока текущий узел — правый потомок
        Node* parent = current_->parent_;
        while (parent != nullptr && current_ == parent->right_) {
          current_ = parent;
          parent = parent->parent_;
        }
        current_ = parent;
      }
      return *this;
    }

    /*декремент*/
    iterator& operator--() {
      if (current_ == nullptr) {
        current_ = this->root;
        while (current_->right_ != nullptr) {
          current_ = current_->right_;
        }
      } else {
        if (current_->left_ != nullptr) {
          current_ = current_->left_;
          while (current_->right_ != nullptr) {
            current_ = current_->right_;
          }
        } else {
          Node* parent = current_->parent_;
          while (parent != nullptr && current_ == parent->left_) {
            current_ = parent;
            parent = parent->parent_;
          }
          current_ = parent;
        }
      }

      return *this;
    }

    /*операторы сравнения*/
    bool operator!=(const iterator& other) const {  // сравнение
      return current_ != other.current_;
    }
    bool operator==(const iterator& other) const {
      return current_ == other.current_;
    }
  };

  class const_iterator {
    friend class set<T>;

   private:
    const Node* current_;  // указатель на const Node
    const Node* root;      // root тоже const

   public:
    // Конструктор
    const_iterator(const iterator& it) : current_(it.current_), root(it.root) {}

    const_iterator(const Node* node, const Node* root = nullptr)
        : current_(node), root(root) {}

    // Разыменование — вернёт const-ссылку
    const T& operator*() const {
      if (current_ == nullptr) {
        throw std::out_of_range("Dereferencing end() const_iterator");
      }
      return current_->data_;
    }

    // ++ (префиксный)
    const_iterator& operator++() {
      if (current_ == nullptr) return *this;
      if (current_->right_ != nullptr) {
        current_ = current_->right_;
        while (current_->left_ != nullptr) {
          current_ = current_->left_;
        }
      } else {
        const Node* parent = current_->parent_;
        while (parent != nullptr && current_ == parent->right_) {
          current_ = parent;
          parent = parent->parent_;
        }
        current_ = parent;
      }
      return *this;
    }

    // -- (префиксный)
    const_iterator& operator--() {
      if (current_ == nullptr) {
        current_ = root;
        while (current_->right_ != nullptr) {
          current_ = current_->right_;
        }
      } else {
        if (current_->left_ != nullptr) {
          current_ = current_->left_;
          while (current_->right_ != nullptr) {
            current_ = current_->right_;
          }
        } else {
          const Node* parent = current_->parent_;
          while (parent != nullptr && current_ == parent->left_) {
            current_ = parent;
            parent = parent->parent_;
          }
          current_ = parent;
        }
      }
      return *this;
    }

    // Сравнение
    bool operator==(const const_iterator& other) const {
      return current_ == other.current_;
    }

    bool operator!=(const const_iterator& other) const {
      return !(*this == other);
    }
  };

  iterator begin() {
    if (root_ == nullptr) {
      return end();  // Возвращаем итератор на "конец"
    }

    Node* start = root_;
    while (start->left_) {
      start = start->left_;
    }
    return iterator(start, root_);
  }

  iterator end() { return iterator(nullptr, root_); }

  // Версии для const-объекта
  const_iterator begin() const {
    if (root_ == nullptr) {
      return end();  // Возвращаем итератор на "конец"
    }
    Node* start = root_;
    while (start && start->left_) {
      start = start->left_;
    }
    return const_iterator(start, root_);
  }

  const_iterator end() const { return const_iterator(nullptr, root_); }

  /*----- Публичный insert -----*/
  std::pair<iterator, bool> insert(const T& value) {
    if (!root_) {
      root_ = new Node(value);
      root_->color_ = Color::BLACK;
      ++size_;
      return std::make_pair(iterator(root_, root_), true);
    } else {
      bool inserted_flag = false;
      // рекурсивно вставляем
      Node* inserted = insertNode(root_, nullptr, value, inserted_flag);

      // определяем, был ли элемент реально добавлен
      if (inserted_flag) {
        size_++;
        balance_set(inserted);
        return std::make_pair(iterator(root_, root_), true);
      } else {
        return std::make_pair(iterator(root_, root_), false);
      }
    }
  }

  /*----- публичный поиск ------*/
  iterator find(const T& key) {
    Node* current = root_;
    while (current != nullptr) {
      if (key == current->data_) {
        return iterator(current);
      }
      if (key > current->data_) {
        current = current->right_;
      } else if (key < current->data_) {
        current = current->left_;
      }
    }
    return this->end();  // Не найдено
  }

  bool contains(const T& value) const {
    Node* current = root_;
    while (current) {
      if (value == current->data_) return true;
      current = (value < current->data_) ? current->left_ : current->right_;
    }
    return false;
  }

  void merge(set& other) {
    for (auto it = other.begin(); it != other.end();) {
      auto next = it;
      ++next;
      auto res = insert(*it);  // вставка элемента из other в это множество
      if (res.second) {
        other.erase(it);  // если вставка успешна — удаляем элемент из other
      }
      it = next;
    }
  }

  void rebalanceAfterErase(Node*& current, Node*& Parent) {
    while (current != root_ && (!current || current->color_ == Color::BLACK)) {
      if (Parent == nullptr) {
        break;
      }
      Node* brother =
          (current == Parent->right_) ? Parent->left_ : Parent->right_;

      if (!brother) {
        if (current == nullptr || current == root_ || Parent == nullptr) {
          if (Parent->parent_) {
            Parent->parent_->color_ = Color::BLACK;
            break;
          }
        }
      }

      // Case 1: brother is red
      if (brother && brother->color_ == Color::RED) {
        brother->color_ = Color::BLACK;
        Parent->color_ = Color::RED;
        if (current == Parent->left_) {
          rotateLeft(Parent);
        } else {
          rotateRight(Parent);
        }
        Parent = Parent->parent_;
        brother = (current == Parent->right_) ? Parent->left_
                                              : Parent->right_;  // новый брат

        if (current == nullptr || current == root_ || Parent == nullptr) {
          break;
        }
      }
      // Case 2: brother and his children are black
      if ((brother && (brother->color_ == Color::BLACK)) &&
          (!brother->left_ || brother->left_->color_ == Color::BLACK) &&
          (!brother->right_ || brother->right_->color_ == Color::BLACK)) {
        brother->color_ = Color::RED;
        current = Parent;
        Parent = current->parent_;

        continue;
      }

      if (current == Parent->left_) {
        if (brother && (brother->color_ == Color::BLACK) && brother->left_ &&
            (brother->left_->color_ == Color::RED) &&
            (!brother->right_ || (brother->right_->color_ == Color::BLACK))) {
          brother->left_->color_ = Color::BLACK;
          brother->color_ = Color::RED;
          rotateRight(brother);
          brother = Parent->right_;
        }
      } else {
        if (brother && (brother->color_ == Color::BLACK) &&
            ((!brother->left_) || (brother->left_->color_ == Color::BLACK)) &&
            brother->right_ && (brother->right_->color_ == Color::RED)) {
          brother->right_->color_ = Color::BLACK;
          brother->color_ = Color::RED;
          rotateLeft(brother);
          brother = Parent->left_;
        }
      }

      if (current == Parent->left_) {
        if (brother && (brother->color_ == Color::BLACK) && brother->right_ &&
            (brother->right_->color_ == Color::RED)) {
          brother->color_ = Parent->color_;
          Parent->color_ = Color::BLACK;
          // Безопаснее обратиться к brother->right_ вместо Parent->right_
          brother->right_->color_ = Color::BLACK;
          rotateLeft(Parent);
          current = this->root_;
        }

      } else {
        if (brother && (brother->color_ == Color::BLACK) && brother->left_ &&
            (brother->left_->color_ == Color::RED)) {
          brother->color_ = Parent->color_;
          Parent->color_ = Color::BLACK;
          brother->left_->color_ = Color::BLACK;
          rotateRight(Parent);
          current = this->root_;
        }
      }
    }
    if (current) {
      current->color_ = Color::BLACK;
    }
  }

  void erase(iterator pos) {
    Node* curr = pos.current_;  // deleted
    if (!curr) {
      return;
    }
    bool isEraseNodeRed = (curr->color_ == Color::RED);
    Node* replacementNode = nullptr;  // займёт место удалённого
    Node* parentNode =
        nullptr;  // родитель replacementNode, нужен для балансировки.

    /*-- нет детей --*/
    if (!curr->left_ && !curr->right_) {
      if (curr->parent_) {
        if (curr->parent_->left_ == curr) {
          curr->parent_->left_ = nullptr;
        } else {
          curr->parent_->right_ = nullptr;
        }

        parentNode = curr->parent_;

      } else {
        this->root_ = nullptr;  // удаляем единственный узел (корень)
      }
      delete curr;
      if (!isEraseNodeRed) {
        rebalanceAfterErase(replacementNode, parentNode);
      }
      if (this->size_ > 0) {
        this->size_--;
      }
      return;
      // 2 случай 1 ребенок
    } else if (!curr->left_ || !curr->right_) {
      Node* child = curr->left_ ? curr->left_ : curr->right_;
      if (!curr->parent_) {
        this->root_ = child;
        child->parent_ = nullptr;
        replacementNode = child;  // Узел заменяется единственным ребёнком.
        parentNode = nullptr;
        delete curr;
      } else {
        if (curr == curr->parent_->left_) {
          curr->parent_->left_ = child;
        } else {
          curr->parent_->right_ = child;
        }
        child->parent_ = curr->parent_;
        replacementNode = child;
        parentNode = curr->parent_;
        delete curr;
      }
      if (!isEraseNodeRed) {
        rebalanceAfterErase(replacementNode, parentNode);
      }
      if (this->size_ > 0) {
        this->size_--;
      }
      return;
      /* 3 случай 2 ребенка*/
    } else {
      Node* temp = curr->right_;  // узел для правого поддерева

      /*-- минимальный в правом поддереве --*/
      while (temp->left_) {
        temp = temp->left_;
      }

      if (temp->parent_ == curr) {
        temp->parent_ = curr->parent_;

        if (!curr->parent_) {
          this->root_ = temp;

        } else if (curr == curr->parent_->left_) {
          curr->parent_->left_ = temp;
        } else {
          curr->parent_->right_ = temp;
        }
        temp->left_ = curr->left_;
        if (temp->left_) {
          temp->left_->parent_ = temp;
        }
        parentNode = temp;
        replacementNode = temp->right_;

      } else {
        if (temp == temp->parent_->left_) {
          temp->parent_->left_ = temp->right_;
        } else {
          temp->parent_->right_ = temp->right_;
        }
        if (temp->right_) {
          temp->right_->parent_ = temp->parent_;
        }
        parentNode = temp->parent_;
        replacementNode = temp->right_;
        temp->parent_ = curr->parent_;

        if (!curr->parent_) {
          this->root_ = temp;
        } else if (curr == curr->parent_->left_) {
          curr->parent_->left_ = temp;
        } else {
          curr->parent_->right_ = temp;
        }
        temp->left_ = curr->left_;
        temp->right_ = curr->right_;

        if (temp->left_) {
          temp->left_->parent_ = temp;
        }
        if (temp->right_) {
          temp->right_->parent_ = temp;
        }
      }
      isEraseNodeRed = (curr->color_ == Color::RED);
      delete curr;
      if (!isEraseNodeRed) {
        rebalanceAfterErase(replacementNode, parentNode);
      }
      if (this->size_ > 0) {
        this->size_--;
      }
    }
  }

  void rotateLeft(Node*& n) {
    if (!n || !n->right_) {
      return;
    }
    Node* rightChild = n->right_;
    n->right_ = rightChild->left_;

    if (rightChild->left_) {
      rightChild->left_->parent_ = n;
    }
    rightChild->parent_ = n->parent_;

    if (!n->parent_) {
      this->root_ = rightChild;
    } else if (n->parent_->left_ == n) {
      n->parent_->left_ = rightChild;
    } else {
      n->parent_->right_ = rightChild;
    }
    rightChild->left_ = n;
    n->parent_ = rightChild;
  }

  void rotateRight(Node*& n) {
    if (!n || !n->left_) {
      return;
    }
    Node* leftChild = n->left_;
    n->left_ = leftChild->right_;

    if (leftChild->right_) {
      leftChild->right_->parent_ = n;
    }
    leftChild->parent_ = n->parent_;

    if (!n->parent_) {
      this->root_ = leftChild;
    } else if (n->parent_->left_ == n) {
      n->parent_->left_ = leftChild;
    } else {
      n->parent_->right_ = leftChild;
    }
    leftChild->right_ = n;
    n->parent_ = leftChild;
  }

  set& operator=(set&& other) noexcept {
    if (this != &other) {
      clear();  // удалим свои текущие узлы

      root_ = other.root_;
      size_ = other.size_;

      other.root_ = nullptr;
      other.size_ = 0;
    }
    return *this;
  }

  set& operator=(const set& other) {
    if (this != &other) {
      clear();  // удалим текущие данные
      copyTree(other.root_);
    }
    return *this;
  }

  template <typename... Args>
  std::vector<std::pair<iterator, bool>> insert_many(Args&&... args) {
    static_assert(
        (std::is_convertible_v<Args, T> && ...),
        "insert_many: все аргументы должны быть типа T или приводимы к нему");
    static_assert(sizeof...(args) > 0,
                  "insert_many: должен быть хотя бы один аргумент");
    std::vector<std::pair<iterator, bool>> results;
    (results.emplace_back(this->insert(std::forward<Args>(args))), ...);
    return results;
  }
};
}  // namespace s21
