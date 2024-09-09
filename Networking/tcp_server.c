#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#define BACKLOG 10

//wraps
void sendall(int sockfd, const void *buffer, size_t len, int flags)
{
	int status;	
sendall_again:
	while(status > 0)
		status = send(sockfd, buffer, len, flags);
	if(status < 0){
		if(errno == EINTR)
			goto sendall_again;
		perror("send: ");
		exit(1);
	}
}

void recvall(int sockfd, void *buffer, size_t len, int flags)
{
	int status;	
recvall_again:
	while(status > 0)
		status = recv(sockfd, buffer, len, flags);
	if(status < 0){
		if(errno == EINTR)
			goto recvall_again;
		perror("recv: ");
		exit(1);
	}
}

void Close(int sockfd)
{
	int status;
	//use fsync if not special fd, ie socketfd
CLOSE:
	status = close(sockfd); 
	if(status == -1)
	{
		if(errno == EINTR)
		{
#ifdef HP_UX
			goto CLOSE;
#endif
		}
		perror("close :");
		exit(1);
	}
}

//wrapper for sigaction //providing hints and returning result
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

int main(void)
{
	int connfd;
	int sockfd;
	int status;
	pid_t  pid;
	struct addrinfo hints;
	struct addrinfo *res;
	struct addrinfo *p;
	struct sockaddr_storage client_addr;
	socklen_t client_addr_size = sizeof(client_addr);
	char package[256];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; //use my ip

label:
	status = getaddrinfo(INADDR_ANY, "8001", &hints, &res);
	if(status < 0){
		if(errno == EINTR){	
			goto label;  //Signal maybe can set this to auto restart system calls 
						 //when interupted
		}	
		fprintf(stderr, "getaddrinfo: %s", gai_strerror(status)); //errno message
		exit(1);	
	}
	//search linked list for first structure that works
	for(p = res; p != NULL; p = p->ai_next)
	{
	SOCKET:
		sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if(sockfd == -1){
			if(errno == EINTR){
				goto SOCKET;
			}
			perror("socket: ");
			continue;
		}

	BIND:
		status = bind(sockfd, p->ai_addr, p->ai_addrlen);
		if(status == -1){
			if(errno == EINTR){
				goto BIND;
			}
			perror("bind: ");
			close(sockfd);
			continue;
		}
		//leave the loop
		break; 
	}
	//check if nothing bit (ie reached NULL without a bound socket)
	if(p == NULL){
		perror("failed to find socket");
		exit(1);
	}
	
	//dont need anymore
	freeaddrinfo(res);	
	
listen:
	status = listen(sockfd, BACKLOG);
	if(status == -1){
		if(errno == EINTR)
			goto listen;
		
		perror("listen :");
		exit(1);
	}
	Signal(SIGCHLD, sig_child);

	while(1)
	{
		connfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_size);
		if(connfd < 0){
			if(errno == EINTR)
				continue;
			else if(errno == ECONNABORTED) //interupted by client sending RST
				continue;
			else
				perror("accept :");
				exit(1);
		}
		fork();
		if(pid < 0)
		{
			perror("fork :");
			exit(1);
		}
		if(pid == 0)
		{
			//the child closes this socket because it has its own address space
			//and counter, for this socket. it will remain open until
			//all processes that own it close
			Close(sockfd); 

			recvall(connfd, package, sizeof package, 0);
			sendall(connfd, package, sizeof package, 0); //echo back
			exit(0); //closes connfd for child process (decrements the counter)
		}
		else
		{
		//parent closes connfd socket
		Close(connfd);
		}
	}
	exit(0);
}


//todo
//
//
