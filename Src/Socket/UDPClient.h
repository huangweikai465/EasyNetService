#pragma once
#include <stdint.h>
#include <string>
#include "IpInfo.h"
#include "SocketInclude.h"

class UDPClient
{
private:
	static IPAddrRecorder oBroadcastAddr;
	SOCKET m_oSocket;
	SOCKADDR_IN m_oSocketIn;
	void SetUDPTypeToBroadcast(void);   //设置为广播类型  
public:
	UDPClient(uint32_t _addr, uint16_t _port);
	~UDPClient();

	int Send(const char* _pData, uint16_t _uDataLen);
	int Recv(char* _pBuffer, uint16_t _uBufferLen);
};


