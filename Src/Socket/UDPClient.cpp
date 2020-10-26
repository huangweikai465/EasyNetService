#include "UDPClient.h"

#if defined(WIN32)
#elif defined(LINUX) || defined (MACOS)
#endif

using namespace std;
IPAddrRecorder UDPClient::oBroadcastAddr("0.0.0.255");

UDPClient::UDPClient(uint32_t _addr, uint16_t _port)
{
	m_oSocketIn.sin_family = AF_INET;
	m_oSocketIn.sin_port = htons(_port);
	m_oSocketIn.sin_addr.s_addr = _addr;
	m_oSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if ((int)m_oSocket < 0)
	{
		Log::Error("Socket UDP Client Init Error", GetErrNo);
	}
	if ((_addr & oBroadcastAddr.lAddr) == oBroadcastAddr.lAddr)
	{
		Log::Debug("%s is a Broadcast Addrr\r\n",IPAddrRecorder(_addr).ToString().c_str());
		SetUDPTypeToBroadcast();
	}
}

UDPClient::~UDPClient()
{
	CloseSocket(m_oSocket);
}

void UDPClient::SetUDPTypeToBroadcast(void)
{
#if defined(WIN32)
	char iOptval = 1;
#elif defined(LINUX) || defined (MACOS)
    int iOptval = 1;
#endif
	if (setsockopt(m_oSocket, SOL_SOCKET, SO_BROADCAST /*| SO_REUSEADDR*/, &iOptval , sizeof(iOptval)) < 0)
	{
		try {
			Log::Error("UDP Set Broadcast Error", GetErrNo);
		}
		catch (...) {}
	}
}

int UDPClient::Send(const char* _pData, uint16_t _uDataLen)
{
	int ret = (int)sendto(m_oSocket, _pData, _uDataLen, 0, (SOCKADDR*)&m_oSocketIn, sizeof(SOCKADDR));
	if (ret < 0)
	{
		Log::Error("UDP Client Send Error", GetErrNo);
		return -1;
	}
	return ret;
}

int UDPClient::Recv(char* _pBuffer, uint16_t _uBufferLen)
{
	socklen_t sLen = sizeof(SOCKADDR);
	int ret = (int)recvfrom(m_oSocket, _pBuffer, _uBufferLen, 0, (SOCKADDR*)&m_oSocketIn, &sLen);
	if (ret < 0)
	{
		Log::Error("UDP Client Recv Error", GetErrNo);
		return -1;
	}
	return ret;
}


