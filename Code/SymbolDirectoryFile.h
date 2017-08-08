#pragma once
#include "DirectoryFile.h"

#include <string>

using namespace std;

class SymbolDirectoryFile :
	public DirectoryFile
{
public:
	SymbolDirectoryFile();
	SymbolDirectoryFile(const string& name, DirectoryFile* link);
	SymbolDirectoryFile(vector<string> infos);
	virtual ~SymbolDirectoryFile();

	DirectoryFile* getLinkDirectory();
	void setLinkDirectory(DirectoryFile* link);

private:
	DirectoryFile* m_link;
};

