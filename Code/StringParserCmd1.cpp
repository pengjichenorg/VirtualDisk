#include "stdafx.h"
#include "StringParserCmd1.h"
#include "ObjectGenerator.h"
#include "DiskSystem.h"

// NOTE: in cmd1, the last one must be objectString, others are argumentStrings
// NOTE: in cmd1, if the last one use '/', then treat all arguments as path
// NOTE: in cmd1, if the last one doesn't use '/', then last one must be objectString

StringParserCmd1::StringParserCmd1(const queue<string>& strings, stack<File*> currentDirectory) : StringParserCmd(strings, currentDirectory)
{
}

StringParserCmd1::~StringParserCmd1()
{
}

queue<Object> StringParserCmd1::getObjects()
{
	// NOTE: no argumentStrings and objectString
	if (m_strings.empty())
	{
		return queue<Object>();
	}

	// NOTE: get argumentStrings
	queue<string> argumentStrings;
	while (m_strings.size() != 1)
	{
		// NOTE: get argumentStrings
		argumentStrings.emplace(m_strings.front());

		// NOTE: pop string
		m_strings.pop();
	}

	// NOTE: get object
	queue<Object> objects;

	// NOTE: last one is objectString, others are directory
	string objectString = m_strings.front();

	// NOTE: objectString doesn't use /, normal condition
	if (objectString.find('/') == string::npos)
	{
		Path path = objectString;
		// NOTE: get object from path
		objectString = path.m_pathQueue.back();

		// NOTE: objectString has wildcard
		if (objectString.find('*') != string::npos || objectString.find('?') != string::npos)
		{
			// NOTE: generate a temporary object to get object's directory
			Object tempObj(path, argumentStrings, m_currentDirectory);

			// NOTE: get parent in stack
			auto dir = tempObj.m_currentDirectory;
			dir.pop();
		
			// NOTE: ensure that directory could be found
			if (dir.top() != nullptr)
			{
				ObjectGenerator objectGenerator(objectString, tempObj.m_currentDirectory);
				for (auto o : objectGenerator.getObjects())
				{
					Path _path = path;
					_path.m_pathQueue.back() = o;
					objects.emplace(o, argumentStrings, m_currentDirectory);
				}
			}
			// NOTE: if directory could not be found, objects will be empty
		}
		// NOTE: objectString doesn't have wildcard
		else
		{
			// NOTE: from partition
			if (path.m_pathQueue.front().compare(initPartition) == 0)
			{
				// QUESTION: why I use root directory here?
				stack<File*> dir;
				dir.push(DiskSystem::getInstance()->getRootDirectory());
				// NOTE: if I use init partition here
				// dir.push(DiskSystem::getInstance()->getRootDirectory()->search(initPartition, FileType::dirFile)->second);
				objects.emplace(path, argumentStrings, dir);
			}
			// NOTE: not from partition
			else
			{
				auto pathQueue = path.m_pathQueue;
				while (!pathQueue.empty())
				{
					pathQueue.pop();
				}
				objects.emplace(path, argumentStrings, m_currentDirectory);
			}
		}
	}
	// NOTE: objectString use /, treat arguments as objects
	else
	{
		// NOTE: avoid objectString like: \folder/folder\folder
		while (objectString.find('/') != string::npos)
		{
			objectString[objectString.find('/')] = '\\';
		}
		// NOTE: add it as arguments and dont do with wildcard
		argumentStrings.emplace(objectString);
		// NOTE: treat all argumentStrings as objectString
		queue<string> emptyargumentStrings;
		while (!argumentStrings.empty())
		{
			auto objStr = argumentStrings.front();
			while (objStr.find('/') != string::npos)
			{
				objStr[objStr.find('/')] = '\\';
			}
			// NOTE: return as objects
		
			Path path = objStr;
			// NOTE: here need check if it is from partition
			// NOTE: from parition
			if (path.m_pathQueue.front().compare(initPartition) == 0)
			{
				// QUESTION: why I use root directory here?
				stack<File*> dir;
				// dir.push(DiskSystem::getInstance()->getRootDirectory());
				// NOTE: if I use init partition here, then i should pop path
				dir.push(DiskSystem::getInstance()->getRootDirectory()->search(initPartition, FileType::dirFile)->second);
				path.m_pathQueue.pop();
				objects.emplace(path, argumentStrings, dir);
			}
			// NOTE: not from partition
			else
			{
				auto pathQueue = path.m_pathQueue;
				while (!pathQueue.empty())
				{
					pathQueue.pop();
				}
				objects.emplace(path, argumentStrings, m_currentDirectory);
			}

			argumentStrings.pop();
		}
	}

	return objects;
}
