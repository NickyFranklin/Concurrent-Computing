#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>

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
	  atoi(argv[1])-1, atoi(argv[2])-1);
  int length = strlen(buff);
  int size = arr[0];
  int j = 1;
  for(int i = length; i < 1000 && j <= size ; i+=0) {
    sprintf(buff+i, "%d ", arr[j]);
    i = strlen(buff);
    j++;
  }
  
  sprintf(buff+strlen(buff), "\n");
  write(1, buff, strlen(buff));
  shmdt(arr);
  exit(0);
}
