#pragma once
//#define DEBUG
#include <string>

typedef void (*LogHook)(std::string _szLog);

class Log
{
private:
    static std::string GetLogStr(const char * _pLog,va_list *_pVaList);
public:
    static LogHook m_gNormalLogHook;
    static LogHook m_gWarnningHook;
    static LogHook m_gDebugHook;
    static LogHook m_gErrHook;
    static void Normal(const char * pLog,...);
	static void Warnning(const char * pLog,...);
	static void Error(const char * pLog,int iErrcode,...);
	static void Debug(const char * pLog,...);
};

