#include "EasyNetServiceServer.h"
#include "../cJSON/cJSON.h"

void EasyNetServiceServer::ProcReq(IPAddrRecorder _oIP, uint16_t _uPort, std::string _szCMD, std::string _szParam)
{
	std::string szReturnMsg = GetReturnMsg(_szCMD, _szParam);
	m_pServSockAct->SendServiceReport(_oIP, _uPort, szReturnMsg);
}

std::string EasyNetServiceServer::GetReturnMsg(std::string _szCMD, std::string _szParam)
{
	try {
        //m_oSrvCfgFile.ReflashFileContent();
        //m_oNode.LoadFromStr(m_oSrvCfgFile.GetFileContent());  //更新文件

		std::string szServiceContent = "";
		if (_szCMD.compare("type") == 0)
		{
            szServiceContent = m_oNode.SearchType(_szParam).ToString();
		}
		else if (_szCMD.compare("name") == 0)
		{
            szServiceContent = m_oNode.SearchName(_szParam).ToString();
        }
        else if (_szCMD.compare("port") == 0)
        {
            Log::Debug("Searching Port %s\r\n",_szParam.c_str());
            szServiceContent = m_oNode.SearchPort(std::atoi(_szParam.c_str())).ToString();
        }
		else
		{
            szServiceContent = m_oNode.ToString();
		}
		cJSON* cReturnMsg;
		cReturnMsg = cJSON_CreateObject();
		cJSON_AddStringToObject(cReturnMsg, "HostName", m_pServSockAct->GetHostName().c_str());
		cJSON_AddItemToObject(cReturnMsg, "Service", cJSON_Parse(szServiceContent.c_str()));
		char* pMsg = cJSON_PrintUnformatted(cReturnMsg);
		szServiceContent = pMsg;
		free(pMsg);
		cJSON_Delete(cReturnMsg);
		return szServiceContent;
	}
	catch (...)
	{
		Log::Warnning("Config File Error ,Please Check %s \r\n", m_oSrvCfgFile.GetFileName().c_str());
		return "";
	}
}

void EasyNetServiceServer::NewUDPReport(IPAddrRecorder _oIP, std::string _szMsg, void* _pParam)
{
	EasyNetServiceServer* pThis = (EasyNetServiceServer*)_pParam;
	cJSON* cjson;
	cjson = cJSON_Parse(_szMsg.c_str());
	if (cjson != nullptr)
	{
		UdpRequest* newReq = new UdpRequest;
		try
		{
			newReq->oIP = _oIP;
			newReq->uPort = (uint16_t)cJSON_GetObjectItem(cjson, "TcpPort")->valuedouble;
			newReq->szCMD = cJSON_GetObjectItem(cjson, "CMD")->valuestring;
			newReq->szParam = "";
			if (cJSON_HasObjectItem(cjson, "Param"))
			{
				newReq->szParam = cJSON_GetObjectItem(cjson, "Param")->valuestring;
			}
			pThis->m_oReqList.push(newReq);
		}
		catch (...)
		{
			delete newReq;
		}
		cJSON_Delete(cjson);
	}
}

EasyNetServiceServer::EasyNetServiceServer(uint16_t _uUdpPort, std::string _szSrvCfgFileName)
    :m_oSrvCfgFile(_szSrvCfgFileName,EXAMPLECONTENT)
{
    Log::Debug("Starting\r\n");
	if (m_oSrvCfgFile.IsJsonArray() == true)
	{
        m_oNode.LoadFromStr(m_oSrvCfgFile.GetFileContent());  //更新文件
        m_pServSockAct = new ServerSocketAction(NewUDPReport, this);
		m_pServSockAct->StartUDPSetver(_uUdpPort);
		m_bIsRunning = true;
		Log::Normal("Start RasyNetServiceServer at UDPPort:%d\r\n",_uUdpPort);
		m_pRequestProc = new std::thread([](EasyNetServiceServer* _pThis)
			{
				UdpRequest* pReq = nullptr;
				while (_pThis->m_bIsRunning)
				{
					while (_pThis->m_oReqList.empty() == false)
					{
						pReq = _pThis->m_oReqList.front();
						if (pReq != nullptr)
						{
							//添加命令处理程序
							_pThis->ProcReq(pReq->oIP, pReq->uPort, pReq->szCMD, pReq->szParam);
							delete pReq;
						}
						_pThis->m_oReqList.pop();
					}
					DelayMs(10);  //防止CPU占用过高
				}
				while (_pThis->m_oReqList.empty() == false)
				{
					delete _pThis->m_oReqList.front();
					_pThis->m_oReqList.pop();
				}
				Log::Debug("Exit RequestProc thread\r\n");
			}, this);
	}
	else
    {
        Log::Error("ConfigFile %s Error\r\n",0,m_oSrvCfgFile.GetFileName().c_str());
        exit(1);
    }
}

EasyNetServiceServer::~EasyNetServiceServer()
{
	m_bIsRunning = false;
	m_pServSockAct->StopUDPSetver();
	m_pRequestProc->join();
	delete m_pRequestProc;
	delete m_pServSockAct;
}
