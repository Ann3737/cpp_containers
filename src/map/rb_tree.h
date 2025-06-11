#ifndef __RB_TREE_H__
#define __RB_TREE_H__

#include <utility>
#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cmath>

template <typename Key, typename Value>
class rbtree {
public:
    using value_type = std::pair<const Key, Value>;
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
                : value(val), parent(nullptr), left(nullptr)
                , right(nullptr), red(true) {}

        Node(const value_type& val, Node* par)
                : value(val), parent(par), left(nullptr)
                , right(nullptr), red(true) {}
    };

    friend class iterator;
    friend class const_iterator;

public:
    rbtree() : root_(nullptr), size_(0) {}

    ~rbtree() {
        clearTree(this->root_);
        this->root_ = nullptr;
        this->size_ = 0;
    }

    iterator find(const Key& key) {
        return iterator(findNode<Node*>(this->root_, key));
    }

    const_iterator find(const Key& key) const {
        return const_iterator(findNode<const Node*>(this->root_, key));
    }

    template<typename Iter>
    iterator erase(Iter it) {
        Node* temp = it.current;
        it++;
        eraseNode(temp);
        return iterator(it.current);
    }

    size_t size() const {
        return this->size_;
    }

    bool empty() const {
        return (this->size_ == 0);
    }

    iterator end() const {
        return iterator(nullptr);
    }

    const_iterator end() const {
        return const_iterator(nullptr);
    }

    iterator begin() const {
        Node* node = this->root_;
        while (node && node->left) {
            node = node->left;
        }
        return iterator(node);
    }

    const_iterator begin() const {
        Node* node = this->root_;
        while (node && node->left) {
            node = node->left;
        }
        return const_iterator(node);
    }

    void print() const {
        printFancyHelper(root_, "", false, true);
    }

    void printFancyHelper(Node* node, const std::string& prefix, bool isLeft, bool isRoot) const {
        if (!node) return;

        if (node->right)
            printFancyHelper(node->right, prefix + (isRoot ? "    " : (isLeft ? "│   " : "    ")), false, false);

        std::cout << prefix;
        if (!isRoot)
            std::cout << (isLeft ? "└── " : "┌── ");
        std::cout << "(" << node->value.first << "," << node->value.second << ")" << (node->red ? "(R)" : "(B)") << "\n";

        if (node->left)
            printFancyHelper(node->left, prefix + (isRoot ? "    " : (isLeft ? "    " : "│   ")), true, false);
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

    template<typename NodePtr>
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
                current->right = temp->right;
                if (temp->right) {
                    temp->right->parent = current;
                }
                parentNode = current;
                replacementNode = temp->right;
            } else {
                if (temp == temp->parent->left) {
                    temp->parent->left = temp->right;
                } else {
                    temp->parent->right = temp->right;
                }
                if (temp->right) {
                    replacementNode = temp->right;
                    temp->right->parent = temp->parent;
                } else {
                    replacementNode = nullptr;
                }
                parentNode = temp->parent;
            }

            current->value.second = temp->value.second;
            isEraseNodeRed = temp->red;
            delete temp;
            if (!isEraseNodeRed) {
                rebalanceAfterErase(replacementNode, parentNode);
            }
            if (this->size_ > 0) {
                this->size_--;
            }
        }
    }

    template<typename NodePtr>
    std::pair<NodePtr, bool> insert(const Key& key, const Value& value) {
        NodePtr current = this->root_;
        NodePtr parent = nullptr;
        
        while (current != nullptr) {
            parent = current;
            if (key < current->value.first) {
                current = current->left;
            } else if (key > current->value.first) {
                current = current->right;
            } else {
                // if key already in the tree - do nothing (std::map)
                return {current, false};
            }
        }

        NodePtr newNode = new Node({key, value}, parent);
        
        if (parent == nullptr) {
            this->root_ = newNode;
        } else if (key < parent->value.first) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }
        rebalanceAfterInsert(newNode);
        this->size_++;
        return {newNode, true};
    }

    void rebalanceAfterInsert(Node* current) {
        while (current != this->root_ && current->parent && current->parent->red) {
            Node* parent = current->parent;
            Node* grandparent = parent->parent;

            if (grandparent == nullptr) {
                break;
            }            
            Node* uncle = (parent == grandparent->left) ? grandparent->right : grandparent->left;

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

            Node* brother = (current == parent->right) ?
                parent->left : parent->right;
            
            // Case 1: brother is red
            if (brother && brother->red) {
                brother->red = false;
                parent->red = true;
                if (current == parent->left) {
                    rotateLeft(parent);
                } else {
                    rotateRight(parent);
                }
                brother = (current == parent->right) ?
                        parent->left : parent->right;
            }

            // Case 2: brother and his children are black
            if (brother && !brother->red && (!brother->left || !brother->left->red)
                    && (!brother->right || !brother->right->red)) {
                brother->red = true;
                current = parent;
                parent = current->parent;
                // if (current && current->red) {
                //     current->red = false;
                // }
                continue;
            }

            if (current == parent->left) {
                // Case 3.1: Brother is black and near child is red, but far child is black
                // and current is left child
                if (brother && !brother->red && brother->left && brother->left->red
                    && (!brother->right || !brother->right->red)) {
                    brother->left->red = false;
                    brother->red = true;
                    rotateRight(brother);
                    brother = parent->right;
                }
            } else {
                // Case 3.2: Brother is black and near child is red, but far child is black
                // and current is right child
                if (brother && !brother->red && (!brother->left || !brother->left->red) 
                        && brother->right && brother->right->red) {
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
        Node* current;
    public:
        iterator() = default;
        
        iterator(Node* node) : current(node) {}

        iterator(const Node* node) : current(node) {}

        iterator(const rbtree& tree) : current(tree.root_) {
            while (current && current->left) {
                current = current->left;
            }
        }

        value_type& operator*() const {
            return current->value;
        }

        value_type* operator->() const {
            return &(current->value);
        }

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
            return (this->current == other->current);
        }

        bool operator!=(const iterator& other) const {
            return (this->current != other->current);
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
                return;
            }
            if (current->left) {
                Node* next = current->left;
                while(next->right) {
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
        Node* current;
    public:
        const_iterator() = default;
        
        const_iterator(const rbtree& tree) : current(tree.root_) {
            while (current && current->left) {
                current = current->left;
            }
        }

        const value_type& operator*() const {
            return current->value;
        }

        const value_type* operator->() const {
            return &(current->value);
        }

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
            return (this->current == other->current);
        }

        bool operator!=(const const_iterator& other) const {
            return (this->current != other->current);
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
                return;
            }
            if (current->left) {
                Node* next = current->left;
                while(next->right) {
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
};

#endif
