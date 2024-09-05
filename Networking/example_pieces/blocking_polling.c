#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h> //changing system call states (blocking / non blocking)
#include <poll.h> //is slow (could use (libevent)

//basic example
int basic_example(void)
{
	int sockfd;
	int connfd;
	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	fcntl(sockfd, F_SETFL, O_NONBLOCK); //set socket to be non blocking


	//when calling a normally blocking system call
	//it will just pass it (polling it and returning error -1, and errno 
	//EWOULDBLOCK or EAGAIN
again:
	connfd = accept();
	if(connfd == -1){
		switch(errno)
		{
			case EINTR: goto again;
						break;
			case EAGAIN:
			case EWOULDBLOCK: //handle both
							  
		}
	}
	

	return 0;
}

//example using <poll.h>
//use libevent if you need something faster

//	struct pollfd
//	{
//		int		fd;			//file desc
//		short	events;		//bitmap events we wont to know about
//		short	revents;	//bitmap events that have happened
//	}
#define SOCK_AMOUNT 20
int main(void)
{
	int sockfd = socket(PF_INET, SOCK_STREAM, 0);
	struct pollfd fds[SOCK_AMOUNT];
	fds[0].events |= POLLIN;
	fds[0].fd = sockfd;
	fds[1].events |= POLLIN;
	fds[1].fd = 0; //stdout
	//POLLIN alert when ready to receieve 
	//POLLOUT alert when ready to send data

	int timeout = 2000; //miliseconds
	int num_events; 
	int pollin0_happend;
	num_events = poll(fds, SOCK_AMOUNT, timeout);   //returns number of non 0 events
	if(num_events == 0){
		puts("nothings happening here\n");
	} 
	else{ //something happened is it the event we are looking for (POLLIN)
		pollin1_happend = fds[1].revent & POLLIN;
	}

	if(pollin1_happpend){
		fprintf(stdout, "fd ready to recieve\n");
	}
	else{
		fprintf(stdout, "nothing to see here (timeout)\n");
	}

	

	return 0;
}

//exmaple scanning bitmap
