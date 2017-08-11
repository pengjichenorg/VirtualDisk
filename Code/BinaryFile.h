#pragma once

#include "File.h"

#include <vector>

using namespace std;

class BinaryFile :
	public File
{
public:
	BinaryFile();
	BinaryFile(const string& name);
	BinaryFile(vector<string> infos);
	virtual ~BinaryFile();

	vector<unsigned char> getBinData() const;
	void setBinData(const vector<unsigned char>& bin);

private:
	vector<unsigned char> m_bin;
};

