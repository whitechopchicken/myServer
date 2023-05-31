#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<netinet/in.h>
#include<ctype.h>
#include<strings.h>
#include"wrap.h"

#define SERV_PORT 9526
#define MAX_LISTEN 100

int main(int argc , char* argv[]){
    char buf[BUFSIZ];
    int lfd ;  //服务器端的第一个 套接字fd
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));//将地址结构清零       #include<strings.h>

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);// INADDR_ANY 表示让系统自动分配一个. sin_addr后面还需要再转化成 .s_addr 
    lfd = socket(AF_INET, SOCK_STREAM, 0);
    Bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    int cfd;                                        //服务器端接受连接后产生的 第二个套接字fd
    Listen(lfd, MAX_LISTEN);                        //监听的是lfd，不是cfd

    struct sockaddr_in clit_addr;                   //客户端的 addr 不需要初始化，为什么？
    socklen_t clit_addr_len  = sizeof(clit_addr);
    cfd = Accept(lfd, (struct sockaddr *)&clit_addr, &clit_addr_len);     // 为什么传的是clie 的addr ？

    pid_t pid;                                      //进程id
    int ret;
    while(1){                                       //假设服务器一直运行
        cfd = Accept(lfd, (struct sockaddr *)&clit_addr, &clit_addr_len);       // 为什么传的是clie 的addr ？
        pid = fork();                                                           //创建子进程
        if(pid < 0){                                                            //创建进程失败
            perr_exit("fork error");
        }
        else if(pid == 0){                          //创建的是子进程
            close(lfd);
            break;                                  //写在外面，好看些
        }
        else{                                       //创建的是父进程
            close(cfd);                             //线程不能关闭，但是进程可以 
        }
    }
    if(pid == 0){
        ret = read(cfd, buf,sizeof(buf));           //读入客户端的内容  buf[]
        if(ret == 0){                               //read()返回值为0，说明检测到客户端关闭
            close(cfd);  
            exit(1);                                //“子进程可以用exit退出，因为是进程间通信”   
        }
        for(int i=0 ; i<ret;i++){
            buf[i] = toupper(buf[i]);
        }

        write(cfd, buf, ret);                       //不管是read、还是write ，主语都是server，  这里是把改变后的 buf[] 写回客户端 cfd
        write(STDOUT_FILENO, buf, ret);             // 只为了显示出来
        close(lfd); 
    }






    return 0 ;
}