#pragma once
#include "GeneralFile.h"

#include <string>
#include <vector>

using namespace std;

class SymbolGeneralFile :
	public GeneralFile
{
public:
	SymbolGeneralFile();
	SymbolGeneralFile(const string& name, GeneralFile* link);
	SymbolGeneralFile(vector<string> infos);
	virtual ~SymbolGeneralFile();

	void setLinkFile(GeneralFile* link);
	GeneralFile* getLinkFile() const;

private:
	GeneralFile* m_link = nullptr;
};

