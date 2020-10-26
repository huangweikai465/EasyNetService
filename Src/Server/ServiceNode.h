#pragma once
#include <string>
#include "../cJSON/JsonOpt.h"

class ServiceNode
{
    JsonOpt m_oJo;

public:
	ServiceNode();
	~ServiceNode();

	int LoadFromStr(std::string _szJsonStr);
    JsonOpt SearchType(std::string _szType);
    JsonOpt SearchName(std::string _szName);
    JsonOpt SearchPort(uint16_t _uPort);
    std::string Print(void);
    std::string ToString();

	int AddNode(uint16_t _uPort,std::string _szType,std::string _szName);
	int RemoveNode(uint16_t _uPort);
	int RemoveNode(std::string szName);
	int RenameNode(std::string szOldName, std::string szNewName);
};

