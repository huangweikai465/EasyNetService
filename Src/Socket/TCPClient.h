#pragma once
#include "stdint.h"
#include <string>
#include "IpInfo.h"
#include <thread>
#include "SocketInclude.h"

class TCPClient
{
private:
	SOCKET m_oSocket;
	SOCKADDR_IN m_oSocketIn;
public:
	TCPClient(uint32_t _uServerAddr, uint16_t _uPort);
	~TCPClient();
	
	int Connect(void);
	int Send(const char * _pData, uint16_t _uDataLen);
	int Recv(char* _pData, uint16_t _uDataLen);
	void Disconnect(void);
};

