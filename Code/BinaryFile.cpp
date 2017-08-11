#include "stdafx.h"
#include "BinaryFile.h"

#include <fstream>

BinaryFile::BinaryFile()
{
}

BinaryFile::BinaryFile(const string& name) : File(name)
{
	m_type = FileType::binFile;
	m_typeString = "      ";
}

BinaryFile::BinaryFile(vector<string> infos)
{
	m_date = infos[0];
	m_time = infos[1];
	m_size = std::atoi(infos[2].c_str());
	m_name = infos[3];
	m_type = FileType(std::atoi(infos[4].c_str()));
	m_info = infos[5];

	auto dataFile = infos[7];

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

	m_type = FileType::binFile;
	m_typeString = "      ";
}


BinaryFile::~BinaryFile()
{

}

vector<unsigned char> BinaryFile::getBinData() const
{
	return m_bin;
}

void BinaryFile::setBinData(const vector<unsigned char>& bin)
{
	m_bin = bin;
	m_size = m_bin.size();
}
