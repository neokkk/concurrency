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
int in = 0, out = 0, count = 0;

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
  
    while ((in + 1) % BUF_SIZE == out); // 버퍼가 가득 차있으면 더이상 생산할 수 없다.

    ring_buffer[in] = next_produced;
    in = (in + 1) % BUF_SIZE;
    count++;
  }

  return NULL;
}

void *consume(void *arg)
{
  item_t next_consumed;

  while (1) {
    while (in == out); // 버퍼가 비어있으면 더이상 소비할 수 없다.

    // 아이템 소비
    next_consumed = ring_buffer[out];
    out = (out + 1) % BUF_SIZE;
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