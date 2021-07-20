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
    string tag;

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
            cout << TCPUtil::getTime() << ": " << tag << buf << endl;
            write(new_fd, buf, strlen(buf));
        }
        else
        {
            cout << TCPUtil::getTime() << ": " << tag << "client quits..." << endl;
            break;
        }
    }
}


int TCPUtil::_serviceSetUp(const char* ip, int port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        cout << TCPUtil::getTime() << ": " << "service socket failure" << endl;
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
        cout << TCPUtil::getTime() << ": " << "service setsockopt failure" << endl;
        return -1;
    }
    else
    {
        cout << TCPUtil::getTime() << ": " << "service socket setsockopt success" << endl;
    }
    

    int flag = bind(sock, (struct sockaddr*)&local, sizeof(local));
    if (flag == -1)
    {
        cout << TCPUtil::getTime() << ": " << "service bind failure" << endl;
        return -1;
    }
    else
    {
        cout << TCPUtil::getTime() << ": " << "service socket bind success" << endl;
    }
    
    flag = listen(sock, 6);
    if (flag == -1)
    {
        cout << TCPUtil::getTime() << ": " << "service listen failure" << endl;
        return -1;
    }
    else
    {
        cout << TCPUtil::getTime() << ": " << "service socket listen success" << endl;
    }
    return sock;
}


int TCPUtil::serviceStart(char *serv_addr, int serv_port)
{
    int listen_sock = TCPUtil::_serviceSetUp(serv_addr, serv_port);
    if( listen_sock < 0 )
    {
        cout << TCPUtil::getTime() << ": " << "service tcp service start error" << endl;
        return 0;
    }

    while (1)
    {
        struct sockaddr_in client;
        socklen_t len = sizeof(client);
        int new_fd = accept(listen_sock, (struct sockaddr*)&client, &len);
        if (new_fd == -1)
        {
            cout << TCPUtil::getTime() << ": " << "service accept failure" << endl;
            continue;
        }
        else
        {
            cout << TCPUtil::getTime() << ": " << "service accept success" << endl;
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
        cout << TCPUtil::getTime() << ": " << "client socket failure" << endl;
        exit(-1);
    }
    else
    {
        cout << TCPUtil::getTime() << ": " << "client socket success" << endl;
    }
    

    struct sockaddr_in remote;
    remote.sin_family = AF_INET;
    remote.sin_port = htons(serv_port);
    inet_aton(serv_addr, &remote.sin_addr);

    int flag = connect(sock, (struct sockaddr*)&remote, sizeof(remote));
    if (flag == -1)
    {
        cout << TCPUtil::getTime() << ": " << "client connect failure" << endl;
        exit(-1);
    }
    else
    {
        cout << TCPUtil::getTime() << ": " << "client connect success" << endl;
    }

    while (1)
    {
        char buf[1024];
        cout << "please enter #";
        fflush(stdout);
        ssize_t s = read(0, buf, sizeof(buf)-1);
        if (s > 0)
        {
            buf[s-1] = '\0';
            write(sock, buf, strlen(buf));

            s = read(sock, buf, strlen(buf));
            if (s > 0)
            {
                cout << TCPUtil::getTime() << ": " << "client recv:" << buf << endl;
            }
        }
    }

    return 0;
}

int TCPUtil::clientTCPServiceTest(char *serv_addr, int serv_port)
{

    int data = 0;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        cout << TCPUtil::getTime() << ": " << "client socket failure" << endl;
        exit(-1);
    }
    else
    {
        cout << TCPUtil::getTime() << ": " << "client socket success" << endl;
    }
    

    struct sockaddr_in remote;
    remote.sin_family = AF_INET;
    remote.sin_port = htons(serv_port);
    inet_aton(serv_addr, &remote.sin_addr);

    int flag = connect(sock, (struct sockaddr*)&remote, sizeof(remote));
    if (flag == -1)
    {
        cout << TCPUtil::getTime() << ": " << "client connect failure" << endl;
        exit(-1);
    }
    else
    {
        cout << TCPUtil::getTime() << ": " << "client connect success" << endl;
    }

    while (1)
    {
        data++;
        char buf[1024];
        for (int i = 0; i < 100; i++)
        {
            buf[i] = data%26+'a';
        }
        buf[100] = '\n';
        write(sock, buf, strlen(buf));

        int s = read(sock, buf, strlen(buf));
        if (s > 0)
        {
            cout << TCPUtil::getTime() << ": " << "client recv[" << data << "]:" << buf << endl;
        }

        sleep(1);
    }

    return 0;
}


string TCPUtil::getTime()
{
    time_t timep;
    struct tm *p;
    time (&timep);
    string str;
    p=gmtime(&timep);
    // p->tm_sec 获取当前秒
    // p->tm_min 获取当前分
    // 8+p->tm_hour 获取当前时,这里获取西方的时间,刚好相差八个小时
    // p->tm_mday 获取当前月份日数,范围是1-31
    // 1+p->tm_mon 获取当前月份,范围是0-11,所以要加1
    // 1900+p->tm_year 获取当前年份,从1900开始，所以要加1900
    // p->tm_yday 从今年1月1日算起至今的天数，范围为0-365
    str.append( "["+to_string(1900+p->tm_year) +"-"+ to_string(1+p->tm_mon) +"-"+ to_string(p->tm_yday) );
    str.append("_");
    str.append( to_string(8+p->tm_hour) +":"+ to_string(p->tm_min) +":"+ to_string(p->tm_sec) + "]");
    
    return str;
}
