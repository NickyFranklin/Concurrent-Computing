#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <sstream>
#include "thread.h"

//-------------------------------------------------------------
//Name: Nicky Franklin            UserID:nafrankl
//Due Date: 10/16/2023
//Program Assignment 3
//File Name: thread-main.cpp
//Program Purpose:
//Drives the main code that is used to calculate the prefix sum
//of an array using concurrent threads to do it
//-------------------------------------------------------------

using namespace std;
int **B;

//--------------------------------------------------------------------------------------
//Function main:
//   Calculate the prefix sum of an array concurrently
//Parameter Usage:
//char buf: Used to write strings to stdout during concurrent parts of the code
//int n: Size of the array taken in
//int *x: The array of numbers that are taken in
//int a: Hold the value of log base2(n)
//int temp: holds the value of n to be divided by two while calculating log base2(n)
//int b: the number of rows of B
//int k: the number of rows
//PreFix prefix[n]: The number of threads to be created each run
//int ** B: The array that holds all the prefix sum steps
//Functions Called:
//setPreFix
//Join
//Begin
//----------------------------------------------------------------------------------------

int main(void) {
  //Declare variables and set them
  char buf[1000];
  int n;
  int *x;
  cin >> n;
  cout << "Number of input data = " << n << endl;
  
  x = new int[n];

  cout << "Input Array:" << endl;
  for(int i = 0; i < n; i++) {
    cin >> x[i];
    cout << x[i] << "   ";
  }
  cout << endl;

  //Calculate log base 2 of size
  int a = 0;
  int temp = n;
  while(temp != 1) {
    temp /= 2;
    a++;
  }
  int b = a;

  //Allocate memory for B
  B = new int*[b+1];
  for(int i = 0; i < b+1; i++) {
    B[i] = new int[n];
  }

  //Set the first row of B to the original array
  for(int i = 0; i < n; i++) {
    B[0][i] = x[i];
  }
  
  //Set up all the prefix sum objects for each thread
  PreFix prefix[n];
  for(int k = 0; k < n; k++) {
     prefix[k].setPreFix(n, x, 1, k);
  }

  //Get number of rows
  int k = prefix[0].k;

  //Create N threads per row and calculate the prefix sum
  for(int i = 1; i <= k; i++) {
    cout << "Run " << i << ":" << endl;
    for(int j = 0; j < n; j++) {
      prefix[j].Begin();
      sprintf(buf, "      Thread %d Created\n", j);
      write(1, buf, strlen(buf));
    }
    
    for(int j = 0; j < n; j++) {
      prefix[j].Join();
    }

    cout << "Result after run " << i << ":" << endl;
    for(int p = 0; p < n; p++) {
      cout << B[i][p] << " ";
    }
    cout << endl;
    
    if(i == k) {
      cout << endl;
      cout << "Final result after run " << i << ":" << endl;
      for(int p = 0; p < n; p++) {
	cout << B[i][p] << " ";
      }
      cout << endl;
    }
  }
  
  Exit();
  return 0;
}
