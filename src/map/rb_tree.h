#ifndef __RB_TREE_H__
#define __RB_TREE_H__

#include <cstddef>
#include <utility>

template <typename Key, typename Value>
class rbtree {
 public:
  using value_type = std::pair<const Key, Value>;
  using size_type = size_t;
  class iterator;
  class const_iterator;

 private:
  struct Node {
    value_type value;
    Node* parent = nullptr;
    Node* left = nullptr;
    Node* right = nullptr;
    bool red = true;

    Node(const value_type& val)
        : value(val),
          parent(nullptr),
          left(nullptr),
          right(nullptr),
          red(true) {}

    Node(const value_type& val, Node* par)
        : value(val), parent(par), left(nullptr), right(nullptr), red(true) {}
  };

  friend class iterator;
  friend class const_iterator;

 public:
  rbtree() : root_(nullptr), size_(0) {}

  rbtree(const rbtree& tree) : rbtree() {
    for (auto it = tree.begin(); it != tree.end(); ++it) {
      this->insert(*it);
    }
  }

  rbtree(rbtree&& tree) noexcept : root_(tree.root_), size_(tree.size_) {
    tree.size_ = 0;
    tree.root_ = nullptr;
  }

  ~rbtree() {
    if (this->root_) {
      clearTree(this->root_);
      this->root_ = nullptr;
    }
    this->size_ = 0;
  }

  rbtree& operator=(const rbtree& tree) {
    if (this != &tree) {
      if (this->root_) {
        this->clearTree(this->root_);
      }
      this->root_ = nullptr;
      this->size_ = 0;
      for (auto it = tree.begin(); it != tree.end(); ++it) {
        this->insert(*it);
      }
    }
    return *this;
  }

  rbtree& operator=(rbtree&& tree) noexcept {
    if (this != &tree) {
      if (this->root_) {
        this->clearTree(this->root_);
      }
      this->size_ = tree.size_;
      this->root_ = tree.root_;
      tree.size_ = 0;
      tree.root_ = nullptr;
    }
    return *this;
  }

  iterator find(const Key& key) {
    return iterator(findNode<Node*>(this->root_, key), this->root_);
  }

  const_iterator find(const Key& key) const {
    return const_iterator(findNode<const Node*>(this->root_, key), this->root_);
  }

  iterator erase(iterator it) {
    iterator next = it;
    next++;
    eraseNode(it.current);
    return next;
  }

  const_iterator erase(const_iterator it) {
    iterator temp = iterator(it);
    iterator next = iterator(it);
    next++;
    eraseNode(temp.current);
    return next;
  }

  std::pair<iterator, bool> insert(const value_type& value) {
    return insertNode(value);
  }

  std::pair<iterator, bool> insert(const Key& key, const Value& value) {
    return insertNode(std::make_pair(key, value));
  }

  std::pair<iterator, bool> insert_or_assign(const Key& key,
                                             const Value& value) {
    return insert_or_assignNode(key, value);
  }

  size_t size() const { return this->size_; }

  size_t max_size() const {
    return std::numeric_limits<size_type>::max() / sizeof(value_type);
  }

  bool empty() const { return (this->size_ == 0); }

  iterator end() const { return iterator(nullptr, this->root_); }

  const_iterator cend() const { return const_iterator(nullptr, this->root_); }

  iterator begin() const {
    Node* node = this->root_;
    while (node && node->left) {
      node = node->left;
    }
    return iterator(node, this->root_);
  }

  const_iterator cbegin() const {
    Node* node = this->root_;
    while (node && node->left) {
      node = node->left;
    }
    return const_iterator(node, this->root_);
  }

  void clear() {
    this->clearTree(this->root_);
    this->root_ = nullptr;
    this->size_ = 0;
  }

 private:
  Node* root_;
  size_t size_;

  void clearTree(Node* tree) {
    if (tree) {
      clearTree(tree->left);
      clearTree(tree->right);
      delete tree;
    }
  }

  template <typename NodePtr>
  NodePtr findNode(NodePtr root, const Key& key) const {
    NodePtr current = root;
    while (current && current->value.first != key) {
      if (key < current->value.first) {
        current = current->left;
      } else {
        current = current->right;
      }
    }
    return current;
  }

