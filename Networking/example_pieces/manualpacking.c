#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	struct sockaddr_in my_addr;
	//ipv4 manual packing then cast to sockaddr
	//
	
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
	{
		fprintf(strerr, "socket returned -1, failed to create");
		exit(1);
	}
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = hton( --a port number );
	my_addr.sin_addr.s_addr = inet_addr("10.12.110.57"); //inet.pton now?
	memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

	//cast and bind
	bind(sockfd, (struct sockaddr *)&my_addr, sizeof my_addr);
	
	exit(0);
}
