#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mut_1, mut_2;

void *do_work_1(void *arg)
{
    pthread_mutex_lock(&mut_1);
    pthread_mutex_lock(&mut_2);

    printf("critical section 1\n");
    sleep(5);

    pthread_mutex_unlock(&mut_2);
    pthread_mutex_unlock(&mut_1);

    pthread_exit(0);
}

void *do_work_2(void *arg)
{
    pthread_mutex_lock(&mut_2);
    pthread_mutex_lock(&mut_1);

    printf("critical section 2\n");
    sleep(5);

    pthread_mutex_unlock(&mut_1);
    pthread_mutex_unlock(&mut_2);

    pthread_exit(0);
}

int main()
{
    int ret;
    pthread_t t1, t2;

    pthread_mutex_init(&mut_1, NULL);
    pthread_mutex_init(&mut_2, NULL);

    ret = pthread_create(&t1, NULL, do_work_1, NULL);
    assert(ret == 0);
    ret = pthread_create(&t2, NULL, do_work_2, NULL);
    assert(ret == 0);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}