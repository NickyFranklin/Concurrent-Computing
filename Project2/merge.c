#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char** argv) {
  if(argc < 3) {
    printf("it messed up\n");
    exit(1);
  }
  int *x;
  int *y;
  pid_t cpid;
  x = shmat(atoi(argv[1]), NULL, 0);
  y = shmat(atoi(argv[2]), NULL, 0);
  cpid = getpid();
  char buff[1000];
  int sizeX = x[0];
  int sizeY = y[0];
  sprintf(buff, "      $$$ M-PROC(%d): x: %d y: %d\n      ", cpid, sizeX, sizeY);
  int length = strlen(buff);
  int j = 1;
  for(int i = length; i < 1000 && j <= sizeX; i+=0) {
    sprintf(buff+i, "%d ", x[j]);
    i = strlen(buff);
    j++;
  }
  sprintf(buff+strlen(buff), "\n      ");

  length = strlen(buff);
  j = 1;
  for(int i = length; i < 1000 && j <= sizeY; i+=0) {
    sprintf(buff+i, "%d ", y[j]);
    i = strlen(buff);
    j++;
  }
  
  sprintf(buff+strlen(buff), "\n");
  write(1, buff, strlen(buff));
  shmdt(x);
  shmdt(y);
  return 0;
}
