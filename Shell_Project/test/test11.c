#include<stdlib.h>
#include<stdio.h>
#include<signal.h>
#include<unistd.h>
void killhandler(int sig){
printf("May your soul rest in peace....\n");
return;
}

int main(){

int i=0;
signal(SIGKILL,killhandler); 
if(fork()==0){
 printf("i am chlid..\n");
 while(1);
 }
 else{
 printf("I am parent..\n");
 while(1);
 exit(0);
 }
return;
}

