#ifndef TCPUTIL_H
#define TCPUTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>

class TCPUtil
{
public:
    TCPUtil();

public:
    static int serviceStart(char *serv_addr, int serv_port);
    static int clientConnect(char *serv_addr, int serv_port);
private:
    static int _serviceSetUp(const char* ip, int port);
};

#endif // TCPUTIL_H
