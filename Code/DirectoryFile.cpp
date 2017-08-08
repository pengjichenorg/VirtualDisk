#include "stdafx.h"
#include "DirectoryFile.h"

#include <algorithm>

DirectoryFile::DirectoryFile()
{
	m_type = FileType::directoryFile;
}

DirectoryFile::DirectoryFile(const string& name) : File(name)
{
	m_type = FileType::directoryFile;
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

	m_type = FileType::directoryFile;
	m_typeString = "<DIR>           ";
	m_children.emplace(".", this);
	m_children.emplace("..", nullptr);
}

DirectoryFile::~DirectoryFile()
{
	// cout << "TEST: release file:" << m_name << " in " << __FUNCTION__ << endl;
	
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

DirectoryFile* DirectoryFile::getParent() const
{
	return static_cast<DirectoryFile*>(m_parent);
}

void DirectoryFile::addChild(File* file)
{
	auto fileName = file->getName();

	file->setParent(this);
	if (file->getType() == FileType::directoryFile)
	{
		static_cast<DirectoryFile*>(file)->getChildren().find("..")->second = this;
	}

	// NOTE: in default condition, search without lower and upper will cover child
	auto it = this->search(fileName);
	if (it != m_children.end())
	{
		// NOTE: release same name resource
		this->removeChild(it->first);
	}
	m_children.emplace(file->getName(), file); // TODO: same name but different content to do what?

	// cout << "TEST: add child:" << file->getName() << endl;
	/*cout << "TEST: " << m_name << " children:" << endl;
	for (auto _it = m_children.begin(); _it != m_children.end(); _it++)
	{
		cout << "TEST: " << _it->first << "\t";
		if (_it->second != nullptr)
		{
			cout << "second:" << _it->second->getName() << endl;
		}
		else
		{
			cout << "nullptr" << endl;
		}
	}*/
}

void DirectoryFile::removeChild(const string& name)
{
	auto it = m_children.find(name);
	if (it != m_children.end())
	{
		delete it->second;
		it->second = nullptr;
		m_children.erase(it);
	}
	// cout << "TEST: remove child successfully" << endl;
}

map<string, File*>& DirectoryFile::getChildren()
{
	return m_children;
}

ChildIterator DirectoryFile::search(const string& name)
{
	// cout << "TEST: search in file " << m_name << endl;
	for (auto it = m_children.begin(); it != m_children.end(); it++)
	{
		auto lowerName = name;
		transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
		auto lowerFile = it->first;
		transform(lowerFile.begin(), lowerFile.end(), lowerFile.begin(), ::tolower);

		// NOTE: found
		if (lowerName.compare(lowerFile) == 0)
		{
			return it;
		}
	}

	// NOTE: not found
	return m_children.end();
}
