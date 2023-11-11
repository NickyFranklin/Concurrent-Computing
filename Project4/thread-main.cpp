#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <sstream>
#include "thread.h"

//-------------------------------------------------------------
//Name: Nicky Franklin            UserID:nafrankl
//Due Date: 11/8/2023
//Program Assignment 4
//File Name: thread-main.cpp
//Program Purpose:
//Drives the main code that is used to calculate the baby and mother eagle threads
//-------------------------------------------------------------

using namespace std;
Semaphore potsEmpty("potsEmpty", 1);
Semaphore momSleeping("momSleeping", 0);
Semaphore momBack("momBack", 0);
Semaphore mom("mom", 0);
Semaphore mutex3("mutex3", 1);
Semaphore potsAllowed("potsAllowed", 0);
int food;
int numberEating;
bool foodAvailable;
int babyWaker;
int numberFeedings = 0;

//------------------------------------------------------------------------------
//Function main:
//  Provides the code necessary to start the eagle program
//Parameter Usage
//  argc: Number of arguments handed over
//  argv: The arguments handed over (number of eagles, number of feeding pots, number of feedings
//Function Called:
//  setStart: inits baby eagles
//  mother: inits mother eagle
//  Begin: starts mother and baby threads
//------------------------------------------------------------------------------------
int main(int argc, char **argv) {
  //Declare variables and set them
  char buf[1000];
  if(argc < 4) {
    cout << "./prog4 m n t" << endl;
  }
  
  int m = atoi(argv[1]);
  int n = atoi(argv[2]);
  int t = atoi(argv[3]);
  //char buf[1000];
  
  if(m == 0) {
    m = 10;
  }

  if(n == 0) {
    n = 10;
  }

  if(t == 0) {
    t = 10;
  }
  
  Baby babies[20];
  
  for(int i = 0; i < n; i++) {
    babies[i].setStart(i, m, t);
  }

  for(int i = 0; i < m; i++) {
    potsAllowed.Signal();
  }
  
  Mother mother(m, n, t);

  food = 0;
  numberEating = 0;
  foodAvailable = false;

  sprintf(buf, "MAIN: There are %d baby eagles, %d feeding pots, and %d feedings\nMAIN: Game starts!!!!!!!", n, m, t);
  write(1, buf, strlen(buf));  
  
  char buf2[21];
  for(int i = 0; i < n; i++) {
    if(i == 0) {
      sprintf(buf, "Mother Eagle Started.\n");
      write(1, buf, strlen(buf));
      mother.Begin();
    }
    
    for(int j = 0; j < i+1; j++) {
      buf2[j] = ' ';
      buf2[j+1] = '\0';
    }
    sprintf(buf, "%sBaby eagle %d Started.\n", buf2, i+1);
    write(1, buf, strlen(buf));
    babies[i].Begin();
  }
  
  while(!(mother.isRetired)) {

  }
  
  Exit();
  return 0;
}
