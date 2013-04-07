#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main()
{
    pid_t pid[5], wpid;
    int   child_status, i;
    for (i = 0; i < 5; i++)
	if ((pid[i] = fork()) == 0)
	    exit(100 + i); /* Child */
    for (i = 0; i < 5; i++) {
	wpid = waitpid(pid[i], &child_status, 0);
	if (WIFEXITED(child_status))
	    printf("Child %d terminated with exit status %d -> %d\n",
	           wpid, WEXITSTATUS(child_status),pid[i]);
	else
	    printf("Child %d terminated abnormally\n", wpid);
    }
}
