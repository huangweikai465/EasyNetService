#include "FileOpt.h"
#include "../cJSON/cJSON.h"
#include "../Socket/SocketInclude.h"

#if defined(WIN32)
#include <io.h>
#define ACCESS _access_s
#define	R_OK	 4		/* Test for read permission.  */
#define	W_OK	 2		/* Test for write permission.  */
#define	X_OK	 1		/* Test for execute permission.  */
#define	F_OK 0		/* Test for existence.  */
#elif defined(LINUX) || defined(MACOS)
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#define ACCESS access
#endif

std::string FileOpt::c_szPath = GetConfigPath();

void FileOpt::ReadFile(void)
{
	FILE* fp;
    std::string szFile = m_szConfigPath + m_szFileName;
    fp = fopen(szFile.c_str(), "r+");
	if (fp != nullptr)
	{
		fseek(fp, 0, SEEK_END);
		size_t uFileLen = ftell(fp);
		char* szFileContent = new char[uFileLen + 1];
		fseek(fp, 0, SEEK_SET);
		fread(szFileContent, 1, uFileLen, fp);
		fclose(fp);
		szFileContent[uFileLen] = '\0';
		m_szFileContent = szFileContent;
		delete[] szFileContent;
	}
	else
	{
        Log::Error("Open File %s Failed , Exit",GetErrNo,szFile.c_str());
	}
}

bool FileOpt::IsAccessible(std::string _szFileOrDir)
{
    if (ACCESS(_szFileOrDir.c_str(), F_OK) == 0)  //检查是否存在
	{
        return true;
	}
    return false;
}

bool FileOpt::CreateCfgFile(std::string _szFilePath,std::string _szFileName,std::string _szDefaultText)
{
    FILE* fp;
    std::string szFile = _szFilePath + _szFileName;
    fp = fopen(szFile.c_str(), "w+");
    if (fp != nullptr)
    {
        if(_szDefaultText.empty() == false)
            fwrite(_szDefaultText.c_str(), 1, _szDefaultText.length(), fp);
        fclose(fp);
        return true;
    }
    return false;
}

bool FileOpt::CreateDirectory(std::string _szDirName)
{
#if defined (WIN32)
    if (CreateDirectoryA(_szDirName.c_str(), nullptr) == 0)
    {
        return true;
    }
#elif defined(LINUX) || defined (MACOS)
    if(mkdir(_szDirName.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0)
    {
        std::string chmod = "chmod 755 " + _szDirName;
        system(chmod.c_str());
        return true;
    }
#endif
    return false;
}

std::string FileOpt::GetConfigPath()
{
#if defined(WIN32)
    std::string szHome = getenv("HOMEDRIVE");
    szHome += "\\";
    szHome += getenv("HOMEPATH");
    std::string szConfigPath = szHome + "\\" + CONFIGDIR;
    return szConfigPath + "\\";

#elif defined(LINUX) || defined (MACOS)
    std::string szHome = getenv("HOME");
    std::string szConfigPath = szHome + "/" + CONFIGDIR;
    return szConfigPath + "/";
#endif
}


void FileOpt::SetPath(std::string _szPath)
{
    c_szPath = _szPath;
}

FileOpt::FileOpt(std::string _szFileName,std::string szDefaultContent)
      :m_szFileName(_szFileName),
      m_szConfigPath(c_szPath)
{
    std::string szFile = m_szConfigPath + m_szFileName;
    int sAccessible = IsAccessible(m_szConfigPath);
    if(sAccessible == false)
    {
        if(IsAccessible(m_szConfigPath) == false)
        {
            Log::Normal("Can't access to %s,creating...\r\n",m_szConfigPath.c_str());
            if(CreateDirectory(m_szConfigPath) == false)
            {
                Log::Error("Mkdir %s Failed ",GetErrNo,m_szConfigPath.c_str());
            }
            Log::Normal("Create Success\r\n");
        }
    }
    sAccessible = IsAccessible(szFile);
    if (sAccessible == false)
    {
        Log::Normal("Can't access to %s,creating...\r\n",szFile.c_str());
        if(CreateCfgFile(m_szConfigPath,m_szFileName,szDefaultContent) == false)
        {
            Log::Error("Create File %s Failed!",GetErrNo,szFile.c_str());
            exit(1);
        }
        Log::Normal("Create Success\r\n");
    }
    ReadFile();
}

FileOpt::~FileOpt()
{
}

void FileOpt::ReflashFileContent(void)
{
	ReadFile();
}

bool FileOpt::IsJsonContent(void)
{
	cJSON* cjson;
	cjson = cJSON_Parse(m_szFileContent.c_str());
	if (cjson == nullptr)
	{
		return false;
	}
	cJSON_Delete(cjson);
	return true;
}

bool FileOpt::IsJsonArray(void)
{
	cJSON* cjson;
	cjson = cJSON_Parse(m_szFileContent.c_str());
	if (cjson == nullptr)
	{
		return false;
	}
	if (cJSON_IsArray(cjson))
	{
		cJSON_Delete(cjson);
		return true;
	}
	cJSON_Delete(cjson);
	return false;
}

void FileOpt::ReWrite(std::string _szContent)
{
	FILE* fp;
    std::string szFile = m_szConfigPath + m_szFileName;
    fp = fopen(szFile.c_str(), "w+");
	if (fp != nullptr)
	{
		fwrite(_szContent.c_str(), 1, _szContent.length(), fp);
		fclose(fp);
	}
}

std::string FileOpt::GetFilePath()
{
    return m_szConfigPath;
}

std::string FileOpt::GetFileName(void)
{
    return  m_szFileName;
}

std::string FileOpt::GetFileContent(void)
{
	return m_szFileContent;
}
