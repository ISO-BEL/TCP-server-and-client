#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

void sendall(int sockfd, const void *buffer, size_t len, int flags);
void recvall(int sockfd, void *buffer, size_t len, int flags);

int main(int argc, char **argv)
{
	
	int status;
	int sockfd;
	struct addrinfo hints;
	struct addrinfo *res;
	char messagebuffer[256] = "hello from the underground\n";
	

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM; 
	if(argc == 2)
	{
		status = getaddrinfo(argv[1], "8001", &hints, &res);
		if(status < 0)
		{
			fprintf(stderr, "getaddrinfo : %s", gai_strerror(status));
			exit(1);
		}
	}
	else
	{
		status = getaddrinfo(INADDR_ANY, "8001", &hints, &res);
		if(status < 0)
		{
			fprintf(stderr, "getaddrinfo : %s", gai_strerror(status));
			exit(1);
		}
	}
	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if(sockfd == -1){
		perror("socket: ");
		exit(1);
	}
	//can not be restarted when interupted EINTR
	//must call something called select
	status = connect(sockfd, res->ai_addr, res->ai_addrlen);
	if(status == -1){
		if(errno == EISCONN)
			goto already_connected;
			
		perror("connect: ");
		exit(1);
	}
already_connected:
	
	while(status > 0)
		status = send(sockfd, messagebuffer, sizeof(messagebuffer), 0);
	if(status < 0){
		perror("send: ");
		exit(1);
	}

	recv(sockfd, messagebuffer, sizeof(messagebuffer), 0);
	
	FILE *fp = fopen("client.dat", "w+");
	fprintf(fp, "[the message]\n %s",  messagebuffer);

	freeaddrinfo(res);	
	exit(0);

}

void sendall(int sockfd, const void *buffer, size_t len, int flags)
{
	int status;	
sendall_again:
	while(status > 0)
		status = send(sockfd, buffer, len, flags);
	if(status < 0){
		perror("recv: ");
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
		perror("recv: ");
		exit(1);
	}
}

