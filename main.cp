#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <math.h>
#include <semaphore.h>
#include "const.h"
#include "struct.h"
#include "func.h"
using namespace std;
#define PERMS S_IWUSR|S_IRUSR

void* my_thread(void *arg);
void  init_thread_data(struct thread_data *td_ptr, float *buf);
void update(int index, float beta, float *buf_ptr);
sem_t sem0, sem1, sem2, sem3;

int main(){
  srand(time(0)); //seed for rand()
  pthread_t threads[NUM_THREADS]; //pthread object array
  struct thread_data td[NUM_THREADS]; //stores data about pthread
  struct my_buf mb; //declare buffer

  //get shared memory ID
  int shmid = shmget(IPC_PRIVATE, BUFFSIZE*sizeof(char), PERMS);

  //set my buffer pointer to shared memory location
  mb.buf = (float *)shmat(shmid, 0, SHM_RND);

  //init non-zero values into buffer
  mb.init();

  //init thread_data
  init_thread_data(td, mb.buf);

  //init semaphores
  sem_init(&sem0,1,1);
  sem_init(&sem1,1,1);
  sem_init(&sem2,1,1);
  sem_init(&sem3,1,1);

  //create threads
  //pass in the actual thread and my thread_data struct array (td)
  //so each thread gets created with its own thread_data structure
  for(int i=0;i<NUM_THREADS;i++){
    if(pthread_create(&(threads[i]), NULL, &my_thread, (void *)&td[i]) != 0){
      cout<<"Error creating thread "<<i;
    }
  }
  //join threads
  for(int i=0; i<NUM_THREADS;i++){
    if(pthread_join(threads[i], NULL) != 0){
      cout<<"Error joining thread "<<i;
    }
  }

  //safely detach ptr from shared mem location
  if(shmdt(mb.buf) != 0)
    cout<<"\nDetach Shared Memory Failed\n";
  else
    cout<<"\nDetach Shared Memory Success\n";

  //safely remove semaphores
  if(sem_destroy(&sem0) != 0){
    cout<<"\nSem 0 Removed"<<endl;
  }
  if(sem_destroy(&sem1) != 0){
    cout<<"\nSem 1 Removed"<<endl;
  }
  if(sem_destroy(&sem2) != 0){
    cout<<"\nSem 2 Removed"<<endl;
  }
  if(sem_destroy(&sem3) != 0){
    cout<<"\nSem 3 Removed"<<endl;
  }
  return 0;
} // main

//function:      my_thread()
//input(s):      void argument
//output(s):     none
//description:   Gets called by all concurrent processes. Void argument is casted
//               to my thread_data structure so each process may access their pointer to the
//               shared memory location. Uses pthread function from pthread library to utilize
//               semaphore mutex lock.
//precondition:  each thread is "created" in main and "joined".
//               thread_data is initialized
//               buffer in memory is initialized
//postcondition: buffer[index] in memory is updated to new value
//               mutex lock is destroyed
//               exit thread after done.
void* my_thread(void *arg){
  //get data from thread_data structure
  struct thread_data *my_data;
  my_data = (struct thread_data *) arg; //cast thread argument

  int i=0;
  while(i<MAX_ROUNDS){
    float beta   = get_beta();
    int   index  = get_random(0, 3);

    cout<<"\nThread "<<my_data->thread_id;
    
    update(index, beta, my_data->buf_ptr);
    i++;
  }
  pthread_exit(NULL);
}

//function:      init_thread_data()
//input(s):      struct thread_data pointer, buffer pointer
//output(s):     none
//description:   each process has its own data stored in the thread_data struct
//               containing its thread_ID and pointer to the shared buffer in memory.
//               this function initalizes thread_ID and the pointer to each process.
//precondition:  thread_id is 0 (not been initialized)
//               buf is pointing to the shared memory location.
//postcondition: buffer pointer in thread data is now pointing to the shared memory.
//               thread_id is assigned.
void init_thread_data(struct thread_data *td, float *buf){
  for(int i=0;i<NUM_THREADS;i++){
    td[i].thread_id = i;
    td[i].buf_ptr = buf; //set each thread's pointer to the shared memory
  }
}

//function:      update()
//input(s):      index, beta, buffer pointer
//output(s):     none
//description:   passes index of buffer, beta value and pointer to
//               shared mem. sem_wait decrements sem so that no other
//               process may access that specific memory location.
//precondition:  sems are initialized to 1, index is between 0-4,
//               beta in range [-0.5,0.5], buf_ptr points to shared mem
//postcondition: buffer[index] in memory is updated to new value
void update(int index, float beta, float *buf_ptr){}
  switch(index){
    case 0:
      sem_wait(&sem0);
      buf_ptr[0] += beta;
      cout<<" beta="<<beta<<" buffer["<<index<<"]="<<buf_ptr[0];
      sem_post(&sem0);
      break;
    case 1:
      sem_wait(&sem1);
      buf_ptr[1] += beta;
      cout<<" beta="<<beta<<" buffer["<<index<<"]="<<buf_ptr[1];
      sem_post(&sem1);
      break;
    case 2:
      sem_wait(&sem2);
      buf_ptr[2] += beta;
      cout<<" beta="<<beta<<" buffer["<<index<<"]="<<buf_ptr[2];
      sem_post(&sem2);
      break;
    case 3:
      sem_wait(&sem3);
      buf_ptr[3] += beta;
      cout<<" beta="<<beta<<" buffer["<<index<<"]="<<buf_ptr[3];
      sem_post(&sem3);
      break;
    default:
      break;
  }
}
