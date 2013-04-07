#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
void handler(int sig){
printf("Handler returned..\n");
//printf("value");
exit(0);
}

int main(){
int i;
sighandler_t myhandler;
myhandler=signal(SIGINT,handler);
while(1){
//1fork();
sleep(1);
printf("hello\n");
}
return 0;
}

