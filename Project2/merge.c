#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
/*---------------------------------------------------------------------------*/
/* NAME: Nicky Franklin                                   User ID:nafrankl   */
/* DUE DATE: 10/4/2023                                                       */
/* PROGRAM ASSIGNMENT 2                                                      */
/* FILE NAME: merge.c                                                        */
/* PROGRAM PURPOSE: To take two sorted arrays and combine them together      */
/*---------------------------------------------------------------------------*/


/* ----------------------------------------------------------- */
/* FUNCTION binarySearch                                       */
/* Purpose: to determine the index that the number should go   */
/* into m                                                      */
/* PARAMETER USAGE :                                           */
/* int arr[]: The array that has the values we are determing   */
/* int l: the left side of the array                           */
/* int r: the right side of the array                          */
/* int x: the number we are comparing against                  */
/* int i: the index x came from                                */
/* int isX: boolean for if the call came from x[]              */
/* char* buff: The writing buff                                */
/* FUNCTION CALLED :                                           */
/* binarySearch                                                */
/* ----------------------------------------------------------- */
int binarySearch(int arr[], int l, int r, int x, int i, int isX, char *buff) {
  //Makes sure values are proper
  while( l <= r) {
    //Midpoint
    int k = l + (r-l) / 2;
    pid_t cpid = getpid();
    //If the midpoint is found, return it
    if(arr[k] <= x && arr[k+1] >= x) {
      if(isX) {
	sprintf(buff, "      $$$ M-PROC(%d): x[%d] = %d is found between y[%d] = %d and y[%d] = %d\n",
		(int) cpid, i, x, k, arr[k], k+1, arr[k+1]);
	write(1, buff, strlen(buff));
      }

      else {
	sprintf(buff, "      $$$ M-PROC(%d): x[%d] = %d is found between y[%d] = %d and y[%d] = %d\n",
		(int) cpid, i, x, k, arr[k], k+1, arr[k+1]);
	write(1, buff, strlen(buff));
      }
      return k;
    }
    
    //If x is too small, half the max value and run it again
    else if(arr[k] > x) {
      return binarySearch(arr, l, k -1, x, i, isX, buff);
    }

    //If it gets here, then the lower bound needs to be upped
    return binarySearch(arr, k+1, r, x, i, isX, buff);
    
     
  }
  return -1;
}

/* ----------------------------------------------------------- */
/* FUNCTION main                                               */
/* Purpose: Drive the merge code and merge the arrays together */
/* PARAMETER USAGE :                                           */
/* int argc: number of argument                                */
/* char** argv: the arguments given to the program             */
/* FUNCTION CALLED :                                           */
/* binarySearch, fork                                          */
/* ----------------------------------------------------------- */
int main(int argc, char** argv) {
  if(argc < 4) {
    printf("\noops\n");
  }
  //init variables and shared memory
  int *x;
  int *y;
  pid_t cpid, wpid;
  int status;
  x = shmat(atoi(argv[1]), NULL, 0);
  y = shmat(atoi(argv[2]), NULL, 0);
  int *m = shmat(atoi(argv[5]), NULL, 0);
  cpid = getpid();
  char buff[1000];
  int sizeX = atoi(argv[3]);
  int sizeY = atoi(argv[4]);
  //int length = strlen(buff);
  
  wpid = getpid();
  int originalIndex;
  int modifiedIndex;
  int finalIndex;
  int number = 0;
  //use to avoid warnings
  if(number) {

  }
  
  //adds n+m children and have them each calculate a position of the array
  for(int i = 0; i < (sizeX+sizeY); i++) {
    cpid = fork();
    if(cpid == 0) {
      cpid = getpid();
      //X[] specific code goes here
      if(i < sizeX) {
	originalIndex = i;
	modifiedIndex = i;
	int number = x[i];
	sprintf(buff, "      $$$ M-PROC(%d): handling x[%d] = %d\n", (int) cpid, originalIndex, number);
	write(1, buff, strlen(buff));
	//Special case, if the number is smaller than y[0], its index stays same in new array
	if(number < y[0]) {
	  finalIndex = modifiedIndex;
	  sprintf(buff, "      $$$ M-PROC(%d): x[%d] = %d is found to be smaller than y[0] = %d\n", (int) cpid, originalIndex, number, y[0]);
	  write(1, buff, strlen(buff));
	  m[i] = number;
	  exit(1);
	}

        //If the number is greater than everything in Y, it goes to the front of the new array
	else if(number > y[sizeY-1]) {
	  finalIndex = sizeY + originalIndex;
	  sprintf(buff, "      $$$ M-PROC(%d): x[%d] = %d is found to be larger than y[%d] = %d\n",
		  (int) cpid, originalIndex, number, sizeY-1, y[sizeY-1]);
	  write(1, buff, strlen(buff));
	  m[finalIndex] = number;
	  exit(1);
	}
	
	//determines array position
	else {
	  int k = binarySearch(y, 0, sizeY-1, number, i, 1, buff);
	  if(k == -1) {
	    exit(1);
	  }
	  
	  else {
	    sprintf(buff, "      $$$ M-PROC(%d): about to write x[%d] = %d into position %d of the output array\n",
		    (int) cpid, i, number, k+1+i);
	    write(1, buff, strlen(buff));
	    m[k+i+1] = number;
	    exit(1);
	  }
	}
	
	exit(1);
      }
     
    
      //y specific code
      else {
	originalIndex = i - sizeX;
	modifiedIndex = i;
	int number = y[originalIndex];
	//if smallest, go the begining of the new array
	if(number < x[0]) {
	  m[modifiedIndex] = number;
	  exit(1);
	}

	//if biggest, go to front of the array
	else if(number > x[sizeX-1]) {
	  finalIndex = sizeX + originalIndex;
	  m[finalIndex] = number;
	  exit(1);
	}
	
	//find position
	else {
	  int k = binarySearch(x, 0, sizeX-1, number, originalIndex, 0, buff);
	  if(k == -1) {
	    exit(1);
	  }
	  
	  else {
	    sprintf(buff, "      $$$ M-PROC(%d): about to write y[%d] = %d into position %d of the output array\n",
		    (int)  cpid, i, number, k+1+originalIndex);
	    write(1, buff, strlen(buff));
	    m[k+originalIndex+1] = number;
	    exit(1);
	  }
	}
	
	
      }
      
      exit(1);
    }
  }
  //wait for all children
  while((wpid = wait(&status)) > 0);

  //detach memory
  shmdt(x);
  shmdt(y);
  shmdt(m);
  return 0;
}
