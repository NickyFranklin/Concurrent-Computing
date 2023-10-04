#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

/*---------------------------------------------------------------------------*/
/* NAME: Nicky Franklin                                   User ID:nafrankl   */
/* DUE DATE: 10/4/2023                                                       */
/* PROGRAM ASSIGNMENT 2                                                      */
/* FILE NAME: main.c                                                         */
/* PROGRAM PURPOSE: To take in unsorted arrays and sort them concurrently    */
/*---------------------------------------------------------------------------*/

void fillArray(int *arr[], int size) {
  *arr = (int*) malloc((size) * sizeof(int));
  for(int i = 0; i < size; i++) {
    scanf("%d", &(*arr)[i]);
  }
}

void printArray(int arr[], int isQsort, int isX, int size) {
  if(isQsort) {
    printf("Input array for qsort has %d element:\n", size);
  }

  else if(isX) {
    printf("Input array x[] for merge has %d element:\n", size);
  }

  else {
    printf("Input array y[] for merge has %d element:\n", size);
  }
  
  for(int i = 0; i < size; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n\n");
}

int main() {
  int *a;
  int *x;
  int *y;
  int sizeQ;
  int sizeX;
  int sizeY;
  scanf("%d", &sizeQ);
  printf("%d\n", sizeQ);
  fillArray(&a, sizeQ);
  scanf("%d", &sizeX);
  fillArray(&x, sizeX);
  scanf("%d", &sizeY);
  fillArray(&y, sizeY);
  
  int ShmIDQSort;
  key_t keyQsort;
  int *qSortPtr;

  int ShmIDx;
  key_t keyx;
  int *xPtr;

  int ShmIDy;
  key_t keyy;
  int *yPtr;
  
  keyQsort = ftok("main.c", 'q');
  printf("*** MAIN: qsort shared memory key = %d\n", keyQsort);
  
  ShmIDQSort = shmget(keyQsort, sizeof(int) * (sizeQ), IPC_CREAT | 0666);
  if(ShmIDQSort < 0) {
    printf("Uh oh, the shared memory segment was not created\n");
    return 1;
  }
  printf("*** MAIN: qsort shared memory created\n");

  qSortPtr = (int*) shmat(ShmIDQSort, NULL, 0);
  printf("*** MAIN: qsort shared memory attached and ready to use\n\n");

  
  keyx = ftok("main.c", 'x');
  printf("*** MAIN: merge x[] shared memory key = %d\n", keyx);
  
  ShmIDx = shmget(keyx, sizeof(int) * (sizeX), IPC_CREAT | 0666);
  if(ShmIDx < 0) {
    printf("Uh oh, the shared memory segment was not created\n");
    return 1;
  }
  printf("*** MAIN: merge x[] shared memory created\n");

  xPtr = (int*) shmat(ShmIDx, NULL, 0);
  printf("*** MAIN: merge x[] shared memory attached and ready to use\n\n");


  keyy = ftok("main.c", 'y');
  printf("*** MAIN: merge y[] shared memory key = %d\n", keyy);
  
  ShmIDy = shmget(keyy, sizeof(int) * (sizeY), IPC_CREAT | 0666);
  if(ShmIDy < 0) {
    printf("Uh oh, the shared memory segment was not created\n");
    return 1;
  }
  printf("*** MAIN: merge y[] shared memory created\n");

  yPtr = (int*) shmat(ShmIDy, NULL, 0);
  printf("*** MAIN: merge y[] shared memory attached and ready to use\n\n");

  
  //fillArray(qSortPtr);
  //fillArray(xPtr);
  //fillArray(yPtr);
  for(int i = 0; i < sizeQ; i++) {
    qSortPtr[i] = a[i];
  }

  for(int i = 0; i < sizeX; i++) {
    xPtr[i] = x[i];
  }

  for(int i = 0; i < sizeY; i++) {
    yPtr[i] = y[i];
  }
  
  printArray(qSortPtr, 1, 0, sizeQ);
  printArray(xPtr, 0, 1, sizeX);
  printArray(yPtr, 0, 0, sizeY);

  printf("*** MAIN: about to spawn the process for qsort\n");
  printf("*** MAIN: about to spawn the process for merge\n");

  pid_t wpid;
  pid_t cpid = fork();
  if(cpid < 0) {
    printf("Uh oh\n");
    return 0;
  }
 
  //qsort
  if(cpid == 0) {
    char *argv[6];
    for(int i = 0; i < 6; i++) {
      argv[i] = (char*) malloc(100);
    }
    argv[0] = "./qsort";
    sprintf(argv[1], "%d", 0);
    sprintf(argv[2], "%d", atoi(argv[1]) + sizeQ-1);
    sprintf(argv[3], "%d", ShmIDQSort);
    sprintf(argv[4], "%d", sizeQ);
    argv[5] = NULL;
    execvp(argv[0], argv);
    printf("oopsies\n");
  }
  

  //merge
  cpid = fork();

  if(cpid < 0) {
    printf("Uh oh\n");
    return 0;
  }
  
  if(cpid == 0) {
    char* argv[4];
    for(int i = 0; i < 4; i++) {
      argv[i] = (char*) malloc(100);
    }
    argv[0] = "./merge";
    sprintf(argv[1], "%d", ShmIDx);
    sprintf(argv[2], "%d", ShmIDy);
    argv[3] = NULL;
    execvp(argv[0], argv);
    printf("whoopsies\n");
  }

  int status = 0;
  while((wpid = wait(&status)) > 0);
  
  shmdt(qSortPtr);
  printf("*** MAIN: qsort shared memory successfully detached\n");
  shmctl(ShmIDQSort, IPC_RMID, NULL);
  printf("*** MAIN: qsort shared memory successfully removed\n\n");
  
  shmdt(xPtr);
  printf("*** MAIN: merge x[] shared memory successfully detached\n");
  shmctl(ShmIDx, IPC_RMID, NULL);
  printf("*** MAIN: merge x[] shared memory successfully removed\n\n");
   
  shmdt(yPtr);
  printf("*** MAIN: merge y[] shared memory successfully detached\n");
  shmctl(ShmIDy, IPC_RMID, NULL);
  printf("*** MAIN: merge y[] shared memory successfully removed\n\n");

  free(a);
  free(x);
  free(y);
  return 0;
}
