#include<stdlib.h>
#include <stdio.h>
int main(){

if(fork()==0){
printf("\n");
system("ls");
exit(0);
}
else{
int status;
wait(&status);
printf("\ndone\n");
}
return 0;
}

