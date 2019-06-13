#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

int nb_threads=4;
char array[4][1024];

void* thread_function(void* arg){
  int my_id = *(int*)arg;
  snprintf(array[my_id], 1024, "Hello from thread %d", my_id);

  sleep(1);
  return NULL;
}


int main(int argc, char**argv) {
  pthread_t tid[nb_threads];
  int ret;

  for(int i=0; i<nb_threads; i++) {
    ret = pthread_create(&tid[i], NULL, thread_function, &i);
    if(ret != 0) {
      perror("pthread_create failed");
      abort();
    }
  }

  for(int i=0; i<nb_threads; i++) {
    ret = pthread_join(tid[i], NULL);
    if(ret != 0) {
      perror("pthread_create failed");
      abort();
    }
  }

  for(int i=0; i<nb_threads; i++) {
    printf("Thread %d wrote: %s\n", i, array[i]);
  }
  
  return EXIT_SUCCESS;
}
