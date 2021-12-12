#include "art_wrapper.hpp"

extern "C" void *create_tree() {
  return reinterpret_cast<void *>(new art_wrapper<string_key *, char *>());
}