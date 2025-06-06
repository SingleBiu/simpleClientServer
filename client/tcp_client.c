/*
 * @Author: SingleBiu
 * @Date: 2021-09-13 10:53:50
 * @LastEditors: SingleBiu
 * @LastEditTime: 2024-12-16 20:45:50
 * @Description: file content
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define NET_CFG_FILE "net.conf"
#define LOOP 0

int tcp_send(const char *serv_ip,short serv_port,char *buf);


int tcp_send(const char *serv_ip,short serv_port,char *buf)
{
    int n = 0;

    if (sizeof(buf) > 127)
    {
        return 0;
    }

    char sendbuf[128];
    char recvbuf[128];

    int sock;
    int ret;
// retry:
    sock = socket(AF_INET,SOCK_STREAM,0);
    if (sock == -1)
    {
        perror("socket errno");
        return -1;
    }

    struct sockaddr_in sa;
    memset(&sa,0,sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(serv_port);
    sa.sin_addr.s_addr = inet_addr(serv_ip);

    ret = connect(sock,(struct sockaddr*) &sa,sizeof(sa));
    if (ret == -1)
    {
        perror("connect error");
        close(sock);
        //每隔X秒尝试重连
        // sleep(3);
        // goto retry;
        return -1;
    }

    #if LOOP
    while (1)
    {
    #endif
        // 清空
        memset(sendbuf,0,sizeof(sendbuf));
        memset(recvbuf,0,sizeof(recvbuf));

        strncpy(sendbuf,buf,sizeof(buf));

        // 发送
        send(sock,sendbuf,sizeof(sendbuf),0);

        // 接收
        n = recv(sock,recvbuf,sizeof(recvbuf),0);
        if (n == 0)
        {
            printf("Server terminated prematually\n");
            #if LOOP
            break;
            #endif
        }
        else if (n < -1)
        {
            
        }
        
        printf("received: %s\n",recvbuf);
    #if LOOP
        //间隔2秒发送一次
        sleep(2);
    }
    #endif
    return -1;
}


int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        return 0;
    }
    char buf[128] = {0};
    strncpy(buf,argv[1],sizeof(argv[1]));
    //从配置文件中读取服务器ip和端口号
    char servIp[15];
    char servPort[5];
    FILE* cfgFp = fopen(NET_CFG_FILE,"r");
    fscanf(cfgFp,"%s",servIp);
    fscanf(cfgFp,"%s",servPort);

    tcp_send(servIp,atoi(servPort),buf);

    // while (1)
    // {
    //     if (tcp_send(servIp,atoi(servPort),buf))
    //     {
    //         /* code */
    //     }
    //     else
    //     {
    //         // error
    //     }

    // }

    printf("system exit\n");

    return 0;
}