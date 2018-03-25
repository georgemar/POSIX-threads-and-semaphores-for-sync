#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include "display.h"
#include <pthread.h>
#include <fcntl.h>

pthread_mutex_t mutex;

void *t1( void *param) {
  int i=0;
  for (i = 0; i < 10; i++){
    pthread_mutex_lock(&mutex);
    display("Hello world\n");
    pthread_mutex_unlock(&mutex);
  }
}
void *t2( void *param ){
  int i=0;
  for (i = 0; i < 10; i++) {
    pthread_mutex_lock(&mutex);
    display("Kalimera kosme\n");
    pthread_mutex_unlock(&mutex);
  }
}

int main() {
  int  iret1, iret2;
  pthread_t thread1, thread2;
  iret1 = pthread_create( &thread1, NULL, t1, NULL);
  iret2 = pthread_create( &thread2, NULL, t2, NULL);
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  pthread_mutex_destroy(&mutex);
  pthread_exit(&thread1);
  pthread_exit(&thread2);
  return 0;
}
