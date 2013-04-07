#include<stdlib.h>
#include<stdio.h>

/*void cleanup(void) {
   printf("cleaning up\n");
}*/
int main(void) {
/*   atexit(cleanup);
   if (fork() == 0)
      printf("hello from child\n");
   else
      printf("hello from parent\n");
   exit(0);
*/
/*    if (fork() == 0) {
	* Child */
/*	printf("Terminating Child  PID = %d\n",getpid());
	exit(0);
    } else {
	printf("Running Parent, PID = %d\n", getpid());
	while (1) ; * Infinite loop */
if (fork() == 0) {
	/* Child */
	printf("Running Child, PID = %d\n", getpid());
//	while (1) ; /* Infinite loop */
        exit(-1);
    } else {
        int child_status;
	wait(&child_status);
	printf("Terminating Parent, PID = %d\nstatus:%d ", getpid(),child_status);
	exit(0);
    }  
return;  
}
