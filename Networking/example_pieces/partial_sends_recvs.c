
char buffer[256];
int status;

//server sends response in a btye stream, sometimes all in one delivery 
//sometimes and many packets so i need to wrap my recvs in a loop
//in order to get the whole message, we can wait for a connection close (return 0)
while(status > 0)
	status = recv(sockfd, &buffer, sizeof buffer, 0);
if(status < 0){
	perror("recv: ");
	exit(1);
}

while(status > 0)
	status = send(sockfd, &buffer, sizeof buffer, 0);
if(status < 0){
	perror("recv: ");
	exit(1);
	}

 

