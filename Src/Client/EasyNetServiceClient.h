#pragma once
#include "ClientSocketAction.h"
#include "../cJSON/JsonOpt.h"
#include <vector>

#define GETALL "all"
#define SEARCHIP "ip"


typedef struct {
	uint16_t uPort;
	std::string szServiceType;
	std::string szServiceName;
} PortService;  //描述某个端口提供的服务

typedef std::vector<PortService*> ServiceList;
typedef void (*ProcServiceMsg)(std::string _szIP,std::string _szHostName, ServiceList* _pPortSrv,void* _pParam);

class EasyNetServiceClient
{
private:
	ClientSocketAction m_oNetService;
	ProcServiceMsg m_pProcSrvMsg;
	void* m_pParam;

	uint16_t m_uTcpPort;
    uint16_t m_uUdpPort;

    void InitSrvListFromJSON(ServiceList* _pPortSrv, JsonOpt & _oJson);
    void CleanSrvList(ServiceList* _pPortSrv);

	void SendUDPCmd(std::string _szCMD, std::string _szParam = "");

protected:
    static void GetServiceReport(std::string _sIP,std::string _szMsg,void* _pParam);  //新服务报文

public:
	EasyNetServiceClient(uint16_t _uUdpPort,uint16_t _uTcpPort,ProcServiceMsg _pProcSrvMsg,void * _pParam);
	~EasyNetServiceClient();

	void AllService(void);

    void SearchServicePort(uint16_t _uPort);
    void SearchServiceType(std::string _szType);
    void SearchServiceName(std::string _szName);
	void SearchIP(IPAddrRecorder _szIP);

	void ModifyTCPPort(uint16_t _uPort);
	void ModifyUDPPort(uint16_t _uPort);

	//void SetMask(std::string _szName);

};

