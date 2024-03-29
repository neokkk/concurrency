void wait(volatile int *s) {
	for (;;) {
		while (*s <= 0); // 사용 중인 자원이 최댓값 이상인 경우 대기
		__sync_fetch_and_sub(s, 1); // 여러 프로세스가 동시에 요청. 횟수 감소 후 다시 시도
	}
}

void signal(int *s) {
	__sync_fetch_and_add(s, 1); // V
}
