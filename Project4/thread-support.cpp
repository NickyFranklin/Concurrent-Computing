#ifndef THREADCLASS_H
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
extern Semaphore potsEmpty;
extern Semaphore momSleeping;
extern Semaphore momBack;
extern Semaphore mom;
extern Semaphore mutex3;
extern Semaphore potsAllowed;
extern int food;
extern int numberEating;
extern bool foodAvailable;
extern int babyWaker;
extern int numberFeedings;


//-------------------------------------------------------------------
//Name: Nicky Franklin                             User ID: nafrankl
//Due Date: 11/8/2023
//Program Assigment 4
//File Name: thread-support.cpp
//Program Purpose:
//This file provides the supporting functions for the eagle thread
//programs
//------------------------------------------------------------------



//----------------------------------------------------------------------
//Function ready_to_eat()
//  This function is responsible for controlling how the baby eagles eat
//  and making sure that they don't eat what they shouldn't
//Parameter usage:
//  buf: Buffer for text to be output to terminal
//  potsAllowed: semaphore to allow only a certain number of eagles through to eat
//  mutex3: Ensures mutual exclusion
//  numberEagle: purely for printing the right number
//  food: keeps track of the ammount of food left
//  numberEating: keeps number of kids eating
//  potsEmpty: Makes sure all the pots are empty before telling mom to get more food
//  momSleeping: waits until mom sleeps
//  mom: wakes mom
//  babyWaker: Tells mom which baby woke her
//Function Called:
//  N/A
//-----------------------------------------------------------------------------------
void Baby::ready_to_eat() {
  //Since we are editing commonly shared attributes, these must all be edited one at a time
 
  sprintf(buf, "%sBaby eagle %d is ready to eat\n", buf2, numberEagle+1);
  write(1, buf, strlen(buf));
  potsAllowed.Wait();
  mutex3.Wait();
  //If there is food, then eat
  if(food > 0) {
    sprintf(buf, "%sBaby Eagle %d is eating using feeding pot %d\n", buf2, numberEagle+1, food);
    write(1, buf, strlen(buf));
    food--;

    //Allow the next baby through
    numberEating++;
    
    mutex3.Signal();
  }

  //If there is no more food, wake mom up
  else {
    //If there are people still eating, then wait
    if(numberFeedings+1 > t) {
      mom.Signal();
      while(1) {
	
      }
    }
    potsEmpty.Wait();
    
    //mom is not sleeping yet, then wait
    momSleeping.Wait();
    
    sprintf(buf, "%sBaby eagle %d sees all feeding pots are empty and wakes up the mother\n",
	    buf2, numberEagle+1);
    write(1, buf, strlen(buf));
    babyWaker = numberEagle+1;
    
    //Wake mom up
    mom.Signal();
    
    //Wait for mom to bring back food
    momBack.Wait();

    sprintf(buf, "%sBaby Eagle %d is eating using feeding pot %d\n", buf2, numberEagle+1, food);
    write(1, buf, strlen(buf));
    //Make food available again
    food--;
    
    //Start Eating
    numberEating++;
    mutex3.Signal();
  }
  //mutex3.Signal();
  
  return;
}

//----------------------------------------------------------------------
//Function finish_eating()
//  This function is responsible for controlling the finishing of eating for the eagles
//  the last one to finish eating calls the next batch to eat
//Parameter usage:
//  buf: Buffer for text to be output to terminal
//  potsAllowed: semaphore to allow only a certain number of eagles through to eat
//  mutex3: Ensures mutual exclusion
//  food: keeps track of the ammount of food left
//  numberEating: keeps number of kids eating
//  potsEmpty: Makes sure all the pots are empty before telling mom to get more food
//Function Called:
//  N/A
//-----------------------------------------------------------------------------------
void Baby::finish_eating() {
  sprintf(buf, "%sBaby eagle %d finishes eating\n", buf2, numberEagle+1);
  write(1, buf, strlen(buf));
  mutex3.Wait();
  numberEating--;
  if(numberEating == 0 && food == 0) {
    potsEmpty.Signal();
    for(int i = 0; i < m; i++) {
      potsAllowed.Signal();
    }
  }
  //sprintf(buf, "bird %d after finishEating statement\n", numberEagle);
  //write(1, buf, strlen(buf));
  mutex3.Signal();
  return;
}

//----------------------------------------------------------------------
//Function goto_sleep()
//  This function is responsible for controlling how the mother sleeps and determining
//  if she retires
//Parameter usage:
//  buf: Buffer for text to be output to terminal
//  momSleeping: waits until mom sleeps
//  mom: wakes mom
//  babyWaker: Tells mom which baby woke her
//  numbersfed: Keeps track of the numbers fed
//  numberFeedings: see above
//Function Called:
//  retire(): determines if mom is retiring
//-----------------------------------------------------------------------------------
void Mother::goto_sleep() {
  momSleeping.Signal();
  sprintf(buf, "Mother eagle takes a nap.\n", babyWaker);
  write(1, buf, strlen(buf));
  mom.Wait();
  if(retire()) {
    sprintf(buf, "Mother Eagle retires after %d feedings. Game is over!!!!\n", numbersFed);
    write(1, buf, strlen(buf));
    exit(0);
    return;
  }
  sprintf(buf, "Mother Eagles is awoke by baby eagle %d and starts preparing food.\n", babyWaker);
  write(1, buf, strlen(buf));
 
  //May change where these go
  numbersFed++;
  numberFeedings++;
}

//----------------------------------------------------------------------
//Function food_ready()
//  This function is responsible for adding food to the pots
//Parameter usage:
//  buf: Buffer for text to be output to terminal
//  food: Keeps track of ammount of food
//  momBack: tells children that food has been refilled
//  numbersFed: The numbered feeding
//Function Called:
//  N/A
//-----------------------------------------------------------------------------------
void Mother::food_ready() {
  sprintf(buf, "Mother eagle says \"Feeding (%d)\"\n", numbersFed);
  write(1, buf, strlen(buf));
  food = this->m;
  momBack.Signal();
}

//----------------------------------------------------------------------
//Function retire()
//  Decides if the mom is retiring
//Parameter usage:
//  numbersFed: Keeps track of the number of feedings
//  t: max number of feedings
//  isRetired: determines if mother should be retired
//Function Called:
//  N/A
//-----------------------------------------------------------------------------------
bool Mother::retire() {
  if(numbersFed+1 > t) {
    this->isRetired = true;
    return true;
  }
  return false;
}
