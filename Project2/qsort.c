#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
/*---------------------------------------------------------------------------*/
/* NAME: Nicky Franklin                                   User ID:nafrankl   */
/* DUE DATE: 10/4/2023                                                       */
/* PROGRAM ASSIGNMENT 2                                                      */
/* FILE NAME: qsort.c                                                        */
/* PROGRAM PURPOSE: To sort an array concurrently using qsort                */
/*---------------------------------------------------------------------------*/

/* ----------------------------------------------------------- */
/* FUNCTION swap                                               */
/* Purpose: swaps two elements in an array                     */
/* PARAMETER USAGE :                                           */
/* int* a, int* b: The elements in the array to be switched    */
/* FUNCTION CALLED :                                           */
/* n/a                                                         */
/* ----------------------------------------------------------- */
void swap(int* a, int* b) {
  int s = *a;
  *a = *b;
  *b = s;
}

/* ----------------------------------------------------------- */
/* FUNCTION partition                                          */
/* Purpose: Sort the pivot of the array so that it may be      */
/* sorted quickly                                              */
/* PARAMETER USAGE :                                           */
/* int arr[]: The array that has the values we are partitioning*/
/* int low: the low index of the array                         */
/* int high: the high index of the array                       */
/* FUNCTION CALLED :                                           */
/* swap                                                        */
/* ----------------------------------------------------------- */
int partition(int arr[], int low, int high) {
  char buff[1000];
  int pivot = arr[high];
  pid_t cpid = getpid();
  sprintf(buff, "   ### Q-PROC(%d): pivot element is a[%d] = %d\n", cpid, high, arr[high]); 
  write(1, buff, strlen(buff));
  int lowIndex = low - 1;
  //determines points that needs to be swapped
  for(int i = low; i <= high; i++) {
    if(arr[i] < pivot) {
      lowIndex++;
      swap(&arr[lowIndex], &arr[i]);
    }
  }
  swap(&arr[lowIndex+1], &arr[high]);
  return lowIndex+1;
}

/* ----------------------------------------------------------- */
/* FUNCTION main                                               */
/* Purpose: sort an array concurrently using qsort             */
/* PARAMETER USAGE :                                           */
/* int argc: # arguments for program                           */           
/* char** argv: arguments for program                          */
/* FUNCTION CALLED :                                           */
/* partition                                                   */
/* ----------------------------------------------------------- */
int main(int argc, char** argv) {
  if(argc < 3) {
    printf("it messed up \n");
    exit(1);
  }
  //Initialize the variables
  int *arr;
  pid_t cpid;
  arr = shmat(atoi(argv[3]), NULL, 0);
  cpid = getpid();
  char buff[1000];
  sprintf(buff, "   ### Q-PROC(%d): entering with a[%d..%d]\n   ", cpid,
	  atoi(argv[1]), atoi(argv[2]));
  int length = strlen(buff);
  int size = (atoi(argv[4]));
  int j = atoi(argv[1]);
  //Print the array numbers
  for(int i = length; i < 1000 && j < size+atoi(argv[1]) ; i+=0) {
    sprintf(buff+i, "%d ", arr[j]);
    i = strlen(buff);
    j++;
  } 
  sprintf(buff+strlen(buff), "\n");
  write(1, buff, strlen(buff));
  int low = atoi(argv[1]);
  int high = atoi(argv[2]);
  //if the values are valid, partition the arrray
  if(low < high) {
    int part = partition(arr, atoi(argv[1]), atoi(argv[2]));
    int status = 0;
    pid_t wpid;
    
    if(atoi(argv[1]) <= part-1) {
      cpid = fork();
      if(cpid == 0) {
	char *args[6];
	for(int i = 0; i < 6; i++) {
	  args[i] = (char*) malloc(100);
	}
	args[0] = "./qsort";
	sprintf(args[1], "%d", atoi(argv[1]));
	sprintf(args[2], "%d", (part-1));
	sprintf(args[3], "%d", atoi(argv[3]));
	sprintf(args[4], "%d", (part - low));
	args[5] = NULL;
	execvp(args[0], args);
	printf("\nWhoops\n");
      }
    }
  
    if(part+1 <= atoi(argv[2])) {
      cpid = fork();
      if(cpid == 0) {
	char *args[6];
	for(int i = 0; i < 6; i++) {
	  args[i] = (char*) malloc(100);
	}
	args[0] = "./qsort";
	sprintf(args[1], "%d", (part + 1));
	sprintf(args[2], "%d", atoi(argv[2]));
	sprintf(args[3], "%d", atoi(argv[3]));
	sprintf(args[4], "%d", (high - part));
	args[5] = NULL;
	execvp(args[0], args);
	printf("\nWhoops\n");
      }
    }
    //Wait for children
    while((wpid = wait(&status)) > 0);
  }
  //print out ending information
  sprintf(buff, "   ### Q-PROC(%d): section a[%d..%d] sorted", cpid,
	  atoi(argv[1]), atoi(argv[2]));
  length = strlen(buff);
  size = (atoi(argv[4]));
  //printf("pid %d size:%d\n", cpid, size);
  j = atoi(argv[1]);
  if(size != 1) {
    sprintf(buff+length, "\n   ");
    length = strlen(buff);
    for(int i = length; i < 1000 && j < size+atoi(argv[1]) ; i+=0) {
      sprintf(buff+i, "%d ", arr[j]);
      i = strlen(buff);
      j++;
    }
  }
  sprintf(buff+strlen(buff), "\n");
  write(1, buff, strlen(buff));
  sprintf(buff, "   ### Q-PROC(%d): exits\n", cpid);
  write(1, buff, strlen(buff));
  shmdt(arr);
  exit(0);
}
