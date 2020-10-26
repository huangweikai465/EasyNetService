#include "SocketService.h"
//#if defined(WIN32)
//#pragma comment(lib,"Iphlpapi.lib") 
//#pragma comment(lib,"ws2_32.lib")
//#endif

SocketService* SocketService::g_pSrv = nullptr;

SocketService::SocketService()
{
#if defined(WIN32)
	WORD wVersionRequested;
	WSADATA wsaData;
	// 启动socket api
	wVersionRequested = MAKEWORD( 2, 2 );
	int err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 )
	{
		throw "Error at WSAStartup( wVersionRequested, &wsaData )";
	}
#endif
}

SocketService::~SocketService()
{
#if defined(WIN32)
	WSACleanup( );
#endif
}

SocketService* SocketService::GetService(void)
{
	if (g_pSrv == nullptr)
	{
		g_pSrv = new SocketService();
	}
	return g_pSrv;
}

void SocketService::CleanSocketService(void)
{
	if (g_pSrv != nullptr)
	{
		delete g_pSrv;
		g_pSrv = nullptr;
	}
}

std::string SocketService::GetHostName(void)
{
	char buf[256] = { 0 };
	gethostname(buf,256);
	return buf;
}


TCPServer* SocketService::StartTCPServer(uint16_t _port, GetNewClientCB _pTcpNewClientCB,void* _pParam)
{
	TCPServer * pTcpServer = new TCPServer(_port, _pTcpNewClientCB,_pParam);
	pTcpServer->Start();
	return pTcpServer;
}

int SocketService::StopTCPServer(TCPServer* _pTcpServer)
{
	try {
		if (_pTcpServer != nullptr)
		{
			Log::Debug("Stopping TcpServer\r\n");
			_pTcpServer->Stop();
			Log::Debug("Stoped TcpServer\r\n");
			delete _pTcpServer;
			Log::Debug("Deleted TcpServer\r\n");
			return 0;
		}
		return -1;
	}
	catch (...)
	{
		return -2;
	}
}

TCPClient* SocketService::GetTCPClient(uint32_t _uIP, uint16_t _uPort)
{
	Log::Normal("Get TCP Client %s:%d\r\n",IPAddrRecorder(_uIP).ToString().c_str(),_uPort);
	return new TCPClient(_uIP,_uPort);
}

TCPClient* SocketService::GetTCPClient(std::string _szIP, uint16_t _uPort)
{
	IPAddrRecorder oIP(_szIP);
	return new TCPClient(oIP.lAddr, _uPort);
}

void SocketService::DeleteTcpClient(TCPClient* _pTcpClient)
{
	if (_pTcpClient != nullptr)
	{
		delete _pTcpClient;
	}
}

UDPServer* SocketService::StartUDPServer(uint16_t _port, GetNewMsgFromUDPCB _pUdpMsgCB,void* _pParam)
{
	UDPServer* pUdpServer = new UDPServer(_port, _pUdpMsgCB,_pParam);
	pUdpServer->Start();
	return pUdpServer;
}

int SocketService::StopUDPServer(UDPServer* _pUdpServer)
{
	try {
		if (_pUdpServer != nullptr)
		{
			_pUdpServer->Stop();
			delete _pUdpServer;
			_pUdpServer = nullptr;
			return 0;
		}
		return -1;
	}
	catch (...)
	{
		return -2;
	}
}

int SocketService::SendUDPMsg(uint32_t _uIP, uint16_t _uPort, std::string _szMsg)
{
	UDPClient* udpc = new UDPClient(_uIP, _uPort);
	int ret = udpc->Send(_szMsg.c_str(),(uint16_t)_szMsg.length());
	delete udpc;
	return ret;
}

int SocketService::SendUDPMsg(std::string _szIP, uint16_t _uPort, std::string _szMsg)
{
	IPAddrRecorder oIP(_szIP);
	return SendUDPMsg(oIP.lAddr, _uPort, _szMsg);
}

UDPClient* SocketService::GetUDPClient(uint32_t _uIP, uint16_t _uPort)
{
	return new UDPClient(_uIP, _uPort);
}

UDPClient* SocketService::GetUDPClient(std::string _szIP, uint16_t _uPort)
{
	IPAddrRecorder oIP(_szIP);
	return new UDPClient(oIP.lAddr, _uPort);
}

void SocketService::DeleteUDPClient(UDPClient* _pUdpClient)
{
	if (_pUdpClient != nullptr)
	{
		delete _pUdpClient;
	}
}

