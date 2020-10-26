#include "UDPServer.h"

void UDPServer::RecvThread(UDPServer* _pThis)
{
	char recvBuffer[UDPMAXRECVLEN ] = { 0 };
	SOCKADDR_IN newClient;
	socklen_t len = sizeof(SOCKADDR);
	while (_pThis->isRunning)
	{
		memset(recvBuffer, 0, sizeof(recvBuffer));
		int strLen = (int)recvfrom(_pThis->m_oSocket, recvBuffer, sizeof(recvBuffer), 0, (SOCKADDR*)&newClient, &len);
		if (strLen >= 0)
		{
			if (_pThis->pUdpMsgCB != nullptr)
			{
				_pThis->pUdpMsgCB(newClient, recvBuffer, (uint16_t)strLen, _pThis->pParam);
			}
			else
			{
                IPAddrRecorder newIP((uint32_t)newClient.sin_addr.s_addr);
                Log::Debug("Recv \" %s \" From %s\r\n", recvBuffer, newIP.ToString().c_str());
			}
		}
		//else
		//{
		//	Log::Warnning("UDP Server Recv Error\r\n");
		//}
	}
	Log::Debug("UDP Server Recv Stoped\r\n");
}

UDPServer::UDPServer(uint16_t _uPort, GetNewMsgFromUDPCB _pUdpMsgCB,void* _pParam)
{
	m_oSocket = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	m_oSocketIn.sin_family=AF_INET;
	m_oSocketIn.sin_port=htons(_uPort);
	m_oSocketIn.sin_addr.s_addr=htonl(INADDR_ANY);
	pUdpMsgCB = _pUdpMsgCB;
	isRunning = false;
	m_pRecv = nullptr;
	pParam = _pParam;
}


UDPServer::~UDPServer()
{
}

int UDPServer::Start(void)
{
	std::string ErrLog = "";
	m_oSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if ((int)m_oSocket < 0)
	{
		Log::Error("Socket UDP Server Init Error", GetErrNo);
		return -1;
	}
    int err = bind(m_oSocket, (SOCKADDR*)&m_oSocketIn, sizeof(SOCKADDR));  
    if(err < 0)
	{
		Log::Error("Socket UDP Server Bind Error", GetErrNo);
		return -1;
    }
	isRunning = true;
	m_pRecv = new std::thread(RecvThread, this);
	return 0;
}

void UDPServer::Stop(void)
{
	isRunning = false;
	Log::Debug("Release UDPServer\r\n");
	CloseSocket(m_oSocket);
    if(m_pRecv != nullptr)
    {
        m_pRecv->join();
        delete m_pRecv;
    }
}

int UDPServer::Send(SOCKADDR_IN* _pClient, const char* _pData, uint16_t _uDataLen)
{
	return (int)sendto(m_oSocket, _pData, _uDataLen, 0, (SOCKADDR*)_pClient, sizeof(SOCKADDR));
}


