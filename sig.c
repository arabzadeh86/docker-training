#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void print_signal(int);

int main(int argc,char *arv[])
{
  int i;
  for(i=1;i<16;i++){
    signal(i, print_signal);
  }
  sleep(100);
  return 0;
}

void print_signal(int signal)
{
  printf("Signal nubmer: %d is received.\n", signal);
}
