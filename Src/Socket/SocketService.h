#pragma once
#include "UDPClient.h"
#include "UDPServer.h"
#include "TCPClient.h"
#include "TCPServer.h"

class SocketService
{
private:
	static SocketService* g_pSrv;
	SocketService();
	~SocketService(); 
public:
	static SocketService * GetService(void);
	static void CleanSocketService(void);
	//获取本机名称
	static std::string GetHostName(void);
	//TCP服务端
	TCPServer* StartTCPServer(uint16_t _port, GetNewClientCB _pTcpNewClientCB, void* _pParam);
	int StopTCPServer(TCPServer* _pTcpServer);
	//TCP客户端
	//立即发送TCP消息
	TCPClient* GetTCPClient(uint32_t _uIP,uint16_t _uPort);
	TCPClient* GetTCPClient(std::string _szIP,uint16_t _uPort);
	void DeleteTcpClient(TCPClient * _pTcpClient);
	//UDP服务端
	UDPServer* StartUDPServer(uint16_t _port, GetNewMsgFromUDPCB _pUdpMsgCB, void* _pParam);
	int StopUDPServer(UDPServer* _pUdpServer);
	//UDP客户端
	//立即发送UDP消息
	int SendUDPMsg(uint32_t _uIP, uint16_t _uPort, std::string _szMsg);
	int SendUDPMsg(std::string _szIP, uint16_t _uPort, std::string _szMsg);
	UDPClient* GetUDPClient(uint32_t _uIP, uint16_t _uPort);
	UDPClient* GetUDPClient(std::string _szIP, uint16_t _uPort);
	void DeleteUDPClient(UDPClient* _pUdpClient);
};

