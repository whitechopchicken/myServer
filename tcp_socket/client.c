#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<pthread.h>
#define SERV_PORT 9528


void sys_err(const char *str)
{
	perror(str);
	exit(1);
}

int main(int argc, char *argv[])
{
	int cfd;
	int count ;
	char buf[BUFSIZ];

	printf("请设置发送次数 count = :\n");
	scanf("%d", &count);

	struct sockaddr_in serv_addr;  //服务器地址结构

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, "172.23.122.71", &serv_addr.sin_addr.s_addr);
  
	cfd = socket(AF_INET, SOCK_STREAM , 0);	//客户端的socket
	if(cfd == -1 )
		sys_err("socket error");

	int ret = connect(cfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr));  // 1. 
	if(ret != 0)
		sys_err("connect error");
	while(count--){
		write(cfd, "hello" , 5);  //相当于客户端写count 次hello
		ret = read(cfd, buf, sizeof(buf));	//
		write(STDOUT_FILENO, buf, ret);	// 把大写的写到屏幕上。ret是read() 的返回值
	}
	close(cfd);  //结束客户端socket
		     //
	return 0 ;
}
