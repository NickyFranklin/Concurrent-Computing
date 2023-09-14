#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
using namespace std;

int main(int argc, char* argv[]) {
  pid_t c_pid, w_pid;
  FILE *fp;
  fp = fopen("file.txt", "wb");
  char editString[500];
  strcpy(editString, "file.txt");
  int length = strlen(editString);
  char* edit;
  edit = strrchr(editString, '.');
  edit[0] = '%';
  edit[1] = 'd';
  edit[2] = '.';
  edit[3] = 'p';
  edit[4] = 'p';
  edit[5] = 'm';
  cout << editString << "\n";
  int start = 0;
  int total = 1000;
  int procs = 5;
  int increment = 1000 / 5;
  for(int i = 0; i < 5; i++) {
    if((c_pid = fork()) == 0) {
      sprintf(editString, editString, i+6);
      fp = fopen(editString, "wb");
      cout << "printed from child " << getpid() << "\n";
      fclose(fp);
      exit(0);
    }
  }
  while((w_pid = wait(0)) > 0);
  cout << "printed from parent " << getpid() << "\n";
  fclose(fp);
  return 0;
}
