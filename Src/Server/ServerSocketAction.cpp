#include "ServerSocketAction.h"
#include "../Log/Log.h"


void ServerSocketAction::GetNewUDPMsg(SOCKADDR_IN _newClient, const char* _pData, uint16_t _uDataLen, void* _pParam)
{
	IPAddrRecorder oFromIP(_newClient.sin_addr.s_addr);
	std::string szMsg = _pData;
	if (szMsg.length() == _uDataLen)
	{
		Log::Normal(" <-- %s: %s\r\n",oFromIP.ToString().c_str(),szMsg.c_str());
		ServerSocketAction* pThis = (ServerSocketAction*)_pParam;
		if (pThis->m_pUdpRep != nullptr)
		{
			pThis->m_pUdpRep(oFromIP, szMsg, pThis->m_pParam);
		}
	}
}

ServerSocketAction::ServerSocketAction(NewUDPReportCB _pUdpRep, void* _pParam)
{
	m_pUdpServer = nullptr;
	m_pSocketService = SocketService::GetService();
	m_pParam = _pParam;
	m_pUdpRep = _pUdpRep;
}

ServerSocketAction::~ServerSocketAction()
{
	SocketService::CleanSocketService();
}

std::string ServerSocketAction::GetHostName()
{
	return m_pSocketService->GetHostName();
}

void ServerSocketAction::StartUDPSetver(uint16_t _uPort)
{
	StopUDPSetver();
	m_pUdpServer = m_pSocketService->StartUDPServer(_uPort, GetNewUDPMsg, this);
}

void ServerSocketAction::StopUDPSetver()
{
	if (m_pUdpServer != nullptr)
	{
		m_pSocketService->StopUDPServer(m_pUdpServer);
		m_pUdpServer = nullptr;
	}
}

void ServerSocketAction::SendServiceReport(IPAddrRecorder _oIP, uint16_t _uPort, std::string _szMsg)
{
	uint16_t uMsgLen = (uint16_t)_szMsg.length();
	TCPClient oTcpClient(_oIP.lAddr, _uPort);
	if (_szMsg.empty())
	{
		Log::Debug("Return To  %s:%d   None\r\n", _oIP.ToString().c_str(), _uPort);
	}
	try {
		int err = oTcpClient.Connect();
		if (err == 0)
		{
			uint8_t pLen[2] = { 0 };
			pLen[0] = (uint8_t)(uMsgLen & 0xff);
			pLen[1] = 0;//(uint8_t)((uMsgLen >> 8) & 0xff);
			oTcpClient.Send((char*)pLen, sizeof(pLen));
			oTcpClient.Send(_szMsg.c_str(),(uint16_t)_szMsg.length());
			Log::Debug(" --> %s:%d: %02x %02s  %s\r\n", _oIP.ToString().c_str(), _uPort, pLen[0],pLen[1], _szMsg.c_str());
		}
		else
		{
			Log::Error("Connect to %s:%d Failed %d\r\n",GetErrNo, _oIP.ToString().c_str(), _uPort, err);
		}
		oTcpClient.Disconnect();
	}
	catch (...) {
	}
}
