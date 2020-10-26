#pragma once
#include "../Socket/SocketService.h"
#include "../Socket/IpInfo.h"
#include <string>
#include <queue>

typedef void (*NewServiceReportCB)(std::string _sIP,std::string _szMsg,void* _pParam);  //新服务报文

class ClientSocketAction
{
private:
	SocketService * m_pSocketService;
	IPInfo m_oIpif;
	TCPServer* m_pTcpServer;

	NewServiceReportCB m_pNewSrvRep;
	void* m_pParam;

	typedef struct
	{
		std::string szIP;
		std::string szRecvStr;
	}RecvMsgInfo;
	volatile bool m_isRunning;
	std::queue<RecvMsgInfo*> m_oRecvStrQue;
	std::thread* m_pProcStr;

	static void ProcQueueData(ClientSocketAction* _pThis);
	std::string RecvTcpStr(SOCKET _oClient);
protected:
	static void GetNewTcpClient(SOCKET _newSocket,IPAddrRecorder _newClientAddr,void * _pParam);					  //当获取到新的服务时调用，在该函数中调用回调函数
public:
	ClientSocketAction(NewServiceReportCB _pNewSrvRep = nullptr,void * _pParam = nullptr);
	~ClientSocketAction();

	IPDevice* GetIPDevice(uint16_t index);

	void BroadCastMsg(uint16_t _uPort,std::string _sMsg);   //朝各个IP的广播地址的_uPort端口广播自身IP地址
	void StartTcpServer(uint16_t _uPort);	   //建立TCP服务器
	void StopTcpServer(void);
};

