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

extern int** B;
//---------------------------------------------------------
//Function PreFix():
//This is a default constructor so that my program runs
//Parameters: N/A
//Function Called: N/A
//-----------------------------------------------------------
PreFix::PreFix() {
  //This is intentionally left blank as I needed a default constructor
}

//-----------------------------------------------------------------------------------
//Function setPrefix()
//This sets the initial values for the important variables that are going to be used
//Parameter Usage:
//int n: Size of a row
//int x: The starting array
//int i: the index of what row this object is responsible for
//int j: The index of the collumn the object is responsible for
//Function Called:
//log()
//----------------------------------------------------------------------------------------------
void PreFix::setPreFix(int n, int *x, int i, int j) {
  this->n = n;
  this->x = x;
  this->i = i;
  this->j = j;
  log();
}

//------------------------------------------------------
//Function ~PreFix()
//This frees the memory that the class takes up
//Parameter Usage: N/A
//Function Called: N/A
//-------------------------------------------------------
PreFix::~PreFix() {
  free(x);
  for(int i = 0; i < k+1; i++) {
    free(B[i]);
  }
  free(B);
}
//--------------------------------------------------------------------------------------------------
//Function Print()
//This prints out the current row information. This is a deprecated feature and should be ignored
//Parameter Usage: N/A
//Function Called:N/A
//---------------------------------------------------------------------------------------------------
void PreFix::print() {
  cout << n << endl;
  for(int i = 0; i < n; i++) {
    cout << x[i] << " ";
  }
  cout << endl;

  cout << k << endl;
}

//--------------------------------------------------------
//Function exponent()
//This multiplies 2 by itself k times for the prefix sum
//Parameter Usage: N/A
//Function Called: N/A
//--------------------------------------------------------
int PreFix::exponent() {
  int square = 2;
  for(int k = 0; k < i; k++) {
    square *= 2;
  }
  return square;
}

//----------------------------------------------------------------------------------
//Function: log()
//This function calculates log base 2 of the size of a row for the prefix sum
//Parameter Usage: N/A
//Function Called: N/A
//---------------------------------------------------------------------------------

void PreFix::log() {
  int i = 0;
  int temp = n;
  while(temp != 1) {
    temp /= 2;
    i++;
  }
  this->k = i;
}

//--------------------------------------------------------------------------------
//Function ThreadFunc()
//This function calculates the prefix sum of a certain index using threads
//Parameter Usage: N/A
//Function Called:
//ThreadFunc()
//-----------------------------------------------------------------------------
void PreFix::ThreadFunc() {
  Thread::ThreadFunc();
  char buf[1000];
  int temp;
  int expo;
  stringstream msg2;
  int two = 1;
  //Calculate the number that will be used to determine the indexes to add by
  for(int p = 0; p < i-1; p++) {
    two *= 2;
  }
  expo = two;
  temp = j - expo;
  
  if(temp < 0) {
    sprintf(buf, "      Thread %d copies x[%d]\n", j, j);
    B[i][j] = B[i-1][j];
    write(1, buf, strlen(buf));
  }
  
  else {
     sprintf(buf, "      Thread %d computes x[%d] + x[%d]\n", j, j, temp);
    B[i][j] = B[i-1][j] + B[i-1][temp];
    write(1, buf, strlen(buf));
  }
  i++;
  
  if(i > k) {
    i--;
  }
  sprintf(buf, "      Thread %d Exits\n", j);
  write(1, buf, strlen(buf));
  Exit();
}
