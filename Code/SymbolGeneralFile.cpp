#include "stdafx.h"
#include "SymbolGeneralFile.h"


SymbolGeneralFile::SymbolGeneralFile()
{
	m_type = FileType::symlink;
	m_typeString = "<SYMLINK>       ";
}

SymbolGeneralFile::SymbolGeneralFile(const string& name, GeneralFile* link) : GeneralFile(name)
{
	m_type = FileType::symlink;
	m_typeString = "<SYMLINK>       ";
	m_link = link;
	if (m_link != nullptr)
	{
		m_info = " [" + m_link->getName() + "]";
	}
}

SymbolGeneralFile::SymbolGeneralFile(vector<string> infos)
{
	m_date = infos[0];
	m_time = infos[1];
	m_size = std::atoi(infos[2].c_str());
	m_name = infos[3];
	m_type = FileType(std::atoi(infos[4].c_str()));
	m_info = infos[5];

	m_type = FileType::symlink;
	m_typeString = "<SYMLINK>       ";
}

SymbolGeneralFile::~SymbolGeneralFile()
{
	// cout << "TEST: release file:" << m_name << " in " << __FUNCTION__ << endl;

	// dont delete
	m_link = nullptr;
}

GeneralFile* SymbolGeneralFile::getLinkFile() const
{
	return m_link;
}

void SymbolGeneralFile::setLinkFile(GeneralFile* link)
{
	m_link = link;
	m_info = "[" + m_link->getName() + "]";
}
