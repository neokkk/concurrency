#include <errno.h>
#include <sys/sem.h>

int init(int semid, int semnum) {
  union semun arg;
  arg.val = 1;
  return semctl(semid, semnum, SETVAL, arg);
}

int P(int semid, int semnum) {
  struct sembuf buf;
  buf.sem_op = -1;
  buf.sem_num = semnum;
  buf.sem_flg = SEM_UNDO;
  
  while (semop(semid, &buf, 1) == -1) {
    if (errno != EINTR) return -1;
  }
  return 0;
}

int V(int semid, int semnum) {
  struct sembuf buf;
  buf.sem_op = 1;
  buf.sem_num = semnum;
  buf.sem_flg = SEM_UNDO;
  
  return semop(semid, &buf, 1);
}