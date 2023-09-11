#include <stdbool.h>
#include "spinlock.c"

struct reentlock {
  bool lock;
  int id; // current locking thread id
  int count;
};

void reentlock_acquire(struct reentlock *lock, int id)
{
  if (lock->lock && lock->id == id) {
    lock->count++;
  } else {
    spinlock_acquire(&lock->lock);
    lock->id = id;
    lock->count++;
  }
}

void reentlock_release(struct reentlock *lock)
{
  lock->count--;

  if (lock->count == 0) {
    lock->id = 0;
    spinlock_release(lock);
  }
}