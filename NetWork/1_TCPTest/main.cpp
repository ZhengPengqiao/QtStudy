#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <TCPUtil.h>

using namespace std;

static int serv_port = 9999;
static char *serv_addr = "127.0.0.1";


int main(int argc, char* argv[])
{
    if( argc >= 2 )
    {
        TCPUtil::clientConnect(serv_addr, serv_port);
    }
    else
    {
        TCPUtil::serviceStart(NULL, serv_port);
    }
}
