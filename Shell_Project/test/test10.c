//#include "csapp.h"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
int counter = 0;

void handler(int sig) 
{
    counter++;
//    sleep(1); /* Do some work in the handler */
    return;
}
int main() 
{
    int i;

    signal(SIGUSR2, handler);

    if (fork() == 0) {  /* Child */
	for (i = 0; i < 5; i++) {
	    kill(getppid(), SIGUSR2);
	    printf("sent SIGUSR2 to parent\n");
	sleep(1);
	}
	exit(0);
    }

    wait(NULL);
    printf("counter=%d\n", counter); 
    exit(0);
}
