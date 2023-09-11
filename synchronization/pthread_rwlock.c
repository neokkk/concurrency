#define _GNU_SOURCE

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

void *reader(void *arg)
{
  pthread_rwlock_rdlock(&rwlock);
  printf("reader %d\n", (int)arg);
  pthread_rwlock_unlock(&rwlock);

  return NULL;
}

void *writer(void *arg)
{
  pthread_rwlock_wrlock(&rwlock);
  printf("writer %d\n", (int)arg);
  pthread_rwlock_unlock(&rwlock);

  return NULL;
}

int main()
{
  int i;
  pthread_t rds[10], wrs[2];

  for (i = 0; i < 5; i++)
    pthread_create(&rds[i], NULL, reader, i);
  
  for (i = 0; i < 2; i++)
    pthread_create(&wrs[i], NULL, writer, i);

  for (i = 5; i < 10; i++)
    pthread_create(&rds[i], NULL, reader, i);

  for (i = 0; i < 10; i++)
    pthread_join(rds[i], NULL);

  for (i = 0; i < 2; i++)
    pthread_join(wrs[i], NULL);

  pthread_rwlock_destroy(&rwlock);

  return 0;
}