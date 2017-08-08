// VD.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "VDos.h"

using namespace std;

int main()
{
	// NOTE: initialize disksystem
	DiskSystem::getInstance();
	VDos vdos;
	vdos.run();
	// NOTE: destory disksystem
	DiskSystem::destoryInstance();
    return 0;
}