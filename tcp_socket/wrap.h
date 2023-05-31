#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>


int Bind(int fd, const struct sockaddr *sa, socklen_t  salen);

int Listen(int fd, int backlog);
