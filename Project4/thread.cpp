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



//--------------------------------------------------------------
//Name: Nicky Franklin          User Id: nafrankl
//Due Date: 11/8/23
//Program Assignment 4
//File Name: thread.cpp
//Program Purpose:
//Define the class functions that are to be used to do the baby and mother eagle programs
//----------------------------------------------------------------




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

//--------------------------------------------------------------------------------
//Function setStart(int n, int m, int t)
//This function sets up the baby eagles
//Parameter Usage:
//  n: number eagles
//  m: number feeding pots
//  t: number feedings
//Function Called:
//  n/a
//-----------------------------------------------------------------------------
void Baby::setStart(int n, int m, int t) {
  this->numberEagle = n;
  this->m = m;
  this->t = t;
  this->numberPot = -1;
  for(int i = 0; i < n+1; i++) {
    buf2[i] = ' ';
    buf2[i+1] = '\0';
  }
}

//--------------------------------------------------------------------------------
//Function Mother(int m, int n, int t)
//This function sets up the mother eagle
//Parameter Usage:
//  n: number eagles
//  m: number feeding pots
//  t: number feedings
//Function Called:
//  n/a
//-----------------------------------------------------------------------------
Mother::Mother(int m, int n, int t) {
  this->m = m;
  this->n = n;
  this->t = t;
  this->numbersFed = 0;
  this->isRetired = false;
}

//--------------------------------------------------------------------------------
//Function threadFunc()
//This function sets up the driver code for the baby threads
//Parameter Usage:
//  n/a
//Function Called:
//  n/a
//-----------------------------------------------------------------------------
void Baby::ThreadFunc() {
  Thread::ThreadFunc();
  while(1) {
    Delay();
    ready_to_eat();
    Delay();
    finish_eating();
  }
  Exit();
}

//--------------------------------------------------------------------------------
//Function threadFunc()
//This function sets up the driver code for the mother thread
//Parameter Usage:
//  n/a
//Function Called:
//  n/a
//-----------------------------------------------------------------------------
void Mother::ThreadFunc() {
  Thread::ThreadFunc();
  while(!isRetired) {
    goto_sleep();
    Delay();
    food_ready();
    Delay();
  }
  
  Exit();
}
