#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
int val = 0;
void apple (void){pid_t oval = getppid();kill (oval,SIGKILL);}
void orange(void){val = 1;}

int main (void){pid_t pid = fork();signal(SIGKILL,orange);if (pid == 0){apple();}else {waitpid(pid);} printf("val=%d\n",val);}