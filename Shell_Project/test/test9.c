#include <stdio.h> 
#include<stdlib.h>
#include <signal.h>
#include <stdbool.h> 
#define N 4
//int ccount=0; 
//int ticks = 5; 
void child_handler(int sig)
{
  //  int   child_status;
  //  pid_t pid = wait(&child_status);
  //  ccount -= 1;
    printf("Sig child terminated..:)..\n");
}
/*void child_handler2(int sig)
{
    int   child_status;
    pid_t pid;
    while ((pid = waitpid(-1, &child_status,WNOHANG)) > 0) {
	ccount -= 1;
	printf("Received signal %d from process %d\n", sig, pid);
    }
}

void example(void)
{int i;
pid_t pid;   
 printf("Example entered...\n");
    signal(SIGCHLD, child_handler2);
    printf("Example exting..\n");
	for(i=0;i<3;i++){
           if(pid=fork()==0)
           exit(i);}
//sleep(100);
return ;
}
void example(void)
{
    pid_t pid[N];
    int   child_status, i;
    ccount = N;
    signal(SIGCHLD, child_handler);
    for (i = 0; i < N; i+=1)
	if ((pid[i] = fork()) == 0) {
	    * Child: Exit */
//	    exit(0);
//	}
 //   while (ccount > 0)
//	pause();/* Suspend until signal occurs */
//} 
/*void sigalrm_handler(int sig) { 
  printf("tick\n");
 
  ticks -= 1;
  if (ticks > 0) {
   // signal(SIGALRM,sigalrm_handler);
    alarm(1); 
  } else { 
    printf("*BOOM!*\n"); 
    exit(0); 
  } 
} 
*/
int main(void) { 
signal(SIGINT,child_handler);
if(fork()==0){
	while(1){
	{
	printf("I am child..\n");
	sleep(1);
	}
	}
}
else{
wait(NULL);
printf("I am parent..\n");
}
return 0;
}

