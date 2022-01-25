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
static int cs_select = 0;
static int cs_test = 0;


void showHelp()
{
	printf("./programe options \n");	
	printf("USED:");
	printf("	-help: show help info\n");
	printf("	-ip str: service ip(default %s)\n", serv_addr);
	printf("	-cs val: 0:Client 1:Server(default %d)\n", cs_select);
	printf("	-t: client cycle test (default %d)\n", cs_test);
}

int checkParam(int argc,char **argv)
{
	int i = 0;
	for(i = 1;i < argc;i++)
	{
		if( strcmp("-help", argv[i]) == 0 )
		{
			showHelp();
			return -1;
		}
		else if( strcmp("-ip", argv[i]) == 0 )
		{
            serv_addr = argv[i+1];
			i++;
		}
		else if( strcmp("-cs", argv[i]) == 0)
		{
			cs_select = atoi(argv[i+1]);
			i++;
		}
		else if( strcmp("-t", argv[i]) ==0 )
		{
			cs_test = 1;
		}
		else
		{
			printf("param %s is not support \n\n", argv[i]);
			showHelp();
			return -1;
		}
	}
	return 0;
}

int main(int argc, char* argv[])
{
    char ch;

    checkParam(argc,argv);

    if( cs_select == 0 )
    {
        if( cs_test == 1 )
        {
            TCPUtil::clientTCPServiceTest(serv_addr, serv_port);
        }
        else
        {
            TCPUtil::clientConnect(serv_addr, serv_port);
        }
        
    }
    else
    {
        TCPUtil::serviceStart(NULL, serv_port);
    }

    return 0;
}
