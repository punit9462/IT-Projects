#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
int count=0;
void handler1(int sig)
{
    printf("Handler executed with value: %d\n",count);
    exit(0);
}

int main()
{
        if (fork() == 0) {
	 signal(SIGINT,handler1);	 
	   printf("Hello from child %d\n", (int)getpid());
            sleep(100);
//	    sleep(100);
        }
	else{
	count++;
	wait(NULL);
    	printf("Parent processing input\n");
	}    
return;
}

