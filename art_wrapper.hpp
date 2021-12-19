#pragma once
#include "tree_api.h"

#include "OptimisticLockCoupling/Tree.h"
#include <iostream>
#include <map>

class art_wrapper : public tree_api {
public:
  art_wrapper();
  virtual ~art_wrapper();

  virtual bool find(const char *key, size_t key_sz, char *value_out) override;
  virtual bool insert(const char *key, size_t key_sz, const char *value,
                      size_t value_sz) override;
  virtual bool update(const char *key, size_t key_sz, const char *value,
                      size_t value_sz) override;
  virtual bool remove(const char *key, size_t key_sz) override;
  virtual int scan(const char *key, size_t key_sz, int scan_sz,
                   char *&values_out) override;

private:
  ART_OLC::Tree *my_tree;
};

void loadKey(TID tid, Key &key) {
  // Store the key of the tuple into the key vector
  // Implementation is database specific
  auto tuple = reinterpret_cast<std::pair<const char *, uint64_t> *>(
      tid & ((1UL << 63) - 1));
  key.set(tuple->first, tuple->second);
}

art_wrapper::art_wrapper() { my_tree = new ART_OLC::Tree(loadKey); }

art_wrapper::~art_wrapper() {}

bool art_wrapper::insert(const char *key, size_t key_sz, const char *value,
                         size_t value_sz) {
  auto t = my_tree->getThreadInfo();
  Key key_;
  key_.set(key, key_sz);
  auto val = new std::pair<const char *, uint64_t>(key, key_sz);
  my_tree->insert(key_, reinterpret_cast<TID>(val), t);
  return true;
}

bool art_wrapper::find(const char *key, size_t key_sz, char *value_out) {
  auto t = my_tree->getThreadInfo();
  Key key_;
  key_.set(key, key_sz);
  auto val = my_tree->lookup(key_, t);
  if (val == 0) {
    return false;
  }
  *value_out = reinterpret_cast<TID>(val);
  return true;
}

bool art_wrapper::update(const char *key, size_t key_sz, const char *value,
                         size_t value_sz) {
  return true;
}

bool art_wrapper::remove(const char *key, size_t key_sz) { return true; }

int art_wrapper::scan(const char *key, size_t key_sz, int scan_sz,
                      char *&values_out) {
  return 0;
}