  void eraseNode(Node* node) {
    Node* current = node;
    if (!current) {
      return;
    }

    bool isEraseNodeRed = current->red;
    Node* replacementNode = nullptr;
    Node* parentNode = nullptr;

    // Case 1: deleted node hasn't children
    if (!current->left && !current->right) {
      if (current->parent) {
        if (current->parent->left == current) {
          current->parent->left = nullptr;
        } else {
          current->parent->right = nullptr;
        }
        parentNode = current->parent;
      } else {
        this->root_ = nullptr;
      }
      delete current;
      if (!isEraseNodeRed) {
        rebalanceAfterErase(replacementNode, parentNode);
      }
      if (this->size_ > 0) {
        this->size_--;
      }
      return;
    } else if (!current->left || !current->right) {
      // Case 2: deleted node has only one child
      Node* child = current->left ? current->left : current->right;

      if (!current->parent) {
        this->root_ = child;
        child->parent = nullptr;
        replacementNode = child;
        parentNode = nullptr;
        delete current;
      } else {
        if (current == current->parent->left) {
          current->parent->left = child;
        } else {
          current->parent->right = child;
        }
        child->parent = current->parent;
        replacementNode = child;
        parentNode = current->parent;
        delete current;
      }
      if (!isEraseNodeRed) {
        rebalanceAfterErase(replacementNode, parentNode);
      }
      if (this->size_ > 0) {
        this->size_--;
      }
      return;
    } else {
      // Case 3: deleted node has both children
      Node* temp = current->right;
      while (temp->left) {
        temp = temp->left;
      }
      if (temp->parent == current) {
        temp->parent = current->parent;
        if (!current->parent) {
          this->root_ = temp;
        } else if (current == current->parent->left) {
          current->parent->left = temp;
        } else {
          current->parent->right = temp;
        }
        temp->left = current->left;
        if (temp->left) {
          temp->left->parent = temp;
        }
        parentNode = temp;
        replacementNode = temp->right;
      } else {
        if (temp == temp->parent->left) {
          temp->parent->left = temp->right;
        } else {
          temp->parent->right = temp->right;
        }
        if (temp->right) {
          temp->right->parent = temp->parent;
        }
        parentNode = temp->parent;
        replacementNode = temp->right;

        temp->parent = current->parent;

        if (!current->parent) {
          this->root_ = temp;
        } else if (current == current->parent->left) {
          current->parent->left = temp;
        } else {
          current->parent->right = temp;
        }

        temp->left = current->left;
        temp->right = current->right;
        if (temp->left) {
          temp->left->parent = temp;
        }
        if (temp->right) {
          temp->right->parent = temp;
        }
      }
      isEraseNodeRed = current->red;
      delete current;

      if (!isEraseNodeRed) {
        rebalanceAfterErase(replacementNode, parentNode);
      }
      if (this->size_ > 0) {
        this->size_--;
      }
    }
  }

  std::pair<iterator, bool> insertNode(const value_type& value) {
    Node* current = this->root_;
    Node* parent = nullptr;

    while (current != nullptr) {
      parent = current;
      if (value.first < current->value.first) {
        current = current->left;
      } else if (value.first > current->value.first) {
        current = current->right;
      } else {
        // if key already in the tree - do nothing (std::map)
        return {iterator(current), false};
      }
    }

    Node* newNode = new Node(value, parent);

    if (parent == nullptr) {
      this->root_ = newNode;
    } else if (value.first < parent->value.first) {
      parent->left = newNode;
    } else {
      parent->right = newNode;
    }
    rebalanceAfterInsert(newNode);
    this->size_++;
    return {iterator(newNode), true};
  }

  std::pair<iterator, bool> insert_or_assignNode(const Key& key,
                                                 const Value& value) {
    Node* current = this->root_;
    Node* parent = nullptr;

    while (current != nullptr) {
      parent = current;
      if (key < current->value.first) {
        current = current->left;
      } else if (key > current->value.first) {
        current = current->right;
      } else {
        // if key already in the tree - change value and return false
        current->value.second = value;
        return {iterator(current), false};
      }
    }

    Node* newNode = new Node({key, value}, parent);

    if (parent == nullptr) {
      this->root_ = newNode;
    } else if (key < parent->value.first) {
      parent->left = newNode;
    } else {
      parent->right = newNode;
    }
    rebalanceAfterInsert(newNode);
    this->size_++;
    // if we successfully insert node - return true
    return {iterator(newNode), true};
  }

