#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include "display.h"
#include <semaphore.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/ipc.h>

int main() {
  int i;
  int shmid = shmget(IPC_PRIVATE, sizeof(sem_t), 0600);
  sem_t *sem = (sem_t *) shmat(shmid, 0, 0);
  sem_init(sem,1,1);
  if (fork()) {
    for (i=0;i<10;i++) { 
      sem_wait(sem); 
      display("Hello world\n");  
      sem_post(sem);  
    }
    wait(NULL);
  } else {
    for (i=0;i<10;i++) { 
      sem_wait(sem);
      display("Kalimera kosme\n");
      sem_post(sem);
    }
  }
  shmdt((void *) sem);
  shmctl(shmid, IPC_RMID, 0);
  sem_destroy(sem);
  return 0;
}
