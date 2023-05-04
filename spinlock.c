#include <stdbool.h>
#include <tas.c>

void _spinlock_acquire(bool* lock) { // 개선 전
  while (test_and_set(&lock));
}

void spinlock_acquire(volatile bool* lock) { // 개선 후(TTAS)
  for (;;) {
    while (*lock); // 아토믹 명령의 속도 패널티가 크므로, 값 우선 비교를 통해 불필요한 호출을 줄임
    if (!test_and_set(lock)) break;
  }
}

void spinlock_release(bool* lock) {
  tas_release(lock);
}