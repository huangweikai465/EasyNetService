#pragma once
#include "ServerSocketAction.h"
#include "FileOpt.h"
#include "ServiceNode.h"
#include <queue>
#include <thread>

#define DEFAULTCFGFILE "EasyNetService.cfg"
#define EXAMPLECONTENT "[{\"Port\":22,\"Type\":\"ssh\",\"Name\":\"ExampleSSH\"},{\"Port\":21,\"Type\":\"ftp\",\"Name\":\"ExampleFTP\"}]"

typedef struct {
	IPAddrRecorder oIP;
	uint16_t uPort;
	std::string szCMD;
	std::string szParam;
} UdpRequest;

class EasyNetServiceServer
{
private:
	ServerSocketAction* m_pServSockAct;
	FileOpt m_oSrvCfgFile;
	ServiceNode m_oNode;
	std::queue<UdpRequest*> m_oReqList;
	std::thread* m_pRequestProc;
	volatile bool m_bIsRunning;

	void ProcReq(IPAddrRecorder _oIP, uint16_t _uPort, std:: string _szCMD, std::string _szParam);

	std::string GetReturnMsg(std::string _szCMD, std::string _szParam);
protected:
	static void NewUDPReport(IPAddrRecorder _oIP, std::string _szMsg, void* _pParam);
public:
    EasyNetServiceServer(uint16_t _uUdpPort = 3465,std::string _szSrvCfgFileName = "");
	~EasyNetServiceServer();

};

