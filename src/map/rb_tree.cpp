// #ifndef __RB_TREE_H__
// #define __RB_TREE_H__

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

public:
    rbtree() : root_(nullptr) {}

    ~rbtree() {
        clearTree(this->root_);
    }

    Node* find(const Key& key) const {
        Node* current = this->root_;
        while (current && current->value.first != key) {
            if (key < current->value.first) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return current;
    }

    void erase(const Key& key) {
        Node* current = this->find(key);
        if (!current) {
            return;
        }

        bool isEraseNodeRed = current->red;
        Node* deletedNode = nullptr;
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
            deletedNode = current;
            isEraseNodeRed = current->red;
            delete current;
            if (!isEraseNodeRed) {
                rebalanceAfterErase(replacementNode, parentNode);
            }
            return;
        }

        if (!current->left && current->right) {
            // Case 2.1: deleted node has only right child
            if (!current->parent) {
                this->root_ = current->right;
                current->right->parent = nullptr;
                deletedNode = current;
                replacementNode = current->right;
                parentNode = nullptr;
                delete current;
            } else {
                current->right->parent = current->parent;
                if (current == current->parent->left) {
                    current->parent->left = current->right;
                } else {
                    current->parent->right = current->right;
                }
                deletedNode = current;
                replacementNode = current->right;
                parentNode = current->parent;
                delete current;
                
            }
            rebalanceAfterErase(replacementNode, parentNode);
        } else if (current->left && !current->right) {
            // Case 2.2: deleted node has only left child
            if (!current->parent) {
                this->root_ = current->left;
                current->left->parent = nullptr;
                deletedNode = current;
                replacementNode = current->left;
                parentNode = nullptr;
                delete current;
            } else {
                current->left->parent = current->parent;
                if (current == current->parent->left) {
                    current->parent->left = current->left;
                } else {
                    current->parent->right = current->left;
                }
                deletedNode = current;
                replacementNode = current->left;
                parentNode = current->parent;
                delete current;
            }
            rebalanceAfterErase(replacementNode, parentNode);
        } else {
            // Case 3: deleted node has both children :)
            Node* temp = current->right;
            while (temp->left) {
                temp = temp->left;
            }
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

            current->value.second = temp->value.second;
            isEraseNodeRed = temp->red;
            deletedNode = temp;
            parentNode = temp->parent;
            if (temp->parent == current) {
                current->right = temp->right;
                if (temp->right) {
                    temp->right->parent = current;
                }
            }
            delete temp;
            if (!isEraseNodeRed) {
                rebalanceAfterErase(replacementNode, parentNode);
            }
        }
    }

   
    Node* insert(const Key& key, const Value& value) {
        Node* current = this->root_;
        Node* parent = nullptr;

        
        while (current != nullptr) {
            parent = current;
            if (key < current->value.first) {
                current = current->left;
            } else if (key > current->value.first) {
                current = current->right;
            } else {
                // if key already in the tree - do nothing (std::map)
                return current;
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
        return newNode;
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

    void clearTree(Node* tree) {
        if (tree) {
            clearTree(tree->left);
            clearTree(tree->right);
            delete tree;
        }
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
                if (current && current->red) {
                    current->red = false;
                }
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
    struct iterator {
        Node* current;

        iterator(const rbtree& tree) : current(tree.root_) {
            while (current && current->left) {
                current = current->left;
            }
        }

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
    };
};

int main() {
   rbtree<int, int> tree;
   tree.insert(1, 0);
   tree.insert(2, 0);
   tree.insert(3, 0);
   tree.insert(4, 0);
   tree.insert(5, 0);
   tree.insert(6, 3);
   tree.insert(7, 0);
   tree.insert(8, 0);
   tree.insert(9, 0);
   tree.insert(10, 0);
   tree.print();
   tree.erase(5);
   tree.print();

   rbtree<int, int> tree2;


//    rbtree<int, int>::iterator it(tree);

//    for (int i = 0; i < 10; i++) {
//     printf("%d\n", it.current->value.first);
//     it.next();
//    }

//    rbtree<int, int> tree2;
//    tree2.insert(15, 1);
//    tree2.insert(14, 2);
//    tree2.insert(10, 3);
//    tree2.insert(9, 4); 
//    tree2.insert(19, 5);
//    tree2.insert(20, 6);
//    tree2.insert(21, 7);
//    tree2.print();

//    rbtree<int, int> tree3;
//    tree3.insert(1, 1);
//    tree3.insert(2, 2);
//    tree3.insert(3, 3);
//    tree3.insert(4, 4);
//    tree3.insert(5, 5);
//    tree3.print();

//    return 0;
}



// #endif
