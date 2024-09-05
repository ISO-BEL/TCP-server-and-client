//connectionless datagram server(udp)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>

#define MY_PORT "4001"
#define BACKLOG 10 

void (*Signal(int, void (*func)(int) ) )(int);
void sig_child(int signo);
void *get_inaddr(struct sockaddr *sa);

int main(int argc, char *argv)
{
	struct sockaddr_storage client_addr;
	struct addrinfo hints;
	struct addrinfo *res;
	struct addrinfo *p;
	int num_bytes;
	int client_addr_size = sizeof(client_addr);
	int listenfd;
	int connfd;
	int status;
	pid_t pid;
	char addr_string[INET6_ADDRSTRLEN];
	char listen_buffer[256];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET6;
	hints.ai_socktype = SOCK_DGRAM;

	status = getaddrinfo(INADDR_ANY, MY_PORT, &hints, &res);
	if(status < 0){
	}
	for(p = res; p != NULL; p=p->ai_next)
	{
	
		listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if(listenfd == -1){
			continue;
		}
		status = bind(listenfd, p->ai_addr, p->ai_addrlen);
		if(status == -1){
			continue;
		}
		break; //if both succeed break out
	}
	if(p == NULL)
		fprintf(stderr, "failed to bind socket\n");
	
	freeaddrinfo(res);
	//data gram listens via listen recvfrom
	num_bytes = recvfrom(listenfd, listen_buffer, (sizeof(listen_buffer) - 1), 
						0, (struct sockaddr*)&client_addr, &client_addr_size);
	if(num_bytes == -1){
		perror("recvfrom: ");
		exit(1);
	}
	
	//print address of sender
	printf("package recieved from: %s\n", inet_ntop(client_addr.ss_family,
			get_inaddr((struct sockaddr*)&client_addr),
			addr_string, sizeof(addr_string) ));

	//print out package size
	printf("package size in bytes : %d\n", num_bytes);

	//print out message
	printf("[ Message ]\n%s\n", listen_buffer);
	
	exit(0);
}

void *get_inaddr(struct sockaddr *sa)
{
	if(sa->sa_family == AF_INET){
		return &(((struct sockaddr_in*)&sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)&sa)->sin6_addr);	
}

