#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

int main(int argc, char* argv[]) {
  pid_t c_pid, w_pid;
  
  for(int i = 0; i < 5; i++) {
    if((c_pid = fork()) == 0) {
      cout << "printed from child " << getpid() << "\n";
      for(int i = 0; i < 5000000000000000; i++);
      exit(0);
    }
  }
  while((w_pid = wait(0)) > 0);
  cout << "printed from parent " << getpid() << "\n";
  return 0;
}
