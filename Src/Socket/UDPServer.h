#pragma once
#include <stdint.h>
#include <string>
#include "IpInfo.h"
#include <thread>
#include "SocketInclude.h"

#define UDPMAXRECVLEN 256

typedef void (*GetNewMsgFromUDPCB)(SOCKADDR_IN _newClient, const char* _pData, uint16_t _uDataLen, void* _pParam);

class UDPServer
{
private:
	std::thread * m_pRecv;
	SOCKET m_oSocket;
	SOCKADDR_IN m_oSocketIn;
	GetNewMsgFromUDPCB pUdpMsgCB;
	void* pParam;
	volatile bool isRunning;

	static void RecvThread(UDPServer * _pThis);
public:
	UDPServer(uint16_t _uPort, GetNewMsgFromUDPCB _pUdpMsgCB, void* _pParam);
	~UDPServer();
	int Start(void);
	void Stop(void);

	int Send(SOCKADDR_IN* _pClient, const char * _pData,uint16_t _uDataLen);

};

