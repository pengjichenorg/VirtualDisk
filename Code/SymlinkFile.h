#pragma once

#include "BinaryFile.h"

#include <vector>

using namespace std;

class SymlinkFile :
	public BinaryFile
{
public:
	SymlinkFile();
	SymlinkFile(const string& name, BinaryFile* link);
	SymlinkFile(vector<string> infos);
	virtual ~SymlinkFile();

	void setLinkFile(BinaryFile* link);
	BinaryFile* getLinkFile() const;

private:
	BinaryFile* m_link = nullptr;
};

