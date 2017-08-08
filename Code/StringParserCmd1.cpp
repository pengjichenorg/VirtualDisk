#include "stdafx.h"
#include "StringParserCmd1.h"
#include "ObjectGenerator.h"
#include "DiskSystem.h"
#include "VDos.h"

StringParserCmd1::StringParserCmd1(const queue<string>& strings, DirectoryFile* currentDirectory) : StringParserCmd(strings, currentDirectory)
{
}

StringParserCmd1::~StringParserCmd1()
{
}

queue<Object> StringParserCmd1::getObjects()
{
	// cout << "TEST: return queue with 1 object" << endl;

	// NOTE: no arguments and objects
	if (m_strings.empty())
	{
		return queue<Object>();
	}

	// cout << "TEST: get arguments:";
	// NOTE: split test and code
	vector<string> arguments;
	while (m_strings.size() > 1)
	{
		// cout << m_strings.front() << " ";
		// NOTE: get arguments
		arguments.emplace_back(m_strings.front());

		// NOTE: pop string
		m_strings.pop();
	 }
	// cout << endl;

	// cout << "TEST: get object:" << m_strings.front() << endl;
	// cout << "TEST: currentDirectory:" << m_currentDirectory->getName() << endl;

	// NOTE: get object
	queue<Object> objects;

	// NOTE: last one is object, others are directory
	// NOTE: change '/' to '\'
	string pathStr = m_strings.front();
	
	while (pathStr.find('/') != string::npos)
	{
		pathStr[pathStr.find('/')] = '\\';
	}
	// cout << "TEST: after change '/' to  '\\'" << pathStr << endl;

	Path path = pathStr;
	auto objString = path.m_pathQueue.back();
	// NOTE: only if object has wildcard
	if (objString.find('*') != string::npos || objString.find('?') != string::npos)
	{
		// NOTE: generate a temporary object to get object's directory
		Object obj(path, arguments, m_currentDirectory);
		
		// NOTE: test
		// cout << "TEST: wildcard object parent directory:" << (obj.m_fileParent != nullptr? obj.m_fileParent->getName() : "nullptr") << endl;
		
		// NOTE: ensure that directory could be found
		if (obj.m_fileParent != nullptr)
		{
			ObjectGenerator objectGenerator(objString, static_cast<DirectoryFile*>(obj.m_fileParent));
		}
		// NOTE: if directory could not be found, objects will be empty
	}
	// NOTE: object doesnt have wildcard
	else
	{
		// NOTE: from partition and init VDos::path
		if (path.m_pathQueue.front().compare(initPartition) == 0)
		{
			// cout << "TEST: from partition" << endl;
			while (VDos::_path.size() != 1)
			{
				VDos::_path.pop();
			}
			objects.emplace(path, arguments, DiskSystem::getInstance()->getRootDirectory());
		}
		// NOTE: not from partition
		else
		{
			// cout << "TEST: isn't from partition" << endl;
			objects.emplace(path, arguments, m_currentDirectory);
		}
	}

	return objects;
}
