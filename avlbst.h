#ifndef AVL_BST_H
#define AVL_BST_H

#include "bst.h"
#include <algorithm>

template <typename Key, typename Value>
class AVLNode : public Node<Key, Value> {
public:
    AVLNode(const std::pair<const Key, Value>& pair, AVLNode* parent)
        : Node<Key, Value>(pair, parent), balance_(0) {}

    virtual ~AVLNode() {}

    int getBalance() const { return balance_; }
    void setBalance(int balance) { balance_ = balance; }
    void updateBalance(int diff) { balance_ += diff; }

    AVLNode* getParent() const { return static_cast<AVLNode*>(this->parent); }
    AVLNode* getLeft() const { return static_cast<AVLNode*>(this->left); }
    AVLNode* getRight() const { return static_cast<AVLNode*>(this->right); }

protected:
    int balance_;
};

template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value> {
public:
    void insert(const std::pair<const Key, Value>& keyValuePair) {
        if (this->root_ == NULL) {
            this->root_ = new AVLNode<Key, Value>(keyValuePair, NULL);
            return;
     }

        AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_); //current
        AVLNode<Key, Value>* parent = NULL;

          while (curr != NULL) {
              parent = curr;
              if (keyValuePair.first < curr->getKey()) {
                  curr = curr->getLeft();
              } else if (keyValuePair.first > curr->getKey()) {
                  curr = curr->getRight();
              } else {
                  curr->getValue() = keyValuePair.second;
                  return;
              }
          }

        AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(keyValuePair, parent);

        if (keyValuePair.first < parent->getKey()) {

            parent->left = newNode;
        } else {

            parent->right = newNode;
        }

        insertFix(parent, newNode);
    }

    void remove(const Key& key) {
        AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
        if (node == NULL) return;

        AVLNode<Key, Value>* parent = node->getParent();

        int diff = 0;

        if (node->getLeft() != NULL && node->getRight() != NULL) {
            Node<Key, Value>* pred = BinarySearchTree<Key, Value>::predecessor(node);
            this->swapNode(node, static_cast<AVLNode<Key, Value>*>(pred));
        }

        AVLNode<Key, Value>* child = (node->getLeft() != NULL) ? node->getLeft() : node->getRight();

        if (parent == NULL) {
            this->root_ = child;
            if (child != NULL) child->parent = NULL;
        } else {
            if (parent->getLeft() == node) {
                parent->left = child;
                diff = 1;
            } else {
                parent->right = child;
                diff = -1;
            }
            if (child != NULL) child->parent = parent;
        }

        delete node;
        removeFix(parent, diff);
    }
    void print() const {}

private:
    void rotateLeft(AVLNode<Key, Value>* x) {
        AVLNode<Key, Value>* y = x->getRight();
        x->right = y->getLeft();
        if (y->getLeft() != NULL) y->getLeft()->parent = x;

        y->parent = x->getParent();
        if (x->getParent() == NULL) {
            this->root_ = y;
        } else if (x == x->getParent()->getLeft()) {
            x->getParent()->left = y;
        } else {
            x->getParent()->right = y;
        }

        y->left = x;
        x->parent = y;

        x->setBalance(x->getBalance() + 1 - std::min(y->getBalance(), 0));
        
        y->setBalance(y->getBalance() + 1 + std::max(x->getBalance(), 0));
    }

    void rotateRight(AVLNode<Key, Value>* y) {
        AVLNode<Key, Value>* x = y->getLeft();
        y->left = x->getRight();
        if (x->getRight() != NULL) x->getRight()->parent = y;

        x->parent = y->getParent();
        if (y->getParent() == NULL) {
            this->root_ = x;
        } else if (y == y->getParent()->getLeft()) {
            y->getParent()->left = x;
        } else {

            y->getParent()->right = x;
        }

        x->right = y;
        y->parent = x;

        y->setBalance(y->getBalance() - 1 - std::max(x->getBalance(), 0));

        x->setBalance(x->getBalance() - 1 + std::min(y->getBalance(), 0));
    }

    void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* child) {//balance
        if (parent == NULL) return;

        if (child == parent->getLeft()) {
            parent->updateBalance(-1);

        } else {
            parent->updateBalance(1);
        }

        if (parent->getBalance() == 0) { //it is right
            return;
        } else if (abs(parent->getBalance()) == 2) { // unbalance
            rebalance(parent);
            return;
        } else {
            insertFix(parent->getParent(), parent);
        }
    }

    void removeFix(AVLNode<Key, Value>* node, int diff) {
        if (node == NULL) return;

        node->updateBalance(diff);

        if (node->getBalance() == -2) {
            AVLNode<Key, Value>* leftChild = node->getLeft();
            if (leftChild->getBalance() <= 0) {
                rotateRight(node);
                if (node->getParent()->getBalance() == 0) {
                    removeFix(node->getParent(), 1);
                }
            } else {
                rotateLeft(leftChild);
                rotateRight(node);
                removeFix(node->getParent(), 1);
            }
        } else if (node->getBalance() == 2) {
            AVLNode<Key, Value>* rightChild = node->getRight();
            if (rightChild->getBalance() >= 0) {
                rotateLeft(node);
                if (node->getParent()->getBalance() == 0) {
                    removeFix(node->getParent(), -1);
                }
            } else {
                rotateRight(rightChild);
                rotateLeft(node);
                removeFix(node->getParent(), -1);
            }
        } else if (node->getBalance() == 0) {
            if (node->getParent() != NULL) {
                if (node == node->getParent()->getLeft()) {
                    removeFix(node->getParent(), 1);
                } else {
                    removeFix(node->getParent(), -1);
                }
            }
        }
    }

    void rebalance(AVLNode<Key, Value>* node) {
        if (node->getBalance() == -2) {
            AVLNode<Key, Value>* leftChild = node->getLeft();
            if (leftChild->getBalance() <= 0) {
                rotateRight(node);
            } else {
                rotateLeft(leftChild);
                rotateRight(node);
            }
        } else if (node->getBalance() == 2) {
            AVLNode<Key, Value>* rightChild = node->getRight();
            if (rightChild->getBalance() >= 0) {
                rotateLeft(node);
            } else {
                rotateRight(rightChild);
                rotateLeft(node);
            }
        }
    }
};

#endif
