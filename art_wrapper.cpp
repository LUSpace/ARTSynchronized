#include "art_wrapper.hpp"

extern "C" tree_api *create_tree(const tree_options_t &opt) {
  return new art_wrapper();
}