#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "reentrantlock.c"

#define THREAD_NUM 4

struct reentlock lock;

void reentlock_test(int id, int n)
{
  if (n == 0) return;

  reentlock_acquire(&lock, id);
  printf("test %d - %d\n", id, n);
  reentlock_test(id, n - 1);
  reentlock_release(&lock);
}

void *worker(void *arg)
{
  int id = (int)arg;
  assert(id != 0);
  for (int i = 0; i < 10000; i++)
    reentlock_test(i, 10);
}

int main(int argc, char **argv)
{
  int i;
  pthread_t t[THREAD_NUM];

  for (i = 0; i < THREAD_NUM; i++)
    pthread_create(&t[i], NULL, worker, i + 1);
  for (i = 0; i < THREAD_NUM; i++)
    pthread_join(t[i], NULL);

  return 0;
}