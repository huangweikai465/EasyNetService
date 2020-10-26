#pragma once
#include "stdio.h"
#include "string"

#define CONFIGDIR "ens"

#if defined(WIN32)
#define DIRDIV "\\"
#else
#define DIRDIV "/"
#endif

class FileOpt
{
private:
    static std::string c_szPath;
	const std::string m_szFileName;
    const std::string m_szConfigPath;
    std::string m_szFileContent;

    static bool IsAccessible(std::string _szFileOrFir);   //判断文件是否存在以及是否可访问
    static bool CreateDefaultDirectory(std::string _szDirName);
    static std::string GetConfigPath(void);

    void ReadFile(void);
    bool CreateCfgFile(std::string _szFilePath,std::string _szFileName,std::string _szDefaultText = "");
public:
    static void SetPath(std::string _szPath);
    FileOpt(std::string _szFileName,std::string _szDefaultContent = "");
	~FileOpt();
	
	void ReflashFileContent(void);
	bool IsJsonContent(void);
	bool IsJsonArray(void);
	void ReWrite(std::string _szContent);
    std::string GetFilePath(void);
	std::string GetFileName(void);
	std::string GetFileContent(void);
};

