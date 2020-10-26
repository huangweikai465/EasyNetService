#include "IpInfo.h"

void IPInfo::ClearDevicesRecorder()
{
    for (size_t i = 0; i < m_lDevices.size(); i++)
    {
        delete m_lDevices[i];
    }
    m_lDevices.clear();
}
#if defined(WIN32)
void IPInfo::InitDevice(PIP_ADAPTER_INFO _pIpAdapterInfo)
{
    IPAddrRecorder oIpAddr(_pIpAdapterInfo->IpAddressList.IpAddress.String);
    IPAddrRecorder oMask(_pIpAdapterInfo->IpAddressList.IpMask.String);

    if ((oIpAddr.lAddr != 0) && (oMask.lAddr != 0))
    {
        IPDevice* pNewDev = new IPDevice;
        pNewDev->SetName(_pIpAdapterInfo->Description);
        pNewDev->SetAddr(oIpAddr.lAddr);
        pNewDev->SetMask(oMask.lAddr);
        pNewDev->SetBroadcastAddr( oIpAddr.lAddr | (~oMask.lAddr));
        
//        char buf[18] = { 0 };
//        sprintf_s(buf,18, "%02X:%02X:%02X:%02X:%02X:%02X",
//			(uint8_t)_pIpAdapterInfo->Address[0],
//			(uint8_t)_pIpAdapterInfo->Address[1],
//			(uint8_t)_pIpAdapterInfo->Address[2],
//			(uint8_t)_pIpAdapterInfo->Address[3],
//			(uint8_t)_pIpAdapterInfo->Address[4],
//			(uint8_t)_pIpAdapterInfo->Address[5]
//            );
//        pNewDev->SetMacAddr(buf);
        m_lDevices.push_back(pNewDev);
    }
}

#elif defined(LINUX) || defined (MACOS)

void IPInfo::InitDevice(struct ifaddrs * _pIfa )
{
    std::string szMac = "";
    IPDevice * pNewDev = new IPDevice;
    try{
        if(_pIfa->ifa_addr->sa_family == AF_INET)
        {
            pNewDev->SetName(_pIfa->ifa_name);
            pNewDev->SetAddr(((struct sockaddr_in*)(_pIfa->ifa_addr))->sin_addr.s_addr);
            pNewDev->SetMask(((struct sockaddr_in*)(_pIfa->ifa_netmask))->sin_addr.s_addr);
            pNewDev->SetBroadcastAddr(((struct sockaddr_in*)(_pIfa->ifa_dstaddr))->sin_addr.s_addr);
            m_lDevices.push_back(pNewDev);
        }
    }
    catch(...)
    {
        delete pNewDev;
    }
}
#endif

IPInfo::IPInfo()
{
}

IPInfo::~IPInfo()
{
    ClearDevicesRecorder();
}

int IPInfo::Init(void)
{
#if defined(WIN32)
	IP_ADAPTER_INFO pIpAdapterInfo[MAXIPDEVICES];// 默认20个网卡空间 足够了 
	unsigned long stSize = sizeof(IP_ADAPTER_INFO) * MAXIPDEVICES;
	// 获取所有网卡信息，参数二为输入输出参数 
    int nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);
	// 空间不足
	if (ERROR_BUFFER_OVERFLOW == nRel) {
		// 释放空间
		return -1; 
	}
    PIP_ADAPTER_INFO m_pIpAdapterInfo = pIpAdapterInfo;
    while(m_pIpAdapterInfo)
    {
        InitDevice(m_pIpAdapterInfo);
        m_pIpAdapterInfo = m_pIpAdapterInfo->Next;
    }
#elif defined(LINUX) || defined (MACOS)
    struct ifaddrs *ifList;
    if (getifaddrs(&ifList) < 0)
    {
        return -1;
    }
    //找到所有网卡，并打印网卡相关信息
    for (struct ifaddrs *ifa = ifList; ifa != NULL; ifa = ifa->ifa_next)
    {
        InitDevice(ifa);
    }
#endif
    return 0;
}

int IPInfo::GetDeviceCount(void)
{
    return (int)m_lDevices.size();
}

IPDevice * IPInfo::GetDevice(uint16_t _index)
{
    if (_index < m_lDevices.size())
        return m_lDevices[_index];
    return nullptr;
}

IPDevice::IPDevice()
{
    m_szName = "";
    szMacAddr = "";
    m_oAddr.lAddr = 0;
    m_oBroadcastAddr.lAddr = 0;
    m_oMask.lAddr = 0;
}

IPDevice::IPDevice(IPDevice& _oDev)
{
    m_szName = _oDev.m_szName;
    m_oAddr = _oDev.m_oAddr;
    m_oMask = _oDev.m_oMask;
    m_oBroadcastAddr = _oDev.m_oBroadcastAddr;
    szMacAddr = _oDev.szMacAddr;
}

IPDevice::~IPDevice()
{
}

void IPDevice::SetName(std::string _szName)
{
    m_szName = _szName;
}

void IPDevice::SetAddr(IPAddrRecorder& _oAddr)
{
    m_oAddr = _oAddr;
}

void IPDevice::SetMask(IPAddrRecorder& _oMask)
{
    m_oMask = _oMask;
}

void IPDevice::SetBroadcastAddr(IPAddrRecorder& _oBroadcastAddr)
{
    m_oBroadcastAddr = _oBroadcastAddr;
}

void IPDevice::SetAddr(uint32_t _oAddr)
{
    m_oAddr.lAddr = _oAddr;
    //Log::Debug("Set Addr %s\r\n",m_oAddr.ToString().c_str());
}

void IPDevice::SetMask(uint32_t _oMask)
{
    m_oMask.lAddr = _oMask;
    //Log::Debug("Set Mask %s\r\n",m_oMask.ToString().c_str());
}

void IPDevice::SetBroadcastAddr(uint32_t _oBroadcastAddr)
{
    m_oBroadcastAddr.lAddr = _oBroadcastAddr;
    //Log::Debug("Set BroadCast %s\r\n",m_oBroadcastAddr.ToString().c_str());
}

void IPDevice::SetMacAddr(std::string _szMacAddr)
{
    szMacAddr = _szMacAddr;
    //Log::Debug("Set MacAddr %s\r\n",szMacAddr.c_str());
}

std::string IPDevice::GetName(void)
{
    return m_szName;
}

IPAddrRecorder IPDevice::GetAddr(void)
{
    return m_oAddr;
}

IPAddrRecorder IPDevice::GetMask(void)
{
    return m_oMask;
}

IPAddrRecorder IPDevice::GetBroadcastAddr(void)
{
    return m_oBroadcastAddr;
}

std::string IPDevice::GetMacAddr(void)
{
    return szMacAddr;
}
