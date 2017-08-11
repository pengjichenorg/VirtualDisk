#include "stdafx.h"
#include "DirectoryFile.h"

#include <algorithm>

DirectoryFile::DirectoryFile()
{
	m_type = FileType::dirFile;
}

DirectoryFile::DirectoryFile(const string& name) : File(name)
{
	m_type = FileType::dirFile;
	m_typeString = "<DIR>           ";
	m_children.emplace(".", this);
	m_children.emplace("..", nullptr);
}

DirectoryFile::DirectoryFile(vector<string> infos)
{
	m_date = infos[0];
	m_time = infos[1];
	m_size = std::atoi(infos[2].c_str());
	m_name = infos[3];
	m_type = FileType(std::atoi(infos[4].c_str()));
	m_info = infos[5];

	m_type = FileType::dirFile;
	m_typeString = "<DIR>           ";
	m_children.emplace(".", this);
	m_children.emplace("..", nullptr);
}

DirectoryFile::~DirectoryFile()
{
	// NOTE: step over . and ..
	for (auto it = m_children.begin(); it != m_children.end(); it++)
	{
		if (it->first.compare(".") == 0 || it->first.compare("..") == 0)
		{
			continue;
		}
		delete it->second;
		it->second = nullptr;
	}
}

void DirectoryFile::addChild(File* file)
{
	file->setParent(this);
	if (file->getType() == FileType::dirFile)
	{
		static_cast<DirectoryFile*>(file)->getChildren().find("..")->second = this;
	}

	// NOTE: in default condition, search without lower and upper will cover child
	auto it = this->search(file->getName(), file->getType());
	if (it != m_children.end())
	{
		// NOTE: release same name resource
		this->removeChild(it->first, file->getType());
	}
	m_children.emplace(file->getName(), file);
}

void DirectoryFile::removeChild(const string& name, FileType type)
{
	auto it = this->search(name, type);
	if (it != m_children.end())
	{
		delete it->second;
		it->second = nullptr;
		m_children.erase(it);
	}
}

map<string, File*>& DirectoryFile::getChildren()
{
	return m_children;
}

ChildIterator DirectoryFile::search(const string& name, FileType type)
{
	for (auto it = m_children.begin(); it != m_children.end(); it++)
	{
		auto lowerName = name;
		transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
		auto lowerFile = it->first;
		transform(lowerFile.begin(), lowerFile.end(), lowerFile.begin(), ::tolower);

		// NOTE: same name and same file type
		if (lowerName.compare(lowerFile) == 0 && it->second->getType() == type)
		{
			return it;
		}
	}

	// NOTE: not found
	return m_children.end();
}
