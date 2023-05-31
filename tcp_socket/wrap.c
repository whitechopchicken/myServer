#include"wrap.h"
#include<errno.h>

void perr_exit(const char *s){
	perror(s);
	exit(1);
}

int Bind(int fd,const  struct sockaddr *sa, socklen_t salen){
	int n; 
	if((n = bind(fd, sa, salen)) < 0 )
		perr_exit("bind error");
	return n ;
}

int Listen(int fd, int backlog){
	int n ;
	if((n = listen(fd, backlog)) < 0)
		perr_exit("listen error");
	return n ;
}