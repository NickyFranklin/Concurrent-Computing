#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void swap(int* a, int* b) {
  int s = *a;
  *a = *b;
  *b = s;
}

int partition(int arr[], int low, int high) {
  char buff[1000];
  int pivot = arr[high];
  pid_t cpid = getpid();
  sprintf(buff, "   ### Q-PROC(%d): pivot element is a[%d] = %d\n", cpid, high, arr[high]); 
  write(1, buff, strlen(buff));
  int lowIndex = low - 1;

  for(int i = low; i <= high; i++) {
    if(arr[i] < pivot) {
      lowIndex++;
      swap(&arr[lowIndex], &arr[i]);
    }
  }
  swap(&arr[lowIndex+1], &arr[high]);
  return lowIndex+1;
}

int main(int argc, char** argv) {
  if(argc < 3) {
    printf("it messed up \n");
    exit(1);
  }
  int *arr;
  pid_t cpid;
  arr = shmat(atoi(argv[3]), NULL, 0);
  cpid = getpid();
  char buff[1000];
  sprintf(buff, "   ### Q-PROC(%d): entering with a[%d..%d]\n   ", cpid,
	  atoi(argv[1]), atoi(argv[2]));
  int length = strlen(buff);
  int size = (atoi(argv[4]));
  //printf("pid %d size:%d\n", cpid, size);
  int j = atoi(argv[1]);
  for(int i = length; i < 1000 && j < size+atoi(argv[1]) ; i+=0) {
    sprintf(buff+i, "%d ", arr[j]);
    i = strlen(buff);
    j++;
  } 
  sprintf(buff+strlen(buff), "\n");
  write(1, buff, strlen(buff));
  int low = atoi(argv[1]);
  int high = atoi(argv[2]);
  //printf("low: %d \n high: %d \n", low, high);
  if(low < high) {
    int part = partition(arr, atoi(argv[1]), atoi(argv[2]));
    int status = 0;
    pid_t wpid;
    //int low = atoi(argv[1]);
    //int high = atoi(argv[2]);
    //printf("%d %d\n", low, part-1);
    //printf("%d %d\n", part+1, high);
    
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
    while((wpid = wait(&status)) > 0);
  }
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
