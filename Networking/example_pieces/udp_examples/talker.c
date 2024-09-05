#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <netdb.h>

#define PORT "4001"

int main(int argc, char **argv)
{
	struct addrinfo hints;
	struct addrinfo *res;
	struct addrinfo *p;
	int talkfd;
	int status;
	int numbytes;

	if(argc < 3){
		fprintf(stderr, "usage: ./talker <ip-of-listen> <message>\n");
		exit(1);
	}
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

	status = getaddrinfo(argv[1], PORT, &hints, &res);
	if(status < 0){
		fprintf(stderr, "getaddrinfo : %s", gai_strerror(status));
		exit(1);
	}
	for(p = res; p != NULL; p = p->ai_next)
	{
		talkfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if(talkfd == -1)
			continue;
		break;
	}
	if(p == NULL)
		fprintf(stderr, "failed to make create endpoint");
	
	numbytes = sendto(talkfd, argv[2], strlen(argv[2]), 0, p->ai_addr, p->ai_addrlen); 
	if(numbytes == -1){
		perror("sendto: ");
		exit(1);
	}
	
	freeaddrinfo(res);
	close(talkfd);
	exit(0);
}
