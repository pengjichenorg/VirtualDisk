// VirtualDisk.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include "VDos.h"

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