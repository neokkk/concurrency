#include <stdbool.h>
#include <spinlock.c>

void rwlock_read_acquire(int* rcount, volatile int* wcount) {
	for (;;) {
		while (*wcount); // writer가 있으면 대기
		__sync_fetch_and_add(rcount, 1);
		if (*wcount == 0) break;
		__sync_fetch_and_sub(rcount, 1);
	}
}

void rwlock_read_release(int* rcount) {
	__sync_fetch_and_sub(rcount, 1);
}

void rwlock_write_acquire(bool* lock, volatile int* rcount, int* wcount) {
	__sync_fetch_and_add(wcount, 1);
	while (*rcount); // reader가 있으면 대기
	spinlock_acquire(lock); // 뮤텍스를 이용하므로 writer 수가 최대 1개임이 보장된다.
}

void rwlock_write_release(bool* lock, int* wcount) {
	spinlock_release(lock);
	__sync_fetch_and_sub(wcount, 1);
}
