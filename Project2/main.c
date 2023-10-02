#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

void fillArray(int* size, int **arr) {
  scanf("%d", size);
  *arr = (int*) calloc(*size, sizeof(int));
  for(int i = 0; i < *size; i++) {
    scanf("%d", &(*arr)[i]);
  }
}

void printArray(int size, int *arr) {
  printf("%d\n", size);
  for(int i = 0; i < size; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

int main() {
  //input array a size
  int k;
  //input array x size
  int m;
  //input array y size
  int n;
  
  int *a;
  int *x;
  int *y;
  
  fillArray(&k, &a);
  fillArray(&m, &x);
  fillArray(&n, &y);
  printArray(k, a);
  printArray(m, x);
  printArray(n, y);
  
}
