#include "stdafx.h"
#include "GeneralFile.h"

#include <fstream>

GeneralFile::GeneralFile()
{
}

GeneralFile::GeneralFile(const string& name) : File(name)
{
	m_type = FileType::generalFile;
	m_typeString = "      ";
}

GeneralFile::GeneralFile(vector<string> infos)
{
	m_date = infos[0];
	m_time = infos[1];
	m_size = std::atoi(infos[2].c_str());
	m_name = infos[3];
	m_type = FileType(std::atoi(infos[4].c_str()));
	m_info = infos[5];

	// TODO: read data
	auto dataFile = infos[7];

	// cout << "TEST: " << dataFile << endl;
	if (dataFile.compare("nullptr") != 0)
	{
		ifstream inputFile;
		inputFile.open(dataFile, ios::binary);
		if (inputFile)
		{
			inputFile.seekg(0, ios::end);
			m_bin.resize((unsigned int)inputFile.tellg());
			inputFile.seekg(0, ios::beg);
			inputFile.read(reinterpret_cast<char*>(m_bin.data()), m_bin.size());
			inputFile.close();
		}
		m_size = m_bin.size();
	}

	m_type = FileType::generalFile;
	m_typeString = "      ";
}


GeneralFile::~GeneralFile()
{
	// cout << "TEST: release file:" << m_name << " in " << __FUNCTION__ << endl;
}

vector<unsigned char> GeneralFile::getBinData() const
{
	return m_bin;
}

void GeneralFile::setBinData(const vector<unsigned char>& bin)
{
	m_bin = bin;
	m_size = m_bin.size();
}
