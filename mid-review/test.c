#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
int main (){
	printf("starting main\n");
	int file_fd = open ("test.txt", O_WRONLY | O_TRUNC | O_CREAT, 0666);
	int new_fd = dup (STDOUT_FILENO);
	dup2 (file_fd, STDOUT_FILENO);
	pid_t child_pid = fork();
	if (child_pid) {
		wait (NULL);
		printf ("in father\n");
	} else {
		dup2 (file_fd, STDOUT_FILENO);
		printf("in child\n");
	}
	printf("end:%d\n",child_pid);
	return 0;
}
