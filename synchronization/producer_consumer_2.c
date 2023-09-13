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
** 버퍼 크기만큼 채울 수 있지만
** 두 스레드가 동시에 같은 변수에 접근해 값을 변경하므로 레이스 컨디션이 발생한다.
*/
void *produce(void *arg)
{
  item_t next_produced;

  while (1) {
    // 아이템 생산
    sprintf(next_produced.message, "%d", count);
    next_produced.value = rand() % 10;
    printf("producer: %s\n", next_produced.message);
  
    while (count == BUF_SIZE);

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
    while (count == BUF_SIZE);

    next_consumed = ring_buffer[tail];
    tail = (tail + 1) % BUF_SIZE;
    count--;

    // 아이템 소비
    printf("consumer: %s", next_consumed.message);
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