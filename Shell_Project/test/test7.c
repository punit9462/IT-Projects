#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#define N 5
int main(){
//char cmdline[MAXLINE]; 
//    while (1) {
	/* read */
/*	printf("> ");                   
	Fgets(cmdline, MAXLINE, stdin); 
	if (feof(stdin))
	    exit(0);
*/
	/* evaluate */
//	eval(cmdline);
    
/*if(fork()==0){
sleep(5);
printf("child: %d grid:%d \n",getpid(),getpgrp());
exit(0);
}
else{
printf("parent: %d grid:%d \n",getpid(),getpgrp());
exit(0);
}*/
pid_t pid[N], wpid;
    int   child_status, i;
    for (i = 0; i < N; i++)
	if ((pid[i] = fork()) == 0)
	    while (1){
		if(i==2) exit(100+i);
	} /* Child infinite loop */

    /* Parent terminates the child processes */
    for (i = 0; i < N; i++) {
	printf("Killing process %d\n", pid[i]);
	if(i!=2)kill(pid[i], SIGINT);
    }

    /* Parent reaps terminated children */
    for (i = 0; i < N; i++) {
	wpid = wait(&child_status);
	if (WIFEXITED(child_status))
	    printf("Child %d terminated with exit status %d\n",
		   wpid, WEXITSTATUS(child_status));
	else
	    printf("Child %d terminated abnormally\n", wpid);
    }
}
 
