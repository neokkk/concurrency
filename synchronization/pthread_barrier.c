#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

volatile int num = 0;

void barrier(volatile int *count, int max)
{
  pthread_mutex_lock(&mut);
  (*count)++;
  if (*count < max)
    pthread_cond_wait(&cond, &mut);
  else
    pthread_cond_broadcast(&cond);
  pthread_mutex_unlock(&mut);
}

void *worker(void *arg)
{
  barrier(&num, 10);

  printf("worker %d\n", (int)arg);

  return NULL;
}

int main()
{
  pthread_t t[12];

  for (int i = 0; i < 12; i++)
    pthread_create(&t[i], NULL, worker, i);

  for (int i = 0; i < 12; i++)
    pthread_join(t[i], NULL);

  return 0;
}