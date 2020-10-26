#include <iostream>
#include "FileOpt.h"
#include "ServiceNode.h"
#include "EasyNetServiceServer.h"
#include "cJSON.h"

FileOpt _oFo(DEFAULTCFGFILE,EXAMPLECONTENT);
ServiceNode oSrvNode;

void PrintUasge(void)
{
    printf("Usage:  ensmanager [cmd] (...)\r\n");
    printf("cmd:\r\n");
    printf("\tls:list current service config\r\n");
    printf("\trm [-p port] or [-n name]:remove service accordding to port or name\r\n");
    printf("\trn oldname newname:rename service\r\n");
    printf("\tadd -p port -t type -n name:add service\r\n");
}

void Init(void)
{
    std::string szContent = _oFo.GetFileContent();
    oSrvNode.LoadFromStr(szContent);
}

int PrintError(int _sError)
{
    if (_sError == 0)
    {
        return 0;
    }
	switch (_sError)
	{
	case -1:
		printf("Node Nod found\r\n");
		break;
	case -2:
		printf("Service Node Error,\r\n");
		break;
	case -3:
		printf("Service Node Error: Config file is not JsonArray\r\n");
		break;
	}
    return -1;
}

void ls(void)
{
    printf("%s\r\n", oSrvNode.Print().c_str());
}

void rm(int argc, char* argv[])
{
    if (argc == 4)
    {
        try {
            char* pParam = argv[2];
            if (pParam[0] == '-')
            {
                if (pParam[1] == 'p')
                {
                    uint16_t uPort = (uint16_t)atoi(argv[3]);
                    int ret = oSrvNode.RemoveNode(uPort);
                    if (PrintError(ret) == 0)
                    {
                        _oFo.ReWrite(oSrvNode.ToString());
                        ls();
                    }
                    return;
                }
                else if (pParam[1] == 'n')
                {
                    int ret = oSrvNode.RemoveNode(argv[3]);
                    if (PrintError(ret) == 0)
                    {
                        _oFo.ReWrite(oSrvNode.ToString());
                        ls();
                    }
                    return;
                }
            }
        }
        catch (...) {}
    }
    PrintUasge();
}

void rn(int argc, char* argv[])
{
    if (argc == 4)
    {
        int ret = oSrvNode.RenameNode(argv[2], argv[3]);
        if (PrintError(ret) == 0)
        {
			_oFo.ReWrite(oSrvNode.ToString());
            ls();
        }
    }
    else
    {
        PrintUasge();
    }
}

void add(int argc, char* argv[])
{
    uint16_t uPort = 0;
    std::string szType = "";
    std::string szName = "";
    if (argc > 4)
    {
        for (int index = 2; index < argc; index++)
        {
            char* pParam = argv[index];
            if (pParam[0] == '-')
            {
                switch (pParam[1])
                {
                case 'p':
                    uPort = (uint16_t)atoi(argv[++index]);
                    break;
                case 't':
                    szType = argv[++index];
                    break;
                case 'n':
                    szName = argv[++index];
                    break;
				default:
                    break;
                }
            }
        }
        if ((szType.empty() == false) && (szName.empty() == false))
        {
            int ret = oSrvNode.AddNode(uPort, szType, szName);
            if (PrintError(ret) == 0) 
            {
                _oFo.ReWrite(oSrvNode.ToString());
                ls();
            }
            return;
        }
    }
    PrintUasge();
}

int ProcCMD(int argc, char* argv[])
{
	if (argc > 1)
	{
		std::string szCMD = argv[1];
        if (szCMD.compare("ls") == 0)
        {
            ls();
        }
        else if (szCMD.compare("rm") == 0)
        {
            rm(argc, argv);
        }
        else if (szCMD.compare("rn") == 0)
        {
            rn(argc, argv);
        }
        else if (szCMD.compare("add") == 0)
        {
            add(argc, argv);
        }
        else
        {
            PrintUasge();
        }
		return 0;
	}
    PrintUasge();
    return -1;
}

int main(int argc, char* argv[])
{
    Init();
    ProcCMD(argc, argv);
}
