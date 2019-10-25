// Copyright 2019 Rica Radu Leonard

#ifndef HASHMAP_H_
#define HASHMAP_H_
#include <functional>
#include <list>
#include <string>
#include <utility>
#include <vector>

template <typename K, typename V, typename Hash = std::hash<K>>
class HashMap {
 private:
  class HashNode {
   public:
    K key;
    V value;
    bool status;
    HashNode() : status(0) {}
    HashNode(K key, V value) : key(key), value(value), status(1) {}
  };

  Hash hash;
  size_t capacity;
  size_t size;
  std::vector<HashNode> nodes;

 public:
  HashMap() : size(0) {
    capacity = 1 << 15;
    nodes.resize(capacity);
  }

  explicit HashMap(size_t capacity) : capacity(capacity), size(0) {
    nodes.resize(capacity);
  }

  bool isEmpty() const { return (size == 0); }
  size_t getSize() { return size; }

  void put(K key, V value) {
    int index = hash(key) & (capacity - 1);
    while (nodes[index].status && nodes[index].key != key) {
      index++;
      index &= (capacity - 1);
    }
    if (!nodes[index].status) {
      size++;
      nodes[index] = HashNode(key, value);
    } else if (nodes[index].key == key) {
      nodes[index].value = value;
    }
  }

  V get(K key) const {
    int index = hash(key) & (capacity - 1);
    while (nodes[index].status && nodes[index].key != key) {
      index++;
      index &= (capacity - 1);
    }
    return nodes[index].value;
  }

  void remove(K key) {
    int index = hash(key) & (capacity - 1);
    while (nodes[index].status && nodes[index].key != key) {
      index++;
      index &= (capacity - 1);
    }
    if (nodes[index].key) {
      nodes[index] = HashNode();
      size--;
    }
  }

  V& operator[](K key) {
    int index = hash(key) & (capacity - 1);
    while (nodes[index].status && nodes[index].key != key) {
      index++;
      index &= (capacity - 1);
    }
    if (!nodes[index].status) {
      size++;
      nodes[index] = HashNode(key, 0);
    }
    return nodes[index].value;
  }
  V operator[](K key) const { return get(key); }
};

#endif  // HASHMAP_H_
