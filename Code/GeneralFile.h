#pragma once
#include "File.h"

#include <string>
#include <vector>

using namespace std;

class GeneralFile :
	public File
{
public:
	GeneralFile();
	GeneralFile(const string& name);
	GeneralFile(vector<string> infos);
	virtual ~GeneralFile();

	vector<unsigned char> getBinData() const;
	void setBinData(const vector<unsigned char>& bin);

private:
	vector<unsigned char> m_bin;
};

