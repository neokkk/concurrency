#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mut_1, mut_2;

void *do_work_1(void *arg)
{
    int done = 0;

    while (!done) {
        pthread_mutex_lock(&mut_1);
        if (pthread_mutex_trylock(&mut_2)) {
            printf("critical section 1\n");
            sleep(5);

            pthread_mutex_unlock(&mut_2);
            pthread_mutex_unlock(&mut_1);
            done = 1;
        } else {
            pthread_mutex_unlock(&mut_1);
        }
    }
    
    pthread_exit(0);
}

void *do_work_2(void *arg)
{
    int done = 0;

    while (!done) {
        pthread_mutex_lock(&mut_2);
        if (pthread_mutex_trylock(&mut_1)) {
            printf("critical section 2\n");
            sleep(5);

            pthread_mutex_unlock(&mut_1);
            pthread_mutex_unlock(&mut_2);
            done = 1;
        } else {
            pthread_mutex_unlock(&mut_2);
        }
    }

    pthread_exit(0);
}

int main()
{
    int ret;
    pthread_t t1, t2;

    pthread_mutex_init(&mut_1, NULL);
    pthread_mutex_init(&mut_2, NULL);
}