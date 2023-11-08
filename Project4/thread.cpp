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
//Due Date: 10/16/23
//Program Assignment 3
//File Name: thread.cpp
//Program Purpose:
//Define the class functions that are to be used to calculate
//The prefix sum
//----------------------------------------------------------------




using namespace std;

extern Semaphore babySem;
extern Semaphore motherSem;
extern int potsFilled;

//--------------------------------------------------------------------------------
//Function ThreadFunc()
//This function calculates the prefix sum of a certain index using threads
//Parameter Usage: N/A
//Function Called:
//ThreadFunc()
//-----------------------------------------------------------------------------
Baby::Baby(int n) {
  this.numberEagle = n;
  this.numberPot = -1;
}

Mother::Mother(int m, int n, int t) {
  this.m = m;
  this.n = n;
  this.t = t;
  this.numbersFed = 0;
  this.isRetired = false;
}

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
