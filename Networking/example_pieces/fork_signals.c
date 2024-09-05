#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

void (*Signal(int, void (*func)(int) ) )(int);
void sig_child(int signo);
void child_function(void);

int main(void)
{
	//setup a signal to catch SIGCHLD and the function to handle it 
	Signal(SIGCHLD, sig_child); 

	pid_t id = fork();	
	if(id < 0) { 
		perror("fork fail"); 
		exit(1);
	}
	else if(id == 0){ //child
		child_function();
	}
	else if(id > 1) { //parent 
		parent_function();
	}

	while(1){}	
	exit(0);
}

void child_function(void)
{
	//closes listening socket	
	//closes listening socket	
	printf("child reached function");
	exit(0); //is made a zombie waiting for parent to clean up
}
void parent_function(void)
{
}

//sigaction wrap
void (*Signal(int signo, void (*func)(int) ) )(int)
{
	struct sigaction act;
	struct sigaction oact;
	
	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if(signo == SIGALRM)
	{
#ifdef  SA_INTERRUPT
		act.sa_flags |= SA_INTERRUPT; //sun os 4.x
#endif
	}
	else 
	{
#ifdef SA_RESTART
		act.sa_flags |= SA_RESTART; //svr4 , 4.4 BSD
#endif
	}

	if (sigaction(signo, &act, &oact) < 0)
		return (SIG_ERR);
	return oact.sa_handler;
}
//is given a signal number (single int parameter)
void sig_child(int signo)
{
	pid_t pid;
	int stat;

	int save_errno = errno; //errno is possibly overwritten by waitpid
	do
	{
		pid = waitpid(-1, &stat, WNOHANG);
		printf("child %d terminated", pid);
	}
	while(pid > 0);
	
	errno = save_errno;
	return; //could interupt
}

