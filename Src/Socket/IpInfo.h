#pragma once

#include <stdio.h>

#include <string>
#include <iostream>
#include <vector>
#include <regex>

#include "SocketInclude.h"

#if defined (WIN32)
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <Iphlpapi.h>
#define MAXIPDEVICES 20
//#define _WINSOCK_DEPRECATED_NO_WARNINGS

#elif defined(LINUX) || defined (MACOS)
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#endif

//记录单个IP信息的结构体
struct IPAddrRecorder
{
	union {
		uint8_t uAddr[4];
		uint32_t lAddr;
	};
	IPAddrRecorder(uint32_t _uIP = 0) {
		lAddr = _uIP;
	}
	IPAddrRecorder(uint32_t * _pIP) {
		lAddr = *_pIP;
	}
	IPAddrRecorder(const IPAddrRecorder & _oIp) {
		lAddr = _oIp.lAddr;
	}
	IPAddrRecorder(const std::string& _szIP) {
		FromString(_szIP);
	}
	~IPAddrRecorder() {}
	IPAddrRecorder & operator =(const IPAddrRecorder& _oIp)
	{
		lAddr = _oIp.lAddr;
		return *this;
	}
	std::string ToString()
	{
		char szAddr[16];
		sprintf(szAddr, "%d.%d.%d.%d", uAddr[0], uAddr[1], uAddr[2], uAddr[3]);
		return szAddr;
	}
	//TODO：从字符串中提取IP地址
	void FromString(const std::string _szIp)
	{
		try
		{
			std::regex regMatch("((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})(\\.((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})){3}");
			std::smatch cmMatchResult;
			if (regex_search(_szIp, cmMatchResult, regMatch))
			{
				std::string szIP = cmMatchResult.str();
				size_t lastindex = 0;
				for (int i = 0;i<4;i++)
				{
					size_t index = szIP.find('.', lastindex);
					std::string szIpItem = szIP.substr(lastindex, index-lastindex);
					uAddr[i] = atoi(szIpItem.c_str());
					lastindex = index + 1;
				}
			}
			else
				lAddr = 0;
		}
		catch (...)
		{
		}
	}
};

//IP设备描述，包含设备的名称、地址、掩码、广播地址、Mac地址等
class IPDevice {
private:
	std::string m_szName;
	IPAddrRecorder m_oAddr;
	IPAddrRecorder m_oMask;
	IPAddrRecorder m_oBroadcastAddr;
	std::string szMacAddr;
public:
	IPDevice();
	IPDevice(IPDevice& _oDev);
	~IPDevice();

	//参数设置接口
	void SetName(std::string _szName);
	void SetAddr(IPAddrRecorder& _oAddr);
	void SetMask(IPAddrRecorder& _oMask);
	void SetBroadcastAddr(IPAddrRecorder& _oBroadcastAddr);
	void SetAddr(uint32_t _oAddr);
	void SetMask(uint32_t _oMask);
	void SetBroadcastAddr(uint32_t _oBroadcastAddr);
	void SetMacAddr(std::string _szMacAddr);

	//参数读取接口
	std::string GetName(void);
	IPAddrRecorder GetAddr(void);
	IPAddrRecorder GetMask(void);
	IPAddrRecorder GetBroadcastAddr(void);
	std::string GetMacAddr(void);
};
typedef std::vector<IPDevice*> DeviceList;
//IP信息结构体，初始化所有的IP设备的数据
class IPInfo
{
private:
	DeviceList m_lDevices;

	void ClearDevicesRecorder();
#if defined(WIN32)

	void InitDevice(PIP_ADAPTER_INFO _pIpAdapterInfo);
#elif defined(LINUX) || defined (MACOS)
	void InitDevice(struct ifaddrs * _pIfa );
#endif
public:
	//该构造和析构不做事情
	IPInfo();
	~IPInfo();

	//初始化时，会读取当前的网络IP，并写入m_pDevices中
	int Init(void);

	int GetDeviceCount(void);
	IPDevice * GetDevice(uint16_t _index);
};

