#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define PROCON_RIVER "/dev/procon_driver"

#define READ_USE_COUNT _IOR('r', '1', int32_t *)
#define INITIALIZE_BUFFER _IOW('w', '1', int32_t *)

#define THREAD_NUM 4

#define print_err(msg) \
( \
  fprintf(stderr, "%s\n", msg) \
)

struct readout {
  int index;
  int value;
};

static int loop;

void *produce(void *arg)
{
  int fd, i, tnum;

  tnum = (int)arg;

  printf("[producer %d] produce\n", tnum);

  if ((fd = open(PROCON_RIVER, O_WRONLY)) < 0) {
    print_err("fail to open device driver");
    return NULL;
  }

  for (i = 0; i < loop; i++) {
    if (write(fd, &i, sizeof(int)) < 0) {
      print_err("fail to write to device");
      goto err;
    }
  }

  close(fd);

err:
  close(fd);
}

void *consume(void *arg)
{
  int fd, i, tnum;
  struct readout rdout;

  tnum = (int)arg;

  printf("[consumer %d] consume\n", tnum);

  if ((fd = open(PROCON_RIVER, O_RDONLY)) < 0) {
    print_err("fail to open device driver");
    return NULL;
  }

  for (int i = 0; i < loop; i++) {
    if (read(fd, &rdout, sizeof(rdout)) < 0) {
      print_err("fail to read from device");
      goto err;
    }
  }

  close(fd);

err:
  close(fd);
}

int main(int argc, char **argv)
{
  int fd, i, count, offset;
  pthread_t t[THREAD_NUM];

  offset = 3;

  if (argc != 2) {
    print_err("input loop count");
    exit(1);
  }

  loop = atoi(argv[1]);
  printf("loop count: %d\n", loop);

  if ((fd = open(PROCON_RIVER, O_RDONLY)) < 0) {
    print_err("fail to open device driver");
    return -1;
  }  

  if (ioctl(fd, READ_USE_COUNT, &count) < 0) {
    print_err("fail to ioctl read use_count");
    goto err;
  }

  printf("use_count: %d\n", count);

  if (ioctl(fd, INITIALIZE_BUFFER, &count) < 0) {
    print_err("fail to ioctl initialize buffer");
    goto err;
  }

  printf("initialized\n");

  if (ioctl(fd, READ_USE_COUNT, &count) < 0) {
    print_err("fail to ioctl read use_count");
    goto err;
  }

  for (i = 0; i < offset; i++) {
    printf("producer thread %d\n", i);
    pthread_create(&t[i], NULL, produce, i);
  }

  for (i = 0; i < THREAD_NUM - offset; i++) {
    printf("consumer thread %d\n", i);
    pthread_create(&t[offset + i], NULL, consume, i);
  }

  for (i = 0; i < THREAD_NUM; i++) {
    pthread_join(t[i], NULL);
  }

  close(fd);
  return 0;

err:
  close(fd);
  return -1;
}