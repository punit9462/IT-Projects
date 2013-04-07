#include<stdio.h>
#include<stdlib.h>
int main(){
/*printf("proccess id: %d\n ",getppid());
if(fork()==0)
{
printf("\n");

//sleep(10);
printf("child terminated %d \n",getppid());
exit(0);
}
else{
wait(NULL);
printf("parent terminated\n");
}*/
/*printf("L0\n");
    fork();
    printf("L1\n");    
    fork();
    printf("L2\n");    
    fork();
    printf("Bye\n");
*/
printf("L0\n");
    if (fork() == 0) {
	printf("L1\n");    
	if (fork() == 0) {
	    printf("L2\n");
	    fork();
	}
    }
    printf("Bye\n");
return 0;
}

