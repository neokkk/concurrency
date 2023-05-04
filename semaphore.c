#define S_NUM 4

void semaphore_acquire(volatile int* count) {
	for (;;) {
		while (*count >= S_NUM); // 사용 중인 자원이 최댓값 이상인 경우 대기
		__sync_fetch_and_add(count, 1); // P
		if (*count < S_NUM) break; // 락 획득
		__sync_fetch_and_sub(count, 1); // 여러 프로세스가 동시에 요청. 횟수 감소 후 다시 시도
	}
}

void semaphore_release(int *count) {
	__sync_fetch_and_sub(count, 1); // V
}
