#include "stdafx.h"
#include "Object.h"
#include "DiskSystem.h"

bool Object::createMode = false;

Object::Object(const Path& path, vector<string> arguments, DirectoryFile* currentDirectory) : m_path(path), m_arguments(arguments)
{
	if (!m_path.m_pathQueue.empty())
	{
		auto pathQueue = m_path.m_pathQueue;
		// NOTE: deal each path
		while (!pathQueue.empty())
		{
			// NOTE: get path str
			auto _path = pathQueue.front();
			// NOTE: upward
			if (_path.compare("..") == 0)
			{
				// cout << "TEST: path .." << endl;
				if (currentDirectory->getParent() != DiskSystem::getInstance()->getRootDirectory())
				{
					currentDirectory = currentDirectory->getParent();
					// cout << "TEST: current:" << currentDirectory->getName() << endl;
					if (m_fileParent != nullptr)
					{
						m_fileParent = m_fileParent->getParent();
						// cout << "TEST: current parent:" << m_fileParent->getName() << endl;
					}
				}
				else
				{
					m_fileParent = currentDirectory;
				}
			}
			// NOTE: no change
			else if (_path.compare(".") == 0)
			{
				// cout << "TEST: path ." << endl;
				if (currentDirectory->getParent() != DiskSystem::getInstance()->getRootDirectory())
				{
					currentDirectory = currentDirectory;
					// cout << "TEST: current:" << currentDirectory->getName() << endl;
					m_fileParent = currentDirectory->getParent();
					// cout << "TEST: current parent:" << currentDirectory->getParent()->getName() << endl;
				}
				else
				{
					m_fileParent = currentDirectory;
				}
			}
			// NOTE: downward
			else
			{
				m_fileParent = currentDirectory;
				// cout << "TEST: find " << _path << endl;
				// NOTE: found
				if (currentDirectory->search(_path) != currentDirectory->getChildren().end())
				{
					// cout << "TEST: found" << endl;
					// NOTE: this is the last one we could found
					if (createMode && pathQueue.size() == 1)
					{
						// IDEA: or output error message here
						// cout << "TEST: the last one has been found" << endl;
						currentDirectory = nullptr;
					}
					else
					{
						currentDirectory = static_cast<DirectoryFile*>(currentDirectory->search(_path)->second);
					}
				}
				// NOTE: lost
				else
				{
					// cout << "TEST: lost" << endl;
					// cout << "TEST: create mode:" << createMode << endl;
					if (createMode)
					{
						// cout << "TEST: in lost currentDirectory:" << currentDirectory->getName() << endl;
						auto childDir = new DirectoryFile(_path);
						currentDirectory->addChild(childDir);
						currentDirectory = childDir;
					}
					else
					{
						currentDirectory = nullptr;
						break;
					}
				}
			}
			pathQueue.pop();
		}
	}
	else
	{
		// cout << "TEST: Empty File!" << endl;
	}

	m_file = currentDirectory;

	if (m_file != nullptr)
	{
		// cout << "TEST: Object.file:" << m_file->getName() << endl;
	}
	else
	{
		// cout << "TEST: m_file is nullptr" << endl;
	}
	if (m_fileParent != nullptr)
	{
		// cout << "TEST: Object.fileParent:" << m_fileParent->getName() << endl;
	}
	else
	{
		// cout << "TEST: m_fileParent is nullptr" << endl;
	}
}