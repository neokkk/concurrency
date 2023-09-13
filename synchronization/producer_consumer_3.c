#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 16

typedef struct {
  char *message;
  int value;
} item_t;

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

volatile bool ready = false;
volatile int head = 0, tail = 0, count = 0;
item_t ring_buffer[BUF_SIZE];

void *produce(void *arg)
{
  item_t next_produced;

  while (1) {
    sprintf(next_produced.message, "%d", count);
    next_produced.value = rand() % 10;
    pthread_mutex_lock(&mut);
    if ((head + 1) % BUF_SIZE == tail) {
      goto err;
    }
    ring_buffer[head] = next_produced;
    head = (head + 1) % BUF_SIZE;
    count++;
    printf("producer: %s\n", next_produced.message);
    ready = true;
    pthread_mutex_unlock(&mut);
    pthread_cond_signal(&cond);
  }

  return NULL;

err:
  pthread_mutex_unlock(&mut);
  pthread_cond_broadcast(&cond);
  return NULL;
}

void *consume(void *arg)
{
  item_t next_consumed;

  while (1) {
    pthread_mutex_lock(&mut);
    while (!ready) {
      pthread_cond_wait(&cond, &mut);
    }
    if (head == tail) {
      continue;
    }
    next_consumed = ring_buffer[tail];
    tail = (tail + 1) % BUF_SIZE;
    count--;
    printf("consumer: %s\n", next_consumed.message);
    pthread_mutex_unlock(&mut);
  }

  return NULL;

err:
  pthread_mutex_unlock(&mut);
  pthread_cond_broadcast(&cond);
  return NULL;
}

int main()
{
  pthread_t producer, consumer;

  pthread_create(&producer, NULL, produce, NULL);
  pthread_create(&consumer, NULL, consume, NULL);

  pthread_join(producer, NULL);
  pthread_join(consumer, NULL);

  pthread_mutex_destroy(&mut);
  pthread_cond_destroy(&cond);

  return 0;
}