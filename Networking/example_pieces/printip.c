#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> //inet_ntop and pton -- INET6_ADDRSTRLEN
#include <netdb.h>

int main(int argc, char **argv)
{
	struct addrinfo hints;
	struct addrinfo	*result;
	struct addrinfo *p;
	int	status = 0;
	char ipstr[INET6_ADDRSTRLEN]; 

	if(argc != 2)
	{
		fprintf(stderr, "no argument passed");
		exit(1);
	}
//thread safety and loop safety maybe (memset)
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	status = getaddrinfo(argv[1], NULL, &hints, &result);
	if(status != 0)
	{
		fprintf(stderr, "getaddinfo : %s\n", gai_strerror(status));
		exit(2);
	}

	//remap out this logic --then play with tabular stuff, spacing (make my own memset)

	for(p = result; p != NULL; p = p->ai_next)
	{
		void *address;
		char *ipversion;

		if(p->ai_family == AF_INET)
		{
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr; 
			address = &(ipv4->sin_addr);
			ipversion = "IPv4";
		}
		else
		{
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
			address = &(ipv6->sin6_addr);
			ipversion = "IPv6";
		}
	//network to presentation then print
	inet_ntop(p->ai_family, address, ipstr, sizeof ipstr);
	printf("  %s: %s\n", ipversion, ipstr);
	}	

	freeaddrinfo(result);

	exit(0);
}
