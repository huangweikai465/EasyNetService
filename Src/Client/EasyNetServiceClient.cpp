#include "EasyNetServiceClient.h"


void EasyNetServiceClient::InitSrvListFromJSON(ServiceList* _pPortSrv, JsonOpt & _oJson)
{
    if(_oJson.IsNull())
        return;
    if(_oJson.IsArray())
    {
        uint16_t uArrayCount = (uint16_t)_oJson.ArraySize();
        for (int index = 0; index < uArrayCount; index++)
        {
            JsonOpt oArrayItem = _oJson[index];
            if(oArrayItem.IsNull() == false)
            {
                PortService* pPortSrv = new PortService;
                try
                {
                    pPortSrv->uPort = (uint16_t)oArrayItem["Port"].ToInt();
                    pPortSrv->szServiceType = oArrayItem["Type"].ToString();
                    pPortSrv->szServiceName = oArrayItem["Name"].ToString();
                    _pPortSrv->push_back(pPortSrv);
                }
                catch (...)
                {
                    delete pPortSrv;
                    continue;
                }
            }
        }
    }
	else
	{
		throw "Param _oJson is Not a Json Array";
	}
}

void EasyNetServiceClient::CleanSrvList(ServiceList* _pPortSrv)
{
	while (_pPortSrv->empty() == false)
	{
		delete _pPortSrv->back();
		_pPortSrv->pop_back();
	}
}

void EasyNetServiceClient::SendUDPCmd(std::string _szCMD,std::string _szParam)
{
    JsonOpt oJo;
    oJo.AddItem("CMD",_szCMD);
    oJo.AddItem("TcpPort",m_uTcpPort);
	if (_szParam.empty() == false)
	{
        oJo.AddItem("Param",_szParam);
	}
    std::string szJson = oJo.ToString();
    Log::Debug("Send : %s\r\n", szJson.c_str());
	m_oNetService.BroadCastMsg(m_uUdpPort, szJson);
}

void EasyNetServiceClient::GetServiceReport(std::string _szIP, std::string _szMsg, void* _pParam)
{
    EasyNetServiceClient* pThis = (EasyNetServiceClient*)_pParam;
    std::vector<PortService*> oSrvQue;
    JsonOpt oJo;
    oJo.LoadFromString(_szMsg);
    JsonOpt oSrv = oJo["Service"];
    if(oSrv.IsNull() == false)
    {
        std::string szHostName = oJo["HostName"].ToString();
        pThis->InitSrvListFromJSON(&oSrvQue, oSrv);
        if (pThis->m_pProcSrvMsg != nullptr)
        {
            pThis->m_pProcSrvMsg(_szIP,szHostName, &oSrvQue,pThis->m_pParam);
        }
    }
	pThis->CleanSrvList(&oSrvQue);
}


EasyNetServiceClient::EasyNetServiceClient(uint16_t _uUdpPort, uint16_t _uTcpPort, ProcServiceMsg _pProcSrvMsg, void* _pParam)
	:m_oNetService(GetServiceReport,this)
{
	m_pProcSrvMsg = _pProcSrvMsg;
	m_pParam = _pParam;
	m_uUdpPort = _uUdpPort;
	m_uTcpPort = _uTcpPort;
	m_oNetService.StartTcpServer(_uTcpPort);
	Log::Normal("Start RasyNetServiceClient at UDPPort:%d,TCPPort:%d\r\n",_uUdpPort,_uTcpPort);
}

EasyNetServiceClient::~EasyNetServiceClient()
{
	m_oNetService.StopTcpServer();
}

void EasyNetServiceClient::AllService(void)
{
	SendUDPCmd("all");
}

void EasyNetServiceClient::SearchServicePort(uint16_t _uPort)
{
    SendUDPCmd("port",std::to_string(_uPort));
}

void EasyNetServiceClient::SearchServiceType(std::string _szType)
{
    SendUDPCmd("type",_szType);
}

void EasyNetServiceClient::SearchServiceName(std::string _szName)
{
	SendUDPCmd("name", _szName);
}

void EasyNetServiceClient::SearchIP(IPAddrRecorder _szIP)
{
	SendUDPCmd("ip", _szIP.ToString());
}

void EasyNetServiceClient::ModifyTCPPort(uint16_t _uPort)
{
	m_uTcpPort = _uPort;
	m_oNetService.StopTcpServer();
	m_oNetService.StartTcpServer(_uPort);
}

void EasyNetServiceClient::ModifyUDPPort(uint16_t _uPort)
{
	m_uUdpPort = _uPort;
}

