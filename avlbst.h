#ifndef AVL_BST_H
#define AVL_BST_H

#include "bst.h"
#include <algorithm>

template <typename Key, typename Value>
class AVLNode : public Node<Key, Value> {
public:
    AVLNode(const std::pair<const Key, Value>& pair, AVLNode* parent)
        : Node<Key, Value>(pair, parent), balance_(0) {}

    

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

virtual ~AVLTree() {
    
    clear_avl();
    }
    void insert(const std::pair<const Key, Value>& keyValuePair) {
    std::cout << "Inserting key: " << keyValuePair.first << ", value: " << keyValuePair.second << std::endl;

    if (this->root_ == NULL) {
        std::cout << "nserting at root." << std::endl;
        this->root_ = new AVLNode<Key, Value>(keyValuePair, NULL);
        return;
    }

    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);

    AVLNode<Key, Value>* parent = NULL;

    while (curr != NULL) {
        parent = curr;

        if (keyValuePair.first < curr->getKey()) {
            std::cout << "going left from key: " << curr->getKey() << std::endl;
            curr = curr->getLeft();
        } else if (keyValuePair.first > curr->getKey()) {
            std::cout << "going right from key: " << curr->getKey() << std::endl;
            curr = curr->getRight();
        } else {
            std::cout << "key exists " << std::endl;
            curr->getValue() = keyValuePair.second;
            return;
        }
    }

    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(keyValuePair, parent);

    if (keyValuePair.first < parent->getKey()) {
        std::cout << "inserting as left child: " << parent->getKey() << std::endl;
        parent->left = newNode;
    } else {
        std::cout << "inserting as right chld of: " << parent->getKey() << std::endl;
        parent->right = newNode;
    }

    std::cout << "calling insertfix on parnt: " << parent->getKey() << ", new node: " << newNode->getKey() << std::endl;
    insertFix(parent, newNode);
}


void remove(const Key& key) {
    std::cout << "---- REMOVE FUNCTION START ----" << std::endl;
    std::cout << "Removing key: " << key << std::endl;

    AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if (node == NULL) {
        std::cout << "Key not found: " << key << std::endl;
        return;
    }

    AVLNode<Key, Value>* parent = node->getParent();
    int diff = 0;

    if (node->getLeft() != NULL && node->getRight() != NULL) {
        Node<Key, Value>* pred = BinarySearchTree<Key, Value>::predecessor(node);
        std::cout << "Node has two children. Swapping with predecessor: " << pred->getKey() << std::endl;

        // ✅ Step 1: Swap the internal data (not the pointers!)
        this->swapNode(node, static_cast<AVLNode<Key, Value>*>(pred));

        // ✅ Step 2: Save pred's key *before* deletion
        Key predKey = pred->getKey();

        // ✅ Step 3: Recursively remove by key
        std::cout << "Recursively removing swapped node: " << predKey << std::endl;
        remove(predKey);
        return;
    }

    AVLNode<Key, Value>* child = (node->getLeft() != NULL) ? node->getLeft() : node->getRight();

    if (parent == NULL) {
        this->root_ = child;
        if (child != NULL) {
            child->parent = NULL;
            std::cout << "Updated child as new root: " << child->getKey() << std::endl;
        } else {
            std::cout << "Tree is now empty after deletion." << std::endl;
        }
    } else {
        if (parent->getLeft() == node) {
            parent->left = child;
            diff = 1;
            std::cout << "Removed left child: " << node->getKey() << std::endl;
        } else {
            parent->right = child;
            diff = -1;
            std::cout << "Removed right child: " << node->getKey() << std::endl;
        }
        if (child != NULL) {
            child->parent = parent;
            std::cout << "Updated parent of child: " << child->getKey() << " to " << parent->getKey() << std::endl;
        }
    }

    std::cout << "Deleting node with key: " << node->getKey() << std::endl;
    delete node;

    removeFix(parent, diff);

    std::cout << "---- REMOVE FUNCTION END ----" << std::endl;
}



    void print() const {} 

    
   
void clear_avl() {
    std::cout << "calling AVL clear" << std::endl;
    clearHelper_avl(static_cast<AVLNode<Key, Value>*>(this->root_));
    this->root_ = nullptr;
}




private:
              void rotateLeft(AVLNode<Key, Value>* x) {  // left
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

                  x->setBalance( 0);
                  y->setBalance( 0);
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

        y->setBalance(0);
        x->setBalance(0);
    }

void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* child) {
        if (parent == NULL) return;

        if (child == parent->getLeft()) {
          std::cout << "parent update to -1" << std::endl;
            parent->updateBalance(-1);
        } else {
          std:: cout << "parent updated to 1" << std::endl;
            parent->updateBalance(1);
        }

        std::cout << parent->getBalance() << std::endl;
        if (parent->getBalance() == 0) {
            return;
        } else if (abs(parent->getBalance()) == 2) {
            rebalance(parent);
            std::cout << "rebalanced on " << parent->getKey() << std::endl;
            std::cout << "now the parent(" << parent ->getKey() << ")->getBalance is" << parent->getBalance() << std::endl;
            return;
        } else {
          std:: cout << "111" << std::endl;
          if (parent!= this->root_){
            std:: cout << "new insertFix on " << parent->getParent()->getKey() << " child:" << parent->getKey() << std::endl;
            insertFix(parent->getParent(), parent);
          }
          else 
          {
            std:: cout << "parent is root_" << std::endl;
          }
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

  








    void clearHelper_avl(AVLNode<Key, Value>* node) {
    if (node == nullptr) return;

    clearHelper_avl(node->getLeft());
    clearHelper_avl(node->getRight());

    // std::cout << "deleting node in teh avl with key : " << node->getKey() << std::endl;
    delete node;
}
};

#endif
