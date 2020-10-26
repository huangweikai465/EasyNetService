#ifndef _SOCKETINCLUDE_H_
#define _SOCKETINCLUDE_H_
#include "../Log/Log.h"

#if defined(WIN32)
#include <WinSock2.h>
#include <ws2tcpip.h> 
#include <windows.h>
#define DelayMs(x) Sleep(x)
#define CloseSocket(_oSocket) \
	shutdown(_oSocket, SD_BOTH); \
	closesocket(_oSocket); 
#define GetErrNo WSAGetLastError()

#elif defined(LINUX) || defined (MACOS)
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
typedef struct sockaddr_in SOCKADDR_IN;
typedef int SOCKET;
typedef struct sockaddr SOCKADDR;
#define DelayMs(x) x<1000?usleep(x):sleep(x/1000)
#define GetErrNo errno
#define CloseSocket(_oSocket) \
	shutdown(_oSocket, 0x02); \
	close(_oSocket);
#endif


#endif
