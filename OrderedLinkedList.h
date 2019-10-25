// Copyright 2019 Rica Radu Leonard

#ifndef ORDEREDLINKEDLIST_H_
#define ORDEREDLINKEDLIST_H_
#include <iostream>
#include <utility>

template <typename K, typename V>
class OrderedLinkedList {
 private:
  class Node {
   public:
    K key;
    V value;
    Node *next;
    Node(K key, V value) : key(key), value(value) { next = nullptr; }
  };
  Node *head;

 public:
  OrderedLinkedList() { head = nullptr; }

  OrderedLinkedList(OrderedLinkedList &other) : OrderedLinkedList() {
    for (auto it : other) {
      this->insert(it.first, it.second);
    }
  }

  OrderedLinkedList &operator=(OrderedLinkedList other) {
    OrderedLinkedList *temp = this->head;
    this->head = other.head;
    other.head = temp;
    return *this;
  }

  ~OrderedLinkedList() {
    while (head) {
      Node *toDelete = head;
      head = head->next;
      delete toDelete;
    }
  }

  bool hasValue(V value) {
    for (auto it : (*this)) {
      if (it.second == value) {
        return 1;
      }
    }
    return 0;
  }

  bool comp(Node *a, Node *b) {
    if (a->key == b->key) {
      return a->value < b->value;
    }
    return a->key > b->key;
  }

  void insert(K key, V value) {
    Node *newNode = new Node(key, value);
    if (head == nullptr || comp(newNode, head)) {
      newNode->next = head;
      head = newNode;
    } else {
      Node *current = head;
      while (current->next && comp(current->next, newNode)) {
        current = current->next;
      }
      newNode->next = current->next;
      current->next = newNode;
    }
  }

  void removeByValue(V value) {
    if (head->value == value) {
      Node *toDelete = head;
      head = head->next;
      delete toDelete;
    } else {
      Node *current = head;
      while (current->next) {
        if (current->next->value == value) {
          Node *toDelete = current->next;
          current->next = current->next->next;
          delete toDelete;
          return;
        }
        current = current->next;
      }
    }
  }

  void update(K key, V value) {
    removeByValue(value);
    insert(key, value);
  }

  class Iterator {
   public:
    Iterator() noexcept : currentNode(head) {}

    explicit Iterator(const Node *node) noexcept : currentNode(node) {}

    Iterator &operator=(Node *other) {
      this->currentNode = other;
      return *this;
    }

    Iterator &operator++() {
      if (currentNode) currentNode = currentNode->next;
      return *this;
    }

    Iterator operator++(int) {
      Iterator iterator = *this;
      ++*this;
      return iterator;
    }

    bool operator!=(const Iterator &other) {
      return currentNode != other.currentNode;
    }

    std::pair<K, V> operator*() {
      return make_pair(currentNode->key, currentNode->value);
    }

   private:
    const Node *currentNode;
  };

  Iterator begin() { return Iterator(head); }
  Iterator end() { return Iterator(nullptr); }
};

#endif  // ORDEREDLINKEDLIST_H_
