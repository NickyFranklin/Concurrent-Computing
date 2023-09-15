#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include<stdio.h>
#include<stdlib.h>
using namespace std;

typedef struct {
  unsigned char r,g,b;
} Pixel;

Pixel* readPPM(FILE *fp) {
  char fileFormat[4];
  int width, height, colorValue;
  fscanf(fp, "%s %d %d %d", fileFormat, &width, &height, &colorValue);
  printf("%s %d %d %d\n", fileFormat, width, height, colorValue);
  Pixel *pixelArr = (Pixel *)malloc(width*height*sizeof(Pixel));
  Pixel pixel;
  unsigned char c = fgetc(fp);
  for(int i = 0; i < (width*height); i++) {
    pixel.r = fgetc(fp);
    pixel.g = fgetc(fp);
    pixel.b = fgetc(fp);
    //printf("%d %d %d %d\n", c, pixel.r, pixel.g, pixel.b);
    pixelArr[i] = pixel;
  }
  return pixelArr;
}

void writePPM() {

}

int main(){
  FILE* fp = fopen("pic1.ppm", "rb");
  Pixel *pixelArr;
  pixelArr = readPPM(fp);
  Pixel pixel;
  for(int i = 0; i < 2; i++) {
    pixel = pixelArr[i];
    printf("%d %d %d\n", pixel.r, pixel.g, pixel.b);
  }
  free(pixelArr);
  return(0);
}

