#include"wrap.h"
#include<errno.h>

void perr_exit(const char *s){
	perror(s);
	exit(1);
}


int Bind(int fd,const  struct sockaddr *sa, socklen_t salen){		//bind与accept 参数有区别，前者的socklen 是int，而后者是 int *
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

// int Accept(int fd, struct sockaddr *sa, socklen_t *addrlen){
// 	int n ;
// 	if(n = accept(fd, sa, addrlen))
// 		perr_exit("accept no error");
// 	return n ;
// }

int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
	int n;

again:
	if ((n = accept(fd, sa, salenptr)) < 0) {
		if ((errno == ECONNABORTED) || (errno == EINTR))
			goto again;
		else
			perr_exit("accept error");
	}
	return n;
}