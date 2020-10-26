#include <stdio.h>
#include <iostream>
#include "SocketService.h"
#include "Log.h"
#include "FileOpt.h"
#include "ServiceNode.h"

using namespace std;

int main(int argc, char* argv[])
{
    FileOpt * fo = new FileOpt(DEFAULTCFGFILE);
    ServiceNode sv;
    sv.LoadFromStr(fo->GetFileContent());
    printf("%s\r\n",sv.SearchPort(21).c_str());


	return 0;
}

