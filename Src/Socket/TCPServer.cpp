#include "TCPServer.h"

//这里不能使用std命名空间，否则bind会跟mac下的另外一个bind冲突，导致编译报错
//using namespace std;

void TCPServer::ListeningNewClient(TCPServer* _pThis)
{
	SOCKADDR_IN newClient;
	socklen_t sLen = sizeof(SOCKADDR);
	while (_pThis->isRunning)
	{
		SOCKET sockConn = accept(_pThis->m_oSocket, (SOCKADDR*)&newClient, &sLen);  
		if((int)sockConn < 0)
		{
			//try {
			//	Log::Warnning("Accept failed: %d\n", GetErrNo);
			//}
			//catch (...) {}
			continue;
		}
        IPAddrRecorder newIP((uint32_t)newClient.sin_addr.s_addr);
        Log::Debug("New TCP Client %s\r\n",newIP.ToString().c_str());
		_pThis->m_oListLock.lock();
		_pThis->m_lClient.push_back(sockConn);
		_pThis->m_oListLock.unlock();
		if (_pThis->pTcpNewClientCB != nullptr)
		{
			//异步调用
			std::thread([](TCPServer* _pThis,SOCKET _soClient,IPAddrRecorder _oClientIP) {
				_pThis->pTcpNewClientCB(_soClient,_oClientIP,_pThis->pParam);
				}, _pThis,sockConn,newIP).detach();
		}
		else
		{
            Log::Debug("Get Connect From : %s\r\n",newIP.ToString().c_str());
			_pThis->Disconnect(sockConn);
		}
		Log::Debug("Proc TCP End\r\n");
	}
	Log::Debug("End TCP Recv thread\r\n");
}
TCPServer::TCPServer(uint16_t _port,GetNewClientCB _pTcpNewClientCB,void * _pParam)
{
	m_oSocket = 0;
	m_pListen = nullptr;
	m_oSocketIn.sin_family = AF_INET;
	m_oSocketIn.sin_port = htons(_port);
	m_oSocketIn.sin_addr.s_addr = htonl(INADDR_ANY);//_addr;
    isRunning = false;
	pTcpNewClientCB = _pTcpNewClientCB;
	pParam = _pParam;
}

TCPServer::~TCPServer()
{
	if (m_oListLock.try_lock())
	{
		Log::Debug("Lock state is Unlocked\r\n");
		m_oListLock.unlock();
	}
	else
	{
		Log::Debug("Lock state is Locked\r\n");
	}
}

int TCPServer::Start(void)
{
	std::string ErrLog = "";
	int err = 0;
	m_oSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if ((int)m_oSocket < 0)
	{
		Log::Error("Socket TCP Server Init Error", GetErrNo);
		return -1;
	}
    err = bind(m_oSocket, (const SOCKADDR*)&m_oSocketIn, sizeof(SOCKADDR));  
    if(err < 0)
	{
		Log::Error("Socket TCP Server Bind Error", GetErrNo);
		return -1;
    } 
    err = listen(m_oSocket, 10);  
    if(err  < 0)
	{
		Log::Error("Socket TCP Server Listen Error", GetErrNo);
    }
	isRunning = true;
	Log::Debug("TCP Server is Running\r\n");
	m_pListen = new std::thread(ListeningNewClient, this);
	return 0;
}

void TCPServer::Stop()
{
	isRunning = false;
	//关闭所有连接
	m_oListLock.lock();
	for (SOCKET so = 0; m_lClient.empty() == false; )
	{
		so = m_lClient.front();
		CloseSocket(so);
		m_lClient.pop_front();
	}
	m_oListLock.unlock();
	CloseSocket(m_oSocket);
	Log::Debug("Waitting For TCP Server Recv thread exit\r\n");
    if(m_pListen != nullptr)
    {
        m_pListen->join();
        delete m_pListen;
    }
}

int TCPServer::Send(SOCKET _soClient, const char* _pData, uint16_t _uDataLen)
{
	return (int)send(_soClient, _pData, _uDataLen, 0);
}

int TCPServer::Recv(SOCKET _soClient, char* _pBuffer, uint16_t _uBufferLen)
{
	return (int)recv(_soClient, _pBuffer, _uBufferLen, 0);
}

void TCPServer::Disconnect(SOCKET _soClient)
{
	m_oListLock.lock();
	if (m_lClient.empty() == false)
	{
		for (std::list<SOCKET>::iterator itr = m_lClient.begin(); itr != m_lClient.end(); itr++)
		{
			if (*itr == _soClient)
			{
				CloseSocket(_soClient);
				Log::Debug("Pop client %d\r\n", _soClient);
				m_lClient.erase(itr);
				break;
			}
		}
	}
	m_oListLock.unlock();
}



