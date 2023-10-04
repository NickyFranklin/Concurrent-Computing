#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int binarySearch(int arr[], int l, int r, int x) {
  while( l <= r) {
    int k = l + (r-l) / 2;

    if(arr[k] <= x && arr[k+1] >= x) {
      return k;
    }
    
    else if(arr[k] > x) {
      return binarySearch(arr, l, k -1, x);
    }

    
    return binarySearch(arr, k+1, r, x);
    
     
  }
  return -1;
}


int main(int argc, char** argv) {
  if(argc < 4) {
    printf("\noops\n");
  }
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
  int length = strlen(buff);
  
  wpid = getpid();
  int originalIndex;
  int modifiedIndex;
  int finalIndex;
  int number;
  for(int i = 0; i < (sizeX+sizeY); i++) {
    cpid = fork();
    if(cpid == 0) {
      if(i < sizeX) {
	originalIndex = i;
	modifiedIndex = i;
	int number = x[i];
	if(number < y[0]) {
	  finalIndex = modifiedIndex;
	  printf("Number: %d\n index: %d\n", number, i);
	  m[i] = number;
	  exit(1);
	}

	else if(number > y[sizeY-1]) {
	  printf("Number: %d\n index: %d\n", number, i);
	  finalIndex = sizeY + originalIndex;
	  m[finalIndex] = number;
	  exit(1);
	}
	
	else {
	  printf("Number: %d\n index: %d\n", number, i);
	  int k = binarySearch(y, 0, sizeY-1, number);
	  if(k == -1) {
	    exit(1);
	  }
	  
	  else {
	    m[k+i] = number;
	    exit(1);
	  }
	}
	
	exit(1);
      }
     
    
      /*
      else {
	originalIndex = i - sizeX;
	modifiedIndex = i;
	int number = y[originalIndex];
	if(number < x[0]) {
	  finalIndex = modifiedIndex;
	  m[0] = number;
	}
	
	else if(number > x[sizeX-1]) {
	  finalIndex = sizeY + originalIndex;
	  m[finalIndex] = number;
	}
	
	else {
	  int k = binarySearch(x, 0, sizeX-1, number);
	  m[originalIndex+k] = number;
	}
      }
      */
      exit(1);
    }
  }
  
  while((wpid = wait(&status)) > 0);

  
  shmdt(x);
  shmdt(y);
  shmdt(m);
  return 0;
}
