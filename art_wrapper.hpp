#pragma once
#include "OptimisticLockCoupling/Tree.h"
#include "tree.h"
#include <iostream>
#include <map>

void loadKey(TID tid, Key &key) {
  // Store the key of the tuple into the key vector
  // Implementation is database specific
  auto tuple = reinterpret_cast<std::pair<string_key *, char *> *>(
      tid & ((1UL << 63) - 1));
  auto sk = tuple->first;
  key.set(sk->key, sk->length);
}

// used to define the interface of all benchmarking trees
template <class T, class P> class art_wrapper : public Tree<T, P> {
public:
  typedef std::pair<T, P> V;

  art_wrapper() { my_tree = new ART_OLC::Tree(loadKey); }

  void bulk_load(const V bulk_arr[], int num) {
    std::cout << "start get thread info" << std::endl;
    auto t = my_tree->getThreadInfo();
    for (int i = 0; i < num; ++i) {
      std::cout << "insert " << i << "key: ";
      printf("%s ------- with %d\n", bulk_arr[i].first->key,
             bulk_arr[i].first->length);
      Key key_;
      key_.set(bulk_arr[i].first->key, bulk_arr[i].first->length);
      auto val =
          new std::pair<string_key *, P>(bulk_arr[i].first, bulk_arr[i].second);
      std::cout << "ready to insert" << std::endl;
      my_tree->insert(key_, reinterpret_cast<TID>(val), t);
    }
  }

  bool insert(const T &key, const P &payload) {
    auto t = my_tree->getThreadInfo();
    if constexpr (std::is_pointer_v<T>) {
      Key key_;
      key_.set(key->key, key->length);
      auto val = new std::pair<string_key *, P>(key, payload);
      my_tree->insert(key_, reinterpret_cast<TID>(val), t);
      return true;
    } else {
      // LOG_FATAL("The key must be string key in HOT!");
      std::cout << "The key must be string key in HOT!" << std::endl;
      exit(-1);
    }
  }

  bool search(const T &key, P *payload) const {
    auto t = my_tree->getThreadInfo();
    if constexpr (std::is_pointer_v<T>) {
      Key key_;
      key_.set(key->key, key->length);
      auto val = my_tree->lookup(key_, t);
      if (val == 0) {
        return false;
      }
      auto tuple = reinterpret_cast<std::pair<string_key *, char *> *>(
          val & ((1UL << 63) - 1));
      *payload = tuple->second;
      return true;
    } else {
      // LOG_FATAL("The key must be string key in HOT!");
      std::cout << "The key must be string key in HOT!" << std::endl;
      exit(-1);
    }
  }

  // 0 means no erase, 1 means erase 1
  bool erase(const T &key) { return false; }

  bool update(const T &key, const P &payload) { return false; }

  void print_min_max() {}

  void get_depth_info() {}

  int range_scan_by_size(const T &key, uint32_t to_scan, V *&result = nullptr) {
    return 0;
  }

private:
  ART_OLC::Tree *my_tree; // Store the pointer to the tree instance
};