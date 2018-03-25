#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include "display.h"
#include <pthread.h>
#include <fcntl.h>

pthread_mutex_t mutex;
pthread_mutex_t mutex1;
pthread_cond_t cond_var;
pthread_cond_t cond_var1;
int con1=0;
int con2=0;
void *t1( void *param) {
  int i=0;
  for (i = 0; i < 10; i++){
    pthread_mutex_lock(&mutex);
    display("ab");
    con1=1;
    pthread_cond_signal(&cond_var);
    while(con2==0){
      pthread_cond_wait(&cond_var1,&mutex);
    }
    con2=0;
    pthread_mutex_unlock(&mutex);
  }
}
void *t2( void *param ){
  int i=0;
  for (i = 0; i < 10; i++) {
    pthread_mutex_lock(&mutex1);
    while(con1==0){
      pthread_cond_wait(&cond_var,&mutex1);
    }
    con1=0;
    display("cd\n");
    con2=1;
    pthread_cond_signal(&cond_var1);
    pthread_mutex_unlock(&mutex1);
  }
}

int main() {
  int  iret1, iret2;
  pthread_t thread1, thread2;
  pthread_mutex_init(&mutex, NULL); 
  pthread_mutex_init(&mutex1, NULL); 
  pthread_cond_init(&cond_var1, NULL);
  pthread_cond_init(&cond_var, NULL);
  iret1 = pthread_create( &thread1, NULL, t1, NULL);
  iret2 = pthread_create( &thread2, NULL, t2, NULL);
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  pthread_mutex_destroy(&mutex);
  pthread_mutex_destroy(&mutex1);
  pthread_exit(&thread1);
  pthread_exit(&thread2);
  return 0;
}