  void rebalanceAfterInsert(Node* current) {
    while (current != this->root_ && current->parent && current->parent->red) {
      Node* parent = current->parent;
      Node* grandparent = parent->parent;

      if (grandparent == nullptr) {
        break;
      }
      Node* uncle = (parent == grandparent->left) ? grandparent->right
                                                  : grandparent->left;

      // Case 1: Red uncle — recoloring
      if (uncle && uncle->red) {
        uncle->red = false;
        parent->red = false;
        grandparent->red = true;
        current = grandparent;
      } else {
        // Uncle is black or null
        if (grandparent->left == parent) {
          // Case 2: Left-Right → rotateLeft on parent
          if (current == parent->right) {
            rotateLeft(parent);
            current = parent;
            parent = current->parent;
          }
          // Case 3: Left-Left → rotateRight on grandparent
          parent->red = false;
          grandparent->red = true;
          rotateRight(grandparent);
        } else {
          if (current == parent->left) {
            // Case 2: Right-Left → rotateRight on parent
            rotateRight(parent);
            current = parent;
            parent = current->parent;
          }
          // Case 3: Right-Right → rotateLeft on grandparent
          parent->red = false;
          grandparent->red = true;
          rotateLeft(grandparent);
        }
      }
    }
    this->root_->red = false;
  }

  void rebalanceAfterErase(Node* current, Node* parent) {
    while (current != this->root_ && (!current || !current->red)) {
      if (parent == nullptr) {
        break;
      }

      Node* brother = (current == parent->right) ? parent->left : parent->right;

      if (!brother) {
        if (current == nullptr || current == root_ || parent == nullptr) {
          if (parent->parent) {
            parent->parent->red = false;
            break;
          }
        }
      }

      // Case 1: brother is red
      if (brother && brother->red) {
        brother->red = false;
        parent->red = true;
        if (current == parent->left) {
          rotateLeft(parent);
        } else {
          rotateRight(parent);
        }
        brother = (current == parent->right) ? parent->left : parent->right;
        if (current == nullptr || current == root_ || parent == nullptr) {
          break;
        }
      }

      // Case 2: brother and his children are black
      if (brother && !brother->red && (!brother->left || !brother->left->red) &&
          (!brother->right || !brother->right->red)) {
        brother->red = true;
        current = parent;
        parent = current->parent;
        continue;
      }

      if (current == parent->left) {
        // Case 3.1: Brother is black and near child is red, but far child is
        // black and current is left child
        if (brother && !brother->red && brother->left && brother->left->red &&
            (!brother->right || !brother->right->red)) {
          brother->left->red = false;
          brother->red = true;
          rotateRight(brother);
          brother = parent->right;
        }
      } else {
        // Case 3.2: Brother is black and near child is red, but far child is
        // black and current is right child
        if (brother && !brother->red &&
            (!brother->left || !brother->left->red) && brother->right &&
            brother->right->red) {
          brother->right->red = false;
          brother->red = true;
          rotateLeft(brother);
          brother = parent->left;
        }
      }

      if (current == parent->left) {
        // Case 4.1: Brother is black and far child is red
        // and current is left child
        if (brother && !brother->red && brother->right && brother->right->red) {
          brother->red = parent->red;
          parent->red = false;
          brother->right->red = false;
          rotateLeft(parent);
          current = this->root_;
        }
      } else {
        // Case 4.2: Brother is black and far child is red
        // and current is right child
        if (brother && !brother->red && brother->left && brother->left->red) {
          brother->red = parent->red;
          parent->red = false;
          brother->left->red = false;
          rotateRight(parent);
          current = this->root_;
        }
      }
    }

    if (current) {
      current->red = false;
    }
  }

  void rotateLeft(Node* n) {
    if (!n || !n->right) {
      return;
    }
    Node* rightChild = n->right;
    n->right = rightChild->left;

    if (rightChild->left) {
      rightChild->left->parent = n;
    }
    rightChild->parent = n->parent;

    if (!n->parent) {
      this->root_ = rightChild;
    } else if (n->parent->left == n) {
      n->parent->left = rightChild;
    } else {
      n->parent->right = rightChild;
    }
    rightChild->left = n;
    n->parent = rightChild;
  }

