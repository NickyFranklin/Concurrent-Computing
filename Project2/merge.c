#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

int main(int argc, char** argv) {
  write(1, "merge.c\n", sizeof("merge.c\n"));
  return 0;
}
