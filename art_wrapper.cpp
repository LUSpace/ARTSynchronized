#include "art_wrapper.hpp"

extern "C" void *create_tree(const tree_options_t &opt) {
  return reinterpret_cast<void *>(new art_wrapper());
}