#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 16

typedef struct {
  char *message;
  int value;
} item_t;

item_t ring_buffer[BUF_SIZE];
volatile int head = 0, tail = 0, count = 0;

/*
** 버퍼 크기 - 1 만큼만 아이템을 채울 수 있다.
*/
void *produce(void *arg)
{
  item_t next_produced;

  while (1) {
    // 아이템 생산
    sprintf(next_produced.message, "%d", count);
    next_produced.value = rand() % 10;
    printf("producer: %s\n", next_produced.message);
  
    while ((head + 1) % BUF_SIZE == tail); // 버퍼가 가득 차있으면 더이상 생산할 수 없다.

    ring_buffer[head] = next_produced;
    head = (head + 1) % BUF_SIZE;
    count++;
  }

  return NULL;
}

void *consume(void *arg)
{
  item_t next_consumed;

  while (1) {
    while (head == tail); // 버퍼가 비어있으면 더이상 소비할 수 없다.

    // 아이템 소비
    next_consumed = ring_buffer[tail];
    tail = (tail + 1) % BUF_SIZE;
    count--;

    printf("consumer: %s\n", next_consumed.message);
  }

  return NULL;
}

int main()
{
  pthread_t producer, consumer;

  pthread_create(&producer, NULL, produce, NULL);
  pthread_create(&consumer, NULL, consume, NULL);

  pthread_join(producer, NULL);
  pthread_join(consumer, NULL);

  return 0;
}