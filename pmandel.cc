#include "mandel.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
//-----------------------------------------------------------------
// Computes members of mandelbrot for specified region
// of the real/imaginary plane. Creates PPM image file
// according to one of several color schemes.
//-----------------------------------------------------------------
//
// Compilation: g++ -c mandel.cc
//              g++ -c mandelPlot.cc
//              g++ -o mandelPlot mandelPlot.o mandel.o -lm
//
// There is a makefile in the project directory
//-----------------------------------------------------------------
//
// Invocation: mandelPlot RealCoord ImagCoord RealSideLength ComplexSideLength EscapeIterations HorizontalPixels VerticalPixels ImageFile
//
// See the project handout for a detailed description of the parameters and
// operation of the program.
//------------------------------------------------------------------

//------------------------------------------------------------------
//The code that was originally written in the mandelbrot Plot code will be
//denoted by these blocks before and after the code. This counts as one of these blocks
//------------------------------------------------------------------
#define PRINTBUFSIZE 241
#define COLORS 15 
unsigned char palette[COLORS][3] = {{255,255,255},{255,0,0},{255,128,0},{255,255,0},{128,255,0},{0,255,0},
				    {0,255,128},{0,255,255},{0,128,255},{0,0,255},{128,0,225},{255,0,255},{255,0,128},{128,128,128},{0,0,0}};

//------------------------------------------------------------------
// Associate a color with one point and add point values
// to the PPM file.  See the project handout for a more complete
// description.
//------------------------------------------------------------------
void addPoint(int scheme, int iterations, int maxiterations, FILE *fp){

  unsigned char color[3];
  int index;

  //
  //--- Three possible schemes for coloring each point.
  //
  if (scheme==1){
      // Scheme 1 - Greyscale uniform 
      color[0]=255 - ((double)iterations/(double)maxiterations)*255;
      color[1]=255 - ((double)iterations/(double)maxiterations)*255;
      color[2]=255 - ((double)iterations/(double)maxiterations)*255;
  }
  if ((scheme==2)||(scheme==3)){
       if (scheme==2){
            iterations=sqrt( (double)iterations/(double)maxiterations)*(float)(maxiterations);
       }
       
       if (iterations==maxiterations)
	        index=COLORS-1; //-- Points in the set are always black, assumed last in palette
       else
            index=iterations/(maxiterations/(COLORS-1));

       color[0]=palette[index][0];
       color[1]=palette[index][1];
       color[2]=palette[index][2];
  }            

  //-- Add the point the file
  fputc(color[0],fp);
  fputc(color[1],fp);
  fputc(color[2],fp);
}
//
//------------ Main 
//
int main(int argc, char *argv[]){
  
  dblcpoint start;       //-- Top left corner
  double hrange;         //-- Length of a side in real plane
  double vrange;         //-- Length of a side in complex plane
  int hpixels;           //-- Pixels on the real side
  int vpixels;           //-- Pixels on the imaginary side
  double hincrement;     //-- Length between pixels in complex plane
  double vincrement;     //-- Length between pixels in complex plane
  int maxiterations;     //-- Number of iterations use to identify points in the set
  int i;                 //-- Loop index for recurrence relation
  FILE *fp;              //-- Descriptor for file to hold image
  int colorscheme;       //-- Color scheme for the plot
  char printBuf[PRINTBUFSIZE];     //-- Output buffer  
  MandelPointRegion *mandelRegion; //-- Point region -- contains points and methods to generate them
  MandelPoint point;               //-- One point
  int procs;
  pid_t c_pid, p_pid, w_pid;
  
  
  //
  // -- Process the input arguments
  //
  if(argc != 10) {
    printf("mandel RealCoord ImagCoord RealSideLength ImagSideLength EscapeIterations HorizontalPixels VerticalPixels ImageFile Nprocs\n");
    exit(1);
  } 

  start = {
    strtod(argv[1],NULL) , strtod(argv[2],NULL)
  };
  printf("Top left coordinate is: %f + %fi\n", start.real(), start.imag());  

  hrange = strtod(argv[3],NULL);
  if (hrange<=0){
    printf("Real side must be greater than zero.\n");
    exit(1);
  }
  printf("Length of real side:  %f\n",hrange);

  vrange=strtod(argv[4],NULL);
  if (vrange<=0){
    printf("Imaginary side must be greater than zero.\n");
    exit(1);
  }
  printf("Length of vertical side:  %f\n",vrange);
  
  maxiterations=atoi(argv[5]);
  if (maxiterations<2){
    printf("Max iterations must be at least 2.\n");
    exit(1);
  }
  
  hpixels=atoi(argv[6]);
  printf("Pixels on horizontal side:  %d\n",hpixels);
  if (hpixels<10){
    printf("Pixels on horizontal side must be at least 10.\n");
    exit(1);
  }

  vpixels=atoi(argv[7]);
  printf("Pixels on vertical side:  %d\n",vpixels);
  if (vpixels<10){
    printf("Pixels on horizontal side must be at least 10.\n");
    exit(1);
  }

  fp=fopen(argv[8], "wb"); /* b - binary mode */
  if (fp==NULL){
    printf("%s cannot be opened for write\n",argv[6]);
  }
  (void) fprintf(fp, "P6\n%d %d\n255\n", hpixels, vpixels);
  //------------------------------------------------------------------
  //The code that was originally written in the mandelbrot Plot code will be
  //denoted by these blocks before and after the code. This counts as one of these blocks
  //------------------------------------------------------------------
  //New code
  procs = atoi(argv[9]);
  
  //------------------------------------------------------------------
  //The code that was originally written in the mandelbrot Plot code will be
  //denoted by these blocks before and after the code. This counts as one of these blocks
  //------------------------------------------------------------------
    //
    //-- Output the calculation parameters
    //
  
  //The for loop and fork are new
  for(i = 0; i < procs; i++) {
    if((c_pid = fork()) == 0) {
      snprintf(printBuf,PRINTBUFSIZE,"Process %d testing rectangle at %.8f + %.8f \n\twidth %.8f and height %.8f \n\tplot area width %d by height %d pixels.\n",
	       getpid(),start.real(),start.imag(),hrange,vrange,hpixels,vpixels);
      write(1,printBuf,strlen(printBuf));
      //
      //--- Create a new point region and calculate the point values.  The "value" is
      //    the number of iterations before the recurrence value exceeds 2. If maxiterations
      //    is reached without exceeding 2, the point is not in the set.
      //
      mandelRegion=new MandelPointRegion(start,hrange,vrange,hpixels,vpixels,maxiterations);
      mandelRegion->ComputePoints();
      
      //
      //--- Now create the image file
      //
      
#ifndef COLORSCHEME
      colorscheme=3;
#else
      colorscheme=COLORSCHEME;
#endif
      
      for (i=0; i < hpixels*vpixels; i++)
	{
	  point=mandelRegion->getPoint(i);
	  addPoint(colorscheme,point.iterationsCompleted,maxiterations,fp);
	}
      
      //
      //-- Done
      //
      snprintf(printBuf,PRINTBUFSIZE,"Process %d done.\n",getpid());
      write(1,printBuf,strlen(printBuf));
      exit(0);
    }
  }
  while((w_pid = wait(0)) > 0);
  return 0;
}



//------------------------------------------------------------------
//The code that was originally written in the mandelbrot Plot code will be
//denoted by these blocks before and after the code. This counts as one of these blocks
//------------------------------------------------------------------
