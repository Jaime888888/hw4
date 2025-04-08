#ifndef BST_H
#define BST_H

#include <iostream>
#include <utility>
#include <cstddef>
#include <functional>

template <typename Key, typename Value>
class Node {
public:
    Node(const std::pair<const Key, Value>& keyValuePair, Node* parent)
        : item_(keyValuePair), parent(parent), left(NULL), right(NULL) {}

    const std::pair<const Key, Value>& getItem() const { return item_; }
    std::pair<const Key, Value>& getItem() { return item_; }
    const Key& getKey() const { return item_.first; }
    const Value& getValue() const { return item_.second; }
    Value& getValue() { return item_.second; }
    Node* getParent() const { return parent; }
    Node*& getParent() { return parent; }
    Node* getLeft() const { return left; }
    Node*& getLeft() { return left; }
    Node* getRight() const { return right; }
    Node*& getRight() { return right; }

protected:
    std::pair<const Key, Value> item_;
    Node* parent;
    Node* left;
    Node* right;
};

template <typename Key, typename Value>
class BinarySearchTree {
public:
    typedef Node<Key, Value> NodeType;

    BinarySearchTree() : root_(NULL) {}
    ~BinarySearchTree() { clear(); }

    void clear() { clearHelper(root_); root_ = NULL; }

    void insert(const std::pair<const Key, Value>& keyValuePair) {
        if (root_ == NULL) {
            root_ = new NodeType(keyValuePair, NULL);
            return;
        }
        NodeType* curr = root_;
        NodeType* parent = NULL;

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

        NodeType* newNode = new NodeType(keyValuePair, parent);
        if (keyValuePair.first < parent->getKey()) {
            parent->getLeft() = newNode;
        } else {
            parent->getRight() = newNode;
        }
    }

    void remove(const Key& key) {
        NodeType* target = internalFind(key);
        if (target == NULL) return;

        if (target->getLeft() != NULL && target->getRight() != NULL) {
            NodeType* pred = predecessor(target);
            swapNode(target, pred);
        }

        NodeType* child = (target->getLeft() != NULL) ? target->getLeft() : target->getRight();

        if (target->getParent() == NULL) {
            root_ = child;
            if (child != NULL) child->getParent() = NULL;
        } else {
            if (target == target->getParent()->getLeft()) {
                target->getParent()->getLeft() = child;
            } else {
                target->getParent()->getRight() = child;
            }
            if (child != NULL) child->getParent() = target->getParent();
        }

        delete target;
    }

    NodeType* internalFind(const Key& key) const {
        NodeType* curr = root_;
        while (curr != NULL) {
            if (key < curr->getKey()) curr = curr->getLeft();
            else if (key > curr->getKey()) curr = curr->getRight();
            else return curr;
        }
        return NULL;
    }

    NodeType* getSmallestNode() const {
        NodeType* curr = root_;
        if (curr == NULL) return NULL;
        while (curr->getLeft() != NULL) curr = curr->getLeft();
        return curr;
    }

    bool isBalanced() const {
        return isBalancedHelper(root_).second;
    }

    void print() const { }

protected:
    NodeType* root_;

    void clearHelper(NodeType* node) {
        if (node == NULL) return;
        clearHelper(node->getLeft());
        clearHelper(node->getRight());
        delete node;
    }

    static NodeType* predecessor(NodeType* node) {
        if (node == NULL) return NULL;
        if (node->getLeft() != NULL) {
            node = node->getLeft();
            while (node->getRight() != NULL) node = node->getRight();
            return node;
        }
        NodeType* parent = node->getParent();
        while (parent != NULL && node == parent->getLeft()) {
            node = parent;
            parent = parent->getParent();
        }
        return parent;
    }

    static NodeType* successor(NodeType* node) {
        if (node == NULL) return NULL;
        if (node->getRight() != NULL) {
            node = node->getRight();
            while (node->getLeft() != NULL) node = node->getLeft();
            return node;
        }
        NodeType* parent = node->getParent();
        while (parent != NULL && node == parent->getRight()) {
            node = parent;
            parent = parent->getParent();
        }
        return parent;
    }

    std::pair<int, bool> isBalancedHelper(NodeType* node) const {
        if (node == NULL) return std::make_pair(0, true);

        auto left = isBalancedHelper(node->getLeft());
        auto right = isBalancedHelper(node->getRight());

        bool balanced = left.second && right.second && abs(left.first - right.first) <= 1;
        return std::make_pair(std::max(left.first, right.first) + 1, balanced);
    }

    void swapNode(NodeType* a, NodeType* b) {
        if (a == b || a == NULL || b == NULL) return;

        NodeType* aParent = a->getParent();
        NodeType* bParent = b->getParent();

        bool aIsLeft = (aParent != NULL && aParent->getLeft() == a);
        bool bIsLeft = (bParent != NULL && bParent->getLeft() == b);

        if (aParent == b) aParent = a;
        if (bParent == a) bParent = b;

        std::swap(a->parent, b->parent);

        if (aParent != NULL) (aIsLeft ? aParent->getLeft() : aParent->getRight()) = b;
        else root_ = b;

        if (bParent != NULL) (bIsLeft ? bParent->getLeft() : bParent->getRight()) = a;
        else root_ = a;

        std::swap(a->left, b->left);
        if (a->getLeft() != NULL) a->getLeft()->parent = a;
        if (b->getLeft() != NULL) b->getLeft()->parent = b;

        std::swap(a->right, b->right);
        if (a->getRight() != NULL) a->getRight()->parent = a;
        if (b->getRight() != NULL) b->getRight()->parent = b;

        std::swap(const_cast<Value&>(a->getItem().second), const_cast<Value&>(b->getItem().second));
    }

public:
    class iterator {
    public:
        iterator(NodeType* ptr = NULL) : curr(ptr) {}
        std::pair<const Key, Value>& operator*() const { return curr->getItem(); }
        std::pair<const Key, Value>* operator->() const { return &(curr->getItem()); }

        bool operator==(const iterator& rhs) const { return curr == rhs.curr; }
        bool operator!=(const iterator& rhs) const { return curr != rhs.curr; }

        iterator& operator++() {
            curr = successor(curr);
            return *this;
        }

    protected:
        NodeType* curr;

        friend class BinarySearchTree<Key, Value>;
    };

    iterator begin() const { return iterator(getSmallestNode()); }
    iterator end() const { return iterator(NULL); }

    iterator find(const Key& key) const {
        return iterator(internalFind(key));
    }
};

#endif
