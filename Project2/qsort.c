#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

int main(int argc, char** argv) {
  write(1, "qsort.c\n", sizeof("qsort.c\n"));
  exit(0);
}
