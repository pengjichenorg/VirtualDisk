#include "stdafx.h"
#include "SymbolDirectoryFile.h"

SymbolDirectoryFile::SymbolDirectoryFile()
{
	m_type = FileType::symlinkd;
}

SymbolDirectoryFile::SymbolDirectoryFile(const string& name, DirectoryFile* link) : DirectoryFile(name)
{
	m_type = FileType::symlinkd;
	m_typeString = "<SYMLINKD>      ";
	m_link = link;
	if (m_link != nullptr)
	{
		m_info = " [" + m_link->getName() + "]";
	}
}

SymbolDirectoryFile::SymbolDirectoryFile(vector<string> infos)
{
	m_date = infos[0];
	m_time = infos[1];
	m_size = std::atoi(infos[2].c_str());
	m_name = infos[3];
	m_type = FileType(std::atoi(infos[4].c_str()));
	m_info = infos[5];

	m_type = FileType::symlinkd;
	m_typeString = "<SYMLINKD>      ";
}

SymbolDirectoryFile::~SymbolDirectoryFile()
{
	// cout << "TEST: release file:" << m_name << " in " << __FUNCTION__ << endl;

	// dont delete
	m_link = nullptr;
}

DirectoryFile* SymbolDirectoryFile::getLinkDirectory()
{
	return m_link;
}

void SymbolDirectoryFile::setLinkDirectory(DirectoryFile* link)
{
	m_link = link;
	m_info = "[" + m_link->getName() + "]";
}
