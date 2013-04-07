//#irm returned..: %d",alarm(4));
#include<stdlib.h>
#include<stdio.h>
#include<signal.h>
/*int main()
{
  printf("alarm returned..: %d\n",alarm(4));
  while (1) {
     printf("Going to sleep\n");
     sleep(1);
  }
  printf("Exiting\n"); // will not be executed
  exit(0);
}*/
int main()
{
  alarm(4);
  int pid = fork();
  while (1) {
     if (pid == 0) printf("I am the child\n");
     else printf("I am the parent\n");
     sleep(1);
  }
}

