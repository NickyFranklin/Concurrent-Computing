#ifndef THREADCLASS_H
#define THREADCLASS_H
#include "ThreadClass.h"
#endif
//--------------------------------------------------
//Name: Nicky Franklin         User Id: nafrankl
//Due Date: 10/16/23
//Program Assignment 3
//File Name: thread.h
//Program Purpose:
//Provide class definitions for the thread.cpp file
//----------------------------------------------------
class Baby : public Thread {
 public:
  Baby();
  void ready_to_eat();
  void finish_eating();
  ~Baby();
  int numberEagle;
  int numberPot;

 private:
  void ThreadFunc();
  
};

class Mother : public Thread {
 public:
  Mother();
  void goto_sleep();
  void food_ready();
  ~Mother();
  int m;
  int n;
  int t;
  int numbersFed;
  bool isRetired;
  bool retire();

 private:
  void ThreadFunc();
  
};