  void rotateRight(Node* n) {
    if (!n || !n->left) {
      return;
    }
    Node* leftChild = n->left;
    n->left = leftChild->right;

    if (leftChild->right) {
      leftChild->right->parent = n;
    }
    leftChild->parent = n->parent;

    if (!n->parent) {
      this->root_ = leftChild;
    } else if (n->parent->left == n) {
      n->parent->left = leftChild;
    } else {
      n->parent->right = leftChild;
    }
    leftChild->right = n;
    n->parent = leftChild;
  }

 public:
  class iterator {
   public:
    Node* current;
    Node* tree_root;
    iterator() = default;

    iterator(const const_iterator& it)
        : current(const_cast<Node*>(it.current)),
          tree_root(const_cast<Node*>(it.tree_root)) {}

    iterator(Node* node) : current(node), tree_root(nullptr) {}

    iterator(Node* node, Node* root) : current(node), tree_root(root) {}

    iterator(const rbtree& tree) : current(tree.root_) {
      while (current && current->left) {
        current = current->left;
      }
    }

    value_type& operator*() const { return current->value; }

    value_type* operator->() const { return &(current->value); }

    iterator& operator++() {
      next();
      return *this;
    }

    iterator operator++(int) {
      iterator temp = *this;
      next();
      return temp;
    }

    iterator& operator--() {
      prev();
      return *this;
    }

    iterator operator--(int) {
      iterator temp = *this;
      prev();
      return temp;
    }

    bool operator==(const iterator& other) const {
      return (this->current == other.current);
    }

    bool operator!=(const iterator& other) const {
      return (this->current != other.current);
    }

   private:
    void next() {
      if (!current) {
        return;
      }
      if (current->right) {
        Node* next = current->right;
        while (next->left) {
          next = next->left;
        }
        current = next;
      } else {
        Node* parent = current->parent;
        while (parent && current == parent->right) {
          current = parent;
          parent = parent->parent;
        }
        current = parent;
      }
    }

    void prev() {
      if (!current) {
        if (!tree_root) {
          return;
        }
        current = tree_root;
        while (current && current->right) {
          current = current->right;
        }
        return;
      }
      if (current->left) {
        Node* next = current->left;
        while (next->right) {
          next = next->right;
        }
        current = next;
      } else {
        Node* parent = current->parent;
        while (parent && current == parent->left) {
          current = parent;
          parent = parent->parent;
        }
        current = parent;
      }
    }
  };

  class const_iterator {
   public:
    const Node* current;
    const Node* tree_root;
    const_iterator() = default;

    const_iterator(const iterator& it)
        : current(it.current), tree_root(it.tree_root) {}

    const_iterator(const Node* node, const Node* root)
        : current(node), tree_root(root) {}

    const_iterator(const rbtree& tree) : current(tree.root_) {
      while (current && current->left) {
        current = current->left;
      }
    }

    const value_type& operator*() const { return current->value; }

    const value_type* operator->() const { return &(current->value); }

    const_iterator& operator++() {
      next();
      return *this;
    }

    const_iterator operator++(int) {
      const_iterator temp = *this;
      next();
      return temp;
    }

    const_iterator& operator--() {
      prev();
      return *this;
    }

    const_iterator operator--(int) {
      const_iterator temp = *this;
      prev();
      return temp;
    }

    bool operator==(const const_iterator& other) const {
      return (this->current == other.current);
    }

    bool operator!=(const const_iterator& other) const {
      return (this->current != other.current);
    }

   private:
    void next() {
      if (!current) {
        return;
      }
      if (current->right) {
        const Node* next = current->right;
        while (next->left) {
          next = next->left;
        }
        current = next;
      } else {
        const Node* parent = current->parent;
        while (parent && current == parent->right) {
          current = parent;
          parent = parent->parent;
        }
        current = parent;
      }
    }

    void prev() {
      if (!current) {
        if (!tree_root) {
          return;
        }
        current = tree_root;
        while (current && current->right) {
          current = current->right;
        }
        return;
      }
      if (current->left) {
        const Node* next = current->left;
        while (next->right) {
          next = next->right;
        }
        current = next;
      } else {
        const Node* parent = current->parent;
        while (parent && current == parent->left) {
          current = parent;
          parent = parent->parent;
        }
        current = parent;
      }
    }
  };
};

#endif
