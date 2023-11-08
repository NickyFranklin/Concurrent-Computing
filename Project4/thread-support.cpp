#idndef THREADCLASS_H
#define THREADCLASS_H
#include "ThreadClass.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <sstream>
#include "thread.h"
//Thread.h may give a merge conflict later

using namespace std;
extern Semaphore mutex;
extern Semaphore potsEmpty;
extern Semaphore momSleeping;
extern Semaphore momBack;
extern Semaphore mom;
extern Semaphore mutex2;
extern bool foodAvailable;
extern int food;
extern int numberEating;

void Baby::ready_to_eat() {
  //Since we are editing commonly shared attributes, these must all be edited one at a time
  mutex.wait();

  //If there is food, then eat
  if(food > 0) {
    food--;

    //Allow the next baby through
    numberEating++;
    mutex.signal();
  }

  //If there is no more food, wake mom up
  else {
    //If there are people still eating, then wait
    potsEmpty.wait();

    //mom is not sleeping yet, then wait
    momSleeping.wait();

    //Wake mom up
    mom.signal();

    //Wait for mom to bring back food
    momBack.wait();

    //Make food available again
    food--;
    
    //Start Eating
    numberEating++;
    
    //Tell next baby to come through
    mutex.signal();
  }
  
  return;
}

void Baby::finish_eating() {
  mutex2.wait();
  numberEating--;
  if(numberEating == 0 && food == 0) {
    potsEmpty.signal();
  }
  mutex2.signal()
  
}

void Mother::goto_sleep() {
  momSleeping.signal();
  mom.wait();
  //May change where these go
  numbersFed++;
  retire();
  if(isRetired) {
    //end game
  }
}

void Mother::food_ready() {
  food = this.m;
  momBack.signal();
}

void Mother::retire() {
  if(numbersFed > t) {
    this.isRetired = true
  }
}
