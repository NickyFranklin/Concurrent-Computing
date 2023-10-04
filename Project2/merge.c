#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int binarySearch(int arr[], int l, int r, int x, int i, int isX, char *buff) {
  while( l <= r) {
    int k = l + (r-l) / 2;
    pid_t cpid = getpid();
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
    
    else if(arr[k] > x) {
      return binarySearch(arr, l, k -1, x, i, isX, buff);
    }

    
    return binarySearch(arr, k+1, r, x, i, isX, buff);
    
     
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
  //int length = strlen(buff);
  
  wpid = getpid();
  int originalIndex;
  int modifiedIndex;
  int finalIndex;
  int number = 0;
  if(number) {

  }
  for(int i = 0; i < (sizeX+sizeY); i++) {
    cpid = fork();
    if(cpid == 0) {
      cpid = getpid();
      if(i < sizeX) {
	originalIndex = i;
	modifiedIndex = i;
	int number = x[i];
	sprintf(buff, "      $$$ M-PROC(%d): handling x[%d] = %d\n", (int) cpid, originalIndex, number);
	write(1, buff, strlen(buff));
	if(number < y[0]) {
	  finalIndex = modifiedIndex;
	  sprintf(buff, "      $$$ M-PROC(%d): x[%d] = %d is found to be smaller than y[0] = %d\n", (int) cpid, originalIndex, number, y[0]);
	  write(1, buff, strlen(buff));
	  m[i] = number;
	  exit(1);
	}

	else if(number > y[sizeY-1]) {
	  finalIndex = sizeY + originalIndex;
	  sprintf(buff, "      $$$ M-PROC(%d): x[%d] = %d is found to be larger than y[%d] = %d\n",
		  (int) cpid, originalIndex, number, sizeY-1, y[sizeY-1]);
	  write(1, buff, strlen(buff));
	  m[finalIndex] = number;
	  exit(1);
	}
	
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
     
    
      
      else {
	originalIndex = i - sizeX;
	modifiedIndex = i;
	int number = y[originalIndex];
	if(number < x[0]) {
	  m[modifiedIndex] = number;
	  exit(1);
	}

	
	else if(number > x[sizeX-1]) {
	  finalIndex = sizeX + originalIndex;
	  m[finalIndex] = number;
	  exit(1);
	}
	
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
  
  while((wpid = wait(&status)) > 0);

  
  shmdt(x);
  shmdt(y);
  shmdt(m);
  return 0;
}
