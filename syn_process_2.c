#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include "display.h"
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>

int main() {
  int i;
  int shmid = shmget(IPC_PRIVATE, sizeof(sem_t), 0600);
  sem_t *sem1 = (sem_t *) shmat(shmid, 0, 0);
  sem_init(sem1,1,1);
  int shmid1 = shmget(IPC_PRIVATE, sizeof(sem_t), 0600);
  sem_t *sem2 = (sem_t *) shmat(shmid1, 0, 0);
  sem_init(sem2,1,0);

  if (fork()) {
    for (i=0; i<10; i++) {
      sem_wait(sem1);
      display("ab");
      sem_post(sem2);
    }
    wait(NULL);
  } else {
    for (i=0; i<10; i++) { 
      sem_wait(sem2);
      display("cd\n"); 
      sem_post(sem1);
    }
  }
  shmdt((void *) sem1);
  shmctl(shmid, IPC_RMID, 0);
  shmdt((void *) sem2);
  shmctl(shmid1, IPC_RMID, 0);
  sem_destroy(sem1);
  sem_destroy(sem2);
  return 0;
}