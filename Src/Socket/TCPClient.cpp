#include "TCPClient.h"

using namespace std;

TCPClient::TCPClient(uint32_t _uServerAddr, uint16_t _uPort)
{
	m_oSocketIn.sin_family = AF_INET;
	m_oSocketIn.sin_port = htons(_uPort);
	m_oSocketIn.sin_addr.s_addr = _uServerAddr;
	m_oSocket = 0;
}


TCPClient::~TCPClient()
{
	if (m_oSocket != 0)
	{
		Disconnect();
	}
}

int TCPClient::Connect(void)
{
	m_oSocket = socket(AF_INET, SOCK_STREAM, 0);
	if ((int)m_oSocket < 0)
	{
		Log::Error("Socket TCP Client Init Error", GetErrNo);
		return -1;
	}

	int err = connect(m_oSocket, (SOCKADDR*)&m_oSocketIn, sizeof(SOCKADDR));
	if (err < 0)
	{
		Log::Error("Socket TCP Client Connecrt Error", GetErrNo);
		return -1;
	}
	return 0;
}

int TCPClient::Send(const char* _pData, uint16_t _uDataLen)
{
	return (int)send(m_oSocket, (const char*)_pData, _uDataLen,0);
}

int TCPClient::Recv(char* _pData, uint16_t _uDataLen)
{
	return (int)recv(m_oSocket, _pData, _uDataLen,0);
}

void TCPClient::Disconnect(void)
{
	CloseSocket(m_oSocket);
	m_oSocket = 0;
}
