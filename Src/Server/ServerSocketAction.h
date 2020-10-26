#pragma once
#include "../Socket/SocketService.h"
#include "../Socket/IpInfo.h"
#include <string>
#include <queue>

typedef void (*NewUDPReportCB)(IPAddrRecorder _oIP, std::string _szMsg, void* _pParam);

class ServerSocketAction
{
private:
	NewUDPReportCB m_pUdpRep;
	SocketService * m_pSocketService;
	void* m_pParam;
	UDPServer* m_pUdpServer;
protected:
	static void GetNewUDPMsg(SOCKADDR_IN _newClient, const char* _pData, uint16_t _uDataLen, void* _pParam);
public:
	ServerSocketAction(NewUDPReportCB _pUdpRep,void * _pParam);
	~ServerSocketAction();

	std::string GetHostName();
	void StartUDPSetver(uint16_t _uPort);
	void StopUDPSetver();
	void SendServiceReport(IPAddrRecorder _oIP, uint16_t _uPort, std::string _szMsg);
};

