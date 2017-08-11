#include "stdafx.h"
#include "SymlinkFile.h"

SymlinkFile::SymlinkFile()
{
	m_type = FileType::symlink;
	m_typeString = "<SYMLINK>       ";
}

SymlinkFile::SymlinkFile(const string& name, BinaryFile* link) : BinaryFile(name)
{
	m_type = FileType::symlink;
	m_typeString = "<SYMLINK>       ";
	m_link = link;
	if (m_link != nullptr)
	{
		m_info = " [" + m_link->getName() + "]";
	}
}

SymlinkFile::SymlinkFile(vector<string> infos)
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

SymlinkFile::~SymlinkFile()
{
	// NOTE: dont delete
	m_link = nullptr;
}

BinaryFile* SymlinkFile::getLinkFile() const
{
	return m_link;
}

void SymlinkFile::setLinkFile(BinaryFile* link)
{
	m_link = link;
	m_info = "[" + m_link->getName() + "]";
}
