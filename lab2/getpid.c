#include<stdio.h>
#include<unistd.h>
int main(){
    
    printf("%d\n",STDIN_FILENO);
    printf("%d %d\n",getpid(),getppid());
    return 0 ;
}