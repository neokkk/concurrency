#include <stdbool.h>

// not atomic
bool _test_and_set(bool* lock) {
  bool old_value = *lock;
  *lock = true;
  return old_value;
}

bool test_and_set(bool* lock) {
  return __sync_lock_test_and_set(lock, true); // gcc, clang
}

void tas_release(bool* lock) {
  return __sync_lock_release(lock);
}