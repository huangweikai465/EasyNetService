#include "ClientSocketAction.h"
#include "iostream"
#include "math.h"

using namespace std;

void ClientSocketAction::ProcQueueData(ClientSocketAction* _pThis)
{
	RecvMsgInfo* pMsgInfo;
	Log::Debug("starting TCP Server queueProc\r\n");
	while (_pThis->m_isRunning)
	{
		while (_pThis->m_oRecvStrQue.empty() == false)
		{
			pMsgInfo = _pThis->m_oRecvStrQue.front();
			Log::Debug(" <-- %s:  %s\r\n",pMsgInfo->szIP.c_str(),pMsgInfo->szRecvStr.c_str());
			if (_pThis->m_pNewSrvRep != nullptr)
			{
				_pThis->m_pNewSrvRep(pMsgInfo->szIP, pMsgInfo->szRecvStr,_pThis->m_pParam);
			}
			_pThis->m_oRecvStrQue.pop();
			delete pMsgInfo;
		}
		DelayMs(10);	
	}
	Log::Debug("Exiting TCP Server queueProc\r\n");
	while (_pThis->m_oRecvStrQue.empty() == false)
	{
		delete _pThis->m_oRecvStrQue.front();
		_pThis->m_oRecvStrQue.pop();
	}
	Log::Debug("Exit TCP Server queueProc\r\n");
}

std::string ClientSocketAction::RecvTcpStr(SOCKET _oClient)
{
	std::string szRecvStr = "";
	char recvBuf[101] = {0};  //最大接收1K字节
	uint16_t uLen = 0;
	if (m_pTcpServer->Recv(_oClient,recvBuf,sizeof(uint16_t)) == 2)
	{
		uLen = recvBuf[0];
		uLen &= 0xff;
		uLen |= ((uint16_t)(recvBuf[1]<<8)) & 0xff00;
		while (uLen > 0)
		{
			int16_t recvCount = (int16_t)m_pTcpServer->Recv(_oClient, recvBuf, (sizeof(recvBuf)-1)<uLen?(sizeof(recvBuf)-1):uLen);
			if (recvCount < 0)
				break;
			szRecvStr += recvBuf;
			uLen = (uint16_t)(uLen - recvCount);
			if(uLen >0)
			{
				DelayMs(10);
				memset(recvBuf, 0, sizeof(recvBuf));
			}
		}
	}
	return szRecvStr;
}

void ClientSocketAction::GetNewTcpClient(SOCKET _newSocket, IPAddrRecorder _newClientAddr, void* _pParam)
{
	static std::mutex oRecvStrQueLock;
	ClientSocketAction* pThis = (ClientSocketAction*)_pParam;
	Log::Debug("Readding TCP Data From %s\r\n",_newClientAddr.ToString().c_str());
	std::string szRecvStr = pThis->RecvTcpStr(_newSocket);
	pThis->m_pTcpServer->Disconnect(_newSocket);
	if (szRecvStr.empty() == false)
	{
		RecvMsgInfo* newMsgInfo = new RecvMsgInfo;
		newMsgInfo->szIP = _newClientAddr.ToString();
		newMsgInfo->szRecvStr = szRecvStr;
		oRecvStrQueLock.lock();   //防止多线程push数据时造成冲突
		pThis->m_oRecvStrQue.push(newMsgInfo);
		oRecvStrQueLock.unlock();
	}
}

ClientSocketAction::ClientSocketAction(NewServiceReportCB _pNewSrvRep,void * _pParam)
{
	m_isRunning = true;
	m_pTcpServer = nullptr;
	m_pNewSrvRep = _pNewSrvRep;
	m_pParam = _pParam;
	m_oIpif.Init();
	m_pSocketService = SocketService::GetService();
	m_pProcStr = new thread(ProcQueueData, this);
}

ClientSocketAction::~ClientSocketAction()
{
	StopTcpServer();
	m_isRunning = false;
	m_pProcStr->join();
	delete m_pProcStr;
	SocketService::CleanSocketService();
}

IPDevice* ClientSocketAction::GetIPDevice(uint16_t index)
{
	return m_oIpif.GetDevice(index);
}

void  ClientSocketAction::BroadCastMsg(uint16_t _uPort,std::string _szMsg)   //朝各个IP的广播地址的_uPort端口广播自身IP地址
{
	for (int i = 0; i < m_oIpif.GetDeviceCount(); i++)
	{
		IPDevice* ipd = nullptr;
		ipd = m_oIpif.GetDevice((uint16_t)i);
		if ((ipd != nullptr) && ipd->GetBroadcastAddr().lAddr != 0)
		{
			IPAddrRecorder broadcastAddr = ipd->GetBroadcastAddr();
			m_pSocketService->SendUDPMsg(broadcastAddr.lAddr, _uPort, _szMsg);
			Log::Debug(" --> %s:  %s\r\n", broadcastAddr.ToString().c_str(), _szMsg.c_str());
		}
	}
}

void ClientSocketAction::StartTcpServer(uint16_t _uPort)
{
	StopTcpServer();
	m_pTcpServer = m_pSocketService->StartTCPServer(_uPort, GetNewTcpClient,this);
}

void ClientSocketAction::StopTcpServer(void)
{
	if (m_pTcpServer != nullptr)
	{
		m_pSocketService->StopTCPServer(m_pTcpServer);
		m_pTcpServer = nullptr;
	}
}




