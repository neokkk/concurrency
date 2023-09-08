#include <stdbool.h>
#include <stdint.h>

// not atomic
bool _compare_and_swap(uint64_t* value, uint64_t expected, uint64_t new_value) {
  uint64_t old_value = *value;
  if (old_value == expected) {
    *value = new_value;
  }
  return old_value;
}

bool compare_and_swap(uint64_t* p, uint64_t value, uint64_t new_value) {
  return __sync_bool_compare_and_swap(p, value, new_value); // gcc, clang
}