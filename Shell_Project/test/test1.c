#include<stdlib.h>
#include <stdio.h>
int count =0;
int main(){
count++;
if(fork()==0){
printf("count = %d",count);
exit(0);
}
else{
int status;
wait(&status);
printf("\ndone\n");
}
return 0;
}

