#include "ServiceNode.h"
#include "../Log/Log.h"

ServiceNode::ServiceNode()
{
}

ServiceNode::~ServiceNode()
{
}

int ServiceNode::LoadFromStr(std::string _szJsonStr)
{
    m_oJo.LoadFromString((_szJsonStr));
    if(m_oJo.IsNull())
        return -1;
    return 0;
}

JsonOpt ServiceNode::SearchType(std::string _szType)
{
    JsonOpt oResult;
    if (m_oJo.IsArray())
	{
        ForEachArrayItem(oNode,m_oJo)
		{
            oNode = m_oJo[index];
            if (oNode.IsNull())
                continue;
            std::string szType = oNode["Type"].ToString();
            if (szType.find(_szType) != std::string::npos)
            {
                oResult.AddArrayItem(oNode);
            }
		}
	}
    return oResult;
}

JsonOpt ServiceNode::SearchName(std::string _szName)
{
    JsonOpt oResult;
    if (m_oJo.IsArray())
    {
        ForEachArrayItem(oNode,m_oJo)
        {
            oNode = m_oJo[index];
            if (oNode.IsNull() )
                continue;
            std::string szName = oNode["Name"].ToString();
            if (szName.find(_szName) != std::string::npos)
            {
                Log::Normal("Get Node %s\r\n",oNode.ToString().c_str());
                oResult.AddArrayItem(oNode);
            }
        }
    }
    return oResult;
}

JsonOpt ServiceNode::SearchPort(uint16_t _uPort)
{
    JsonOpt oResult;
    if (m_oJo.IsArray())
    {
        ForEachArrayItem(oNode,m_oJo)
        {
            oNode = m_oJo[index];
            if (oNode.IsNull() )
                continue;
            int sPort = oNode["Port"].ToInt();
            if ((uint16_t)sPort == _uPort)
            {
                oResult.AddArrayItem(oNode);
            }
        }
    }
    return oResult;
}

std::string ServiceNode::Print(void)
{
	std::string szOutput = "port\ttype\tname\r\n";
    if (m_oJo.IsArray())
    {
        ForEachArrayItem(oNode,m_oJo)
        {
            if(oNode.IsNull())
                continue;
            uint16_t uPort = (uint16_t)oNode["Port"].ToInt();
            std::string szType = oNode["Type"].ToString();
            std::string szName = oNode["Name"].ToString();
            szOutput += std::to_string(uPort) + "\t" + szType + "\t" + szName + "\r\n";
        }
	}
    return szOutput;
}



std::string ServiceNode::ToString()
{
    return m_oJo.ToString();
}

int ServiceNode::AddNode(uint16_t _uPort, std::string _szType, std::string _szName)
{
    JsonOpt oNode;
    oNode.AddItem("Port",_uPort);
    oNode.AddItem("Type",_szType);
    oNode.AddItem("Name",_szName);
    m_oJo.AddArrayItem(oNode);
	return 0;
}

int ServiceNode::RemoveNode(uint16_t _uPort)
{
    if (m_oJo.IsArray())
	{
		int ret = -1;
        ForEachArrayItem(oNode,m_oJo)
        {
            if (oNode.IsNull())
                continue;
            uint16_t uPort = (uint16_t)oNode["Port"].ToInt();
            if (uPort == _uPort)
            {
                m_oJo.RemoveArrayItem(index);
                index--;
                ret = 0;
            }
		}
		return ret;
	}
	return -3;
}

int ServiceNode::RemoveNode(std::string _szName)
{
    if (m_oJo.IsArray())
    {
        int ret = -1;
        ForEachArrayItem(oNode,m_oJo)
        {
            oNode = m_oJo[index];
            if (oNode.IsNull() )
                continue;
            std::string szName = oNode["Name"].ToString();
            if (szName.compare(_szName) == 0)
            {
                m_oJo.RemoveArrayItem(index);
                index --;
                ret = 0;
            }
        }
        return ret;
    }
	return -3;
}

int ServiceNode::RenameNode(std::string _szOldName, std::string _szNewName)
{
    if (m_oJo.IsArray())
    {
        int ret = -1;
        ForEachArrayItem(oNode,m_oJo)
        {
            oNode = m_oJo[index];
            if (oNode.IsNull() )
                continue;
            std::string szName = oNode["Name"].ToString();
            if (szName.compare(_szOldName) == 0)
            {
                m_oJo.ReplaceItem("Name",_szNewName);
                index --;
                ret = 0;
            }
        }
        return ret;
    }
    return -3;
}




