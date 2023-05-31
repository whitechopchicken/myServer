#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<pthread.h>
#include<ctype.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h> 
#include"wrap.h"

#define SERV_PORT 9528 //自定义的端口号

void sys_err(const char *str)
{
	perror(str);
	exit(1);
}

int main(int argc, char *argv[])
{
	int lfd = 0 ; //建立连接的fd
	int cfd = 0 ; //connect fd
	char ret; //read() 需要记录读的字节数
	char buf[BUFSIZ];//BUFSIZ == 8192
	char client_IP[BUFSIZ];
	int i  = 10 ;//

	struct sockaddr_in serv_addr, clit_addr;  //bind()  & accept() 需要传递的参数.下列是对其初始化
	socklen_t clit_addr_len;//accept() 需要传递的参数
	
	serv_addr.sin_family = AF_INET;//
	serv_addr.sin_port = htons(SERV_PORT);	//传一个端口号,由host 传递到 network ，并且是short
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);	//							
									
	//服务器 第一件事：创建socket
	lfd = socket(AF_INET, SOCK_STREAM, 0 ); //domain , type, protocal
	if(lfd==-1){//调用API一定要检查返回值
		//sys_er("socket error");
		printf("socket error\n");
		return 0;
	}

	//bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	Bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) ;
		
	listen(lfd, 128/*最大上限*/);

	clit_addr_len = sizeof(clit_addr);
	cfd = accept(lfd, (struct sockaddr *)&clit_addr, &clit_addr_len);   //调用完accept() 会传出一个 clit_addr ,以及其大小。 它返回一个新的fd

	//(struct sockaddr *)&clit_addr 客户端与服务器建立连接，它会返回客户端ip和port ， 也就是serv_addr.sin_addr.s_addr
	printf("client ip:%s port:%d\n",
			inet_ntop(AF_INET, &clit_addr.sin_addr.s_addr,client_IP, sizeof(client_IP)), //客户端 ip地址， 需要转换
			ntohs(clit_addr.sin_port));//端口号，需要转换

	if(cfd == -1){
		//sys_er("accept error"); 
		printf("accept error\n");
		return 0 ;
	}
	
	while(i--){  //这个循环的目的主要是为了测试，显示输出。
	//接下来 ，服务器就该 read()
		ret = read(cfd, buf, sizeof(buf));//读取来自客户端的输入==buf数组
		//小写转大写，改变buf
		write(STDOUT_FILENO,buf, ret); //把客户端的内容写到 STDOUT中
		for(int j = 0 ; j < ret ; j++){
			buf[j] = toupper(buf[j]); // #include<ctype.h>  小写转大写，测试服务器反映
		}	
		write(cfd, buf, ret);	//写回客户端
	}

	close(lfd);	//关闭连接
	close(cfd);

	return 0;
}