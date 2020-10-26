#include <stdio.h>
#include <iostream>
#include "EasyNetServiceClient.h"
#include "Log.h"
#if defined(WIN32)
#include <crtdbg.h>
#endif
using namespace std;

uint16_t g_uTcpPort = 4657;
uint16_t g_uUdpPort = 4657;

void LogErrorHook(std::string _szLog)
{
    throw _szLog;
}

void NewServiceMsg(std::string _szIP, std::string _szHostName, ServiceList* _pPortSrv,void * _pParam)
{
    _pParam = 0;
	if (_pPortSrv->empty() == false)
	{
		printf("%s :%s\r\n", _szHostName.c_str(), _szIP.c_str());
		printf("\t%s \t%s \t%s\r\n", "port", "type", "name");
		for (size_t index = 0; index < _pPortSrv->size(); index++)
		{
			printf("\t%d \t%s \t%s\r\n", (*_pPortSrv)[index]->uPort, (*_pPortSrv)[index]->szServiceType.c_str(), (*_pPortSrv)[index]->szServiceName.c_str());
		}
	}
}

void PrintUsage(void)
{
	cout << "Usage: EasyNetServiceClient [-t (tcpport)] [-u (udpport)]" << endl;
	cout << "such as : \"EasyNetServiceClient -t 3456 -u 5678\"" << endl;
	cout << "default tcpport is " << g_uTcpPort << endl;
	cout << "default udpport is " << g_uUdpPort << endl;
}

int AnalyzeParam(int argc, char* argv[])
{
	if (argc > 1)
	{
		for (int i = 0; i < argc; i++)
		{
			char* pArgv = argv[i];
			if (pArgv[0] == '-')
			{
				switch (pArgv[1])
				{
				case 't':
					g_uTcpPort = (uint16_t)atoi(argv[++i]);
					break;
				case 'u':
					g_uUdpPort = (uint16_t)atoi(argv[++i]);
					break;
				case 'h':
					PrintUsage();
					break;
				default:
					PrintUsage();
					return -1;
				}
			}
			else
			{
				PrintUsage();
				return -1;
			}
		}
	}
	return 0;
}

int main(int argc, char* argv[])
{
    Log::m_gErrHook = LogErrorHook;
	if (AnalyzeParam(argc, argv) == 0)
	{
		EasyNetServiceClient ensc(g_uUdpPort, g_uTcpPort, NewServiceMsg, nullptr);
		ensc.AllService();
		printf("Press ENTER To Exit\r\n");
		getchar();// DelayMs(1000);
		Log::Debug("Ending\r\n");
	}
#if defined(WIN32) && defined(DEBUG)
	_CrtDumpMemoryLeaks();
#endif
	return 0;
}

