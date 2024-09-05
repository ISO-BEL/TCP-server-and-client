


struct sockaddr_storage their_addr;
char size[INET6_ADDRSTRLEN];


//do this after accept call (their_addr is filled by the accept call)
inet_ntop(address_family, src, &size, sizeof(size) );
//
//
address_family -> their_addr.ss_family
src			   -> (struct sockaddr*)&their_addr
//
//

