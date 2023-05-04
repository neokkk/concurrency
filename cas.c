#include <stdbool.h>
#include <stdint.h>

// not atomic
bool _compare_and_swap(uint64_t* p, uint64_t value, uint64_t new_value) {
  if (*p != value) return false;
  *p = new_value;
  return true;
}

bool _compare_and_swap(uint64_t* p, uint64_t value, uint64_t new_value) {
  return __sync_bool_compare_and_swap(p, value, new_value); // gcc, clang
}