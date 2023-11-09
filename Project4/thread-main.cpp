#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <sstream>
#include "thread.h"

//-------------------------------------------------------------
//Name: Nicky Franklin            UserID:nafrankl
//Due Date: 10/16/2023
//Program Assignment 3
//File Name: thread-main.cpp
//Program Purpose:
//Drives the main code that is used to calculate the prefix sum
//of an array using concurrent threads to do it
//-------------------------------------------------------------

using namespace std;
Semaphore potsEmpty("potsEmpty", 1);
Semaphore momSleeping("momSleeping", 0);
Semaphore momBack("momBack", 0);
Semaphore mom("mom", 0);
Semaphore mutex3("mutex3", 1);
int food;
int numberEating;
bool foodAvailable;
int babyWaker;

int main(int argc, char **argv) {
  //Declare variables and set them
  char buf[1000];
  if(argc < 4) {
    cout << "./prog4 m n t" << endl;
  }
  
  int m = argv[1];
  int n = argv[2];
  int t = argv[3];
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
    babies[i].setStart(i);
  }

  Mother mother(m, n, t);

  food = 0;
  numberEating = 0;
  foodAvailable = false;

  cout << "MAIN: There are " << n << " baby eagles, " << m << "feeding pots, and " << t
       << " feedings.\n";
  cout << "MAIN: Game starts!!!!!";
  
  char buf2[21];
  for(int i = 0; i < n; i++) {
    if(i == 0) {
      sprintf(buf, "Mother Eagle Started.\n");
      write(1, buf, strlen(buf));
      mother.begin();
    }
    
    for(int j = 0; j < i; j++) {
      buf2[j] = ' ';
      buf2[j+1] = '\0';
    }
    sprintf(buf, "%sBaby eagle %d Started.\n", buf2, i);
    write(1, buf, strlen(buf));
    babies[i].begin();
  }
  
  while(!(mother.isRetired)) {

  }
  
  Exit();
  return 0;
}
