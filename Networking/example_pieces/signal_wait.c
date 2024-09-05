#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

//stop / conn
//getpid
//getppid
void handle_sigchld(int signo)
{
	int stat;
	printf("bye child\n");	
	waitpid(-1, &stat, WNOHANG);
	return;
}

int main(void)
{
	struct sigaction act, oact;
	act.sa_handler = &handle_sigchld;
	sigaction(SIGCHLD, &act, &oact);
	

	pid_t pid = fork();
	if(pid < 0)
	{
		exit(1);
	}
	else if(pid == 0)
	{
		exit(0);
	}
	else
	{
		while(1)
		{
			usleep(9000);
		}
	}


	return 0;
}
