#include <stdbool.h>

// not atomic
bool _test_and_set(bool* lock) {
  if (*lock) return true;
  *lock = true;
  return false;
}

bool test_and_set(bool* lock) {
  return __sync_lock_test_and_set(lock, true); // gcc, clang
}

void tas_release(bool* lock) {
  return __sync_lock_release(lock);
}