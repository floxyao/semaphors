#ifndef STRUCT_H
#define STRUCT_H

#include "func.h"

// Struct thread_data
// Description: Contains data of each thread stored separately in a struct
// Each thread has thread ID and a pointer to the shared buffer in memory
struct thread_data{
  int thread_id;
  float *buf_ptr;
};

// Struct shared_buf
// Member Variables: Float pointer buf
// Description: Allocates memory for a buffer
//              Initializes buffer to a non-zero integer between 1 and 100;
struct my_buf{
  float *buf;

  my_buf(){
    buf = new float[BUFFSIZE];
  }

  void init(){
    float *walker = buf;
    for(int i=0; i<BUFFSIZE; i++){
      *walker = get_random(1,100); //init to non-zero
      walker++;
    }
  }
};

#endif
