#include "TCPUtil.h"
#include <iostream>

using namespace std;

TCPUtil::TCPUtil()
{

}


/**
 * @brief thread_run
 * 每一个用户链接，都会创建一个对应的线程进行客户端请求处理
 * @param arg：客户端的文件描述符
 * @return
 */
void* thread_run(void* arg)
{
    struct sockaddr stAddr = {0};
    struct sockaddr_in stAddrIn = {0};
    long lNameLen = sizeof(struct sockaddr);
    int new_fd = *(int *)arg;
    char buf[1024];
    string tag = "No";

    // 获取客户端的IP和端口，并且构造tag打印用的字符串
    if(0 == getsockname(new_fd, &stAddr, (socklen_t *)&lNameLen))
    {
        memcpy(&stAddrIn, &stAddr, lNameLen);
        tag.append("server>");
        tag.append("[");
        tag.append(inet_ntoa(stAddrIn.sin_addr));
        tag.append(":");
        tag.append(to_string(ntohs(stAddrIn.sin_port)));
        tag.append("]");

    }

    pthread_detach(pthread_self());
    memset(buf, 0, sizeof(buf));

    while (1)
    {
        ssize_t s = read(new_fd, buf, sizeof(buf)-1);
        if (s > 0)
        {
            buf[s] = '\0';
            cout << tag << buf << endl;
            write(new_fd, buf, strlen(buf));
        }
        else
        {
            cout << tag << "client quits..." << endl;
            break;
        }
    }
}


int TCPUtil::_serviceSetUp(const char* ip, int port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        cout << "socket failure" << endl;
        return -1;
    }

    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(port);
    if( ip == NULL )
    {
        local.sin_addr.s_addr=htonl(INADDR_ANY);
    }
    else
    {
        inet_aton(ip, &local.sin_addr);
    }

    int opt = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        cout << "setsockopt failure" << endl;
        return -1;
    }

    int flag = bind(sock, (struct sockaddr*)&local, sizeof(local));
    if (flag == -1)
    {
        cout << "bind failure" << endl;
        return -1;
    }
    flag = listen(sock, 6);
    if (flag == -1)
    {
        cout << "listen failure" << endl;
        return -1;
    }
    return sock;
}


int TCPUtil::serviceStart(char *serv_addr, int serv_port)
{
    int listen_sock = TCPUtil::_serviceSetUp(serv_addr, serv_port);
    if( listen_sock < 0 )
    {
        cout << "tcp service start error" << endl;
        return 0;
    }

    while (1)
    {
        struct sockaddr_in client;
        socklen_t len = sizeof(client);
        int new_fd = accept(listen_sock, (struct sockaddr*)&client, &len);
        if (new_fd == -1)
        {
            cout << "accept failure" << endl;
            continue;
        }

        pthread_t tid;
        pthread_create(&tid, NULL, thread_run, &new_fd);
    }
    return 0;
}

int TCPUtil::clientConnect(char *serv_addr, int serv_port)
{

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        fprintf(stderr, "socket failure\n");
        exit(-1);
    }

    struct sockaddr_in remote;
    remote.sin_family = AF_INET;
    remote.sin_port = htons(serv_port);
    inet_aton(serv_addr, &remote.sin_addr);

    int flag = connect(sock, (struct sockaddr*)&remote, sizeof(remote));
    if (flag == -1)
    {
        fprintf(stderr, "connect failure\n");
        exit(-1);
    }

    while (1)
    {
        char buf[1024];
        printf("please enter # ");
        fflush(stdout);
        ssize_t s = read(0, buf, sizeof(buf)-1);
        if (s > 0)
        {
            buf[s-1] = '\0';
            write(sock, buf, strlen(buf));

            s = read(sock, buf, strlen(buf));
            if (s > 0)
            {
                printf("server echo # %s\n", buf);
            }
        }
    }

    return 0;
}


