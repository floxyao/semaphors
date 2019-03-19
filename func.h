#ifndef FUNC_H
#define FUNC_H

//function: get_beta()
//input(s): none
//output(s): none
//description: beta value is chosen randomly in range [-0.5,0.5]
//precondition:  HI value is 0.5, LO value is -0.5
//postcondition: new value is chosen randomly in range and rounded up
float get_beta(){
  double HI = 0.5;
  double LO = -0.5;
  float f = LO + static_cast <float> (rand()) /
                (static_cast <float> (RAND_MAX/(HI-LO)));

  f = ceilf(f * 10)/10; //round up
  return f;
}

//function: get_random()
//input(s): hi, lo
//output(s): none
//description: simple random function
//precondition:  hi and lo are integers
//postcondition: random integer is returned
int get_random(int lo, int hi){
  return rand() % (hi+1) + lo;
}


#endif
