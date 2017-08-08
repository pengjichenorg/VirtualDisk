#pragma once

#include <array>
#include <string>

#include "File.h"
#include "DirectoryFile.h"
#include "GeneralFile.h"
#include "SymbolDirectoryFile.h"

const array<string, 10> partitionName = {
	"C:", "D:", "E:", "F:", "G", "H", "I", "J", "K", "L"
};

const string initPartition = "C:";

class DiskSystem
{
public:
	static DiskSystem* getInstance();
	static void destoryInstance();
	DirectoryFile* getRootDirectory();

	void setRootDirectory(DirectoryFile* root);

private:
	static DiskSystem* m_instance;
	DiskSystem();
	~DiskSystem();
	DirectoryFile* m_rootDirectory = nullptr;
};