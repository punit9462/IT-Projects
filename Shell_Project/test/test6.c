#include<stdlib.h>
#include<stdio.h>
#define N 10
#include<unistd.h>
int main()
{
/*    pid_t pid[N], wpid;
    int   child_status, i;
    for (i = 0; i < N; i++)
*/	//if ((pid[i] = fork()) == 0)
	 //   exit(100 + i); /* Child */
/*    for (i = 0; i <= N; i++) {
	wpid = wait(&child_status);
	if (WIFEXITED(child_status))
	    printf("Child %d terminated with exit status %d\n",
	           wpid, WEXITSTATUS(child_status));
	else
	    printf("Child %d terminated abnormally\n", wpid);
    }
*/
printf("Entered..:) \n ");
pid_t a;
a=execl("/bin/ls","ls -l",NULL);
printf("Returned....\n");
return 0;
}
