#include "stdafx.h"
#include "SymlinkdFile.h"

SymlinkdFile::SymlinkdFile()
{
	m_type = FileType::symlinkd;
}

SymlinkdFile::SymlinkdFile(const string& name, DirectoryFile* link) : DirectoryFile(name)
{
	m_type = FileType::symlinkd;
	m_typeString = "<SYMLINKD>      ";
	m_link = link;
	if (m_link != nullptr)
	{
		m_info = " [" + m_link->getName() + "]";
	}
}

SymlinkdFile::SymlinkdFile(vector<string> infos)
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

SymlinkdFile::~SymlinkdFile()
{
	// NOTE: dont delete
	m_link = nullptr;
}

void SymlinkdFile::addChild(File * file)
{
	m_link->addChild(file);
}

void SymlinkdFile::removeChild(const string & name, FileType type)
{
	m_link->removeChild(name, type);
}

map<string, File*>& SymlinkdFile::getChildren()
{
	return m_link->getChildren();
}

ChildIterator SymlinkdFile::search(const string & name, FileType type)
{
	return m_link->search(name, type);
}

DirectoryFile* SymlinkdFile::getLinkDirectory()
{
	return m_link;
}

void SymlinkdFile::setLinkDirectory(DirectoryFile* link)
{
	m_link = link;
	m_info = "[" + m_link->getName() + "]";
}
