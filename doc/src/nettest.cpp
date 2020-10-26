#include <iostream>
#include <stdio.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

int compare(const void *a,const void *b)
{
       return strcmp(*(char **)a,*(char **)b);
}

int getSubnetMask(char ip[], int len)
{
    struct sockaddr_in *sin = NULL;
    struct ifaddrs *ifa = NULL, *ifList;
    string name[10];
    if (getifaddrs(&ifList) < 0)
    {
        return -1;
    }
    int i = 0;
    //找到所有网卡，并打印网卡相关信息
    for (ifa = ifList; ifa != NULL; ifa = ifa->ifa_next)
    {
        if(ifa->ifa_addr->sa_family == AF_INET)
        {
            string ifname(ifa->ifa_name);
            name[i]=ifname;
            i++;
            printf("interfaceName: %s\n", ifa->ifa_name);

            sin = (struct sockaddr_in *)ifa->ifa_addr;
            printf("ipAddress: %s\n", inet_ntoa(sin->sin_addr));

            sin = (struct sockaddr_in *)ifa->ifa_dstaddr;
            printf("broadcast: %s\n", inet_ntoa(sin->sin_addr));

            sin = (struct sockaddr_in *)ifa->ifa_netmask;
            printf("subnetMask: %s\n\n", inet_ntoa(sin->sin_addr));
        }
    }

    freeifaddrs(ifList);
    return -1;
}


int main(int argc, char *argv[])
{
    char ip[64];
    if(int ret = getSubnetMask(ip,sizeof(ip)) < 0)
    {
        printf("find ip error\n");
        return -1;
    }
    printf("ip=%s\n",ip);
    return 0;
}
