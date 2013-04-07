#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main() {
char str[] ="%123";
char *pch;
pch = strtok(str,"%");
int num = atoi(pch);
printf("%s\n",pch);
printf("%d\n",num);
return 0;
}

