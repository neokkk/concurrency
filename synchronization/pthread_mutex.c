#include <pthread.h>
#include <stdlib.h>

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

void* worker(void *arg) {
	if (pthread_mutex_lock(&mut) != 0) {
		perror("pthread_mutex_lock error");
		exit(-1);
	}
	
	// critical section
	
	if (pthread_mutex_unlock(&mut) != 0) {
		perror("pthread_mutex_unlock error");
		exit(-1);
	}

	return NULL;
}

int main() {
	pthread_t t1, t2;

	if (pthread_create(&t1, NULL, worker, NULL) != 0) {
		perror("t1 pthread_create error");
		return -1;
	}
	
	if (pthread_create(&t2, NULL, worker, NULL) != 0) {
		perror("p2 pthread_create error");
		return -1;
	}

	if (pthread_join(t1, NULL) != 0) {
		perror("p1 pthread_join error");
		return -1;
	}

	if (pthread_join(t2, NULL) != 0) {
		perror("p2 pthread_join error");
		return -1;
	}

	if (pthread_mutex_destroy(&mut) != 0) {
		perror("pthread_mutex_destroy error");
		return -1;
	}

	return 0;
}
