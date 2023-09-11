#include <pthread.h>
#include <stdlib.h>

void barrier(volatile int *count, int max)
{
  __sync_fetch_and_add(count, 1);
  while (*count < max);
}