#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void sigusr_handler(int sig)
{
  printf("received signal %d\n", sig);
  pthread_cond_signal(&cond);
}

int main(int argc, char **argv)
{
  pid_t pid = getpid();
  printf("pid: %d\n", pid);
  signal(SIGUSR1, sigusr_handler);

  pthread_mutex_lock(&mut);

  if (pthread_cond_wait(&cond, &mut)) {
    perror("fail to wait pthread condition variable");
    exit(1);
  }

  printf("sprious wake up\n");
  pthread_mutex_unlock(&mut);

  return 0;
}