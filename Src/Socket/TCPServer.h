#pragma once
#include "stdint.h"
#include <string>
#include "IpInfo.h"
#include <thread>
#include "SocketInclude.h"
#include <list>
#include <mutex>

//typedef void (*GetNewMsgFromTCPCB)(std::string _szIP,std::string _szMsg);
typedef void (*GetNewClientCB)(SOCKET _newSocket,IPAddrRecorder _newClientAddr,void * _pParam);

class TCPServer
{
private:
	volatile bool isRunning;
	std::thread * m_pListen;
	SOCKET m_oSocket;
	SOCKADDR_IN m_oSocketIn;
	GetNewClientCB pTcpNewClientCB;
	void* pParam;
	std::list<SOCKET> m_lClient;
	std::mutex m_oListLock;   //防止多线程同时调用

	static void ListeningNewClient(TCPServer* _pThis);
public:
	TCPServer(uint16_t _port,GetNewClientCB _pTcpMsgCB,void * _pParam);
	~TCPServer();
	
	int Start(void);
	void Stop(void);

	int Send(SOCKET _soClient, const char* _pData, uint16_t _uDataLen);
	int Recv(SOCKET _soClient, char* _pBuffer, uint16_t _uBufferLen);
	
	void Disconnect(SOCKET _soClient);

//	void SendStr(SOCKET _soClient,std::string _szStr);
//	std::string RecvStr(SOCKET _soClient);


};

