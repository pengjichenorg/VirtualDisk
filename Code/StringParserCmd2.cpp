#include "stdafx.h"
#include "StringParserCmd2.h"
#include "DiskSystem.h"

StringParserCmd2::StringParserCmd2(const queue<string>& strings, DirectoryFile* currentDirectory) : StringParserCmd(strings, currentDirectory)
{
}

StringParserCmd2::~StringParserCmd2()
{
}

queue<Object> StringParserCmd2::getObjects()
{
	// cout << "TEST: return queue with 2 objects" << endl;
	queue<Object> objects;

	// NOTE: no arguments and objects
	if (m_strings.empty())
	{
		return queue<Object>();
	}

	// NOTE: get all objectStr
	vector<string> objectStrings;
	vector<string> arguments;

	// NOTE: get strings
	while (!m_strings.empty())
	{
		auto objectStr = m_strings.front();

		// NOTE: get arguments
		if (objectStr.find('/') != string::npos)
		{
			arguments.push_back(objectStr);
		}
		// NOTE: get object
		else
		{
			Path path = objectStr;
			auto objString = path.m_pathQueue.back();
			// NOTE: only if object has wildcard
			if (objString.find('*') != string::npos || objString.find('?') != string::npos)
			{
				// NOTE: generate a temporary object to get object's directory
				Object obj(path, arguments, m_currentDirectory);

				// cout << "TEST: wildcard object parent directory:" << (obj.m_fileParent != nullptr ? obj.m_fileParent->getName() : "nullptr") << endl;

				// NOTE: ensure that directory could be found
				if (obj.m_fileParent != nullptr)
				{
					ObjectGenerator objectGenerator(objString, static_cast<DirectoryFile*>(obj.m_fileParent));
					for (auto s : objectGenerator.getObjects())
					{
						Path _path = path;
						_path.m_pathQueue.back() = s;
						objects.emplace(_path, arguments, m_currentDirectory);
					}
					objectStrings.push_back(objectStr);
					arguments.clear();
				}
				// NOTE: if directory could not be found, objects will be empty
			}
			// NOTE: object doesnt have wildcard
			else
			{
				// NOTE: from partition
				if (path.m_pathQueue.front().compare(initPartition) == 0)
				{
					// cout << "TEST: from partition" << endl;
					objects.emplace(path, arguments, DiskSystem::getInstance()->getRootDirectory());
				}
				// NOTE: not from partition
				else
				{
					// cout << "TEST: isn't from partition" << endl;
					objects.emplace(path, arguments, m_currentDirectory);
				}
				objectStrings.push_back(objectStr);

				// NOTE: different arguments to different objects
				arguments.clear();
			}
		}
		m_strings.pop();
	}

	// NOTE: for command have arguments without objects like "copy /f1 /f2", treat it as directory
	if (!arguments.empty() && objectStrings.empty())
	{
		// NOTE: change '/' to '\'
		for (auto& s : arguments)
		{
			while (s.find('/') != string::npos)
			{
				s[s.find('/')] = '\\';
			}
			// cout << "TEST: s = " << s << endl;
		}
		// NOTE: deal each one
		for (auto s : arguments)
		{
			// cout << "TEST: s = " << s << endl;

			vector<string> emptyArguments;

			Path path = s;
			auto objString = path.m_pathQueue.back();
			// NOTE: only if object has wildcard
			if (objString.find('*') != string::npos || objString.find('?') != string::npos)
			{
				// NOTE: generate a temporary object to get object's directory
				Object obj(path, emptyArguments, m_currentDirectory);

				// cout << "TEST: wildcard object parent directory:" << (obj.m_fileParent != nullptr ? obj.m_fileParent->getName() : "nullptr") << endl;

				// NOTE: ensure that directory could be found
				if (obj.m_fileParent != nullptr)
				{
					ObjectGenerator objectGenerator(objString, static_cast<DirectoryFile*>(obj.m_fileParent));
					for (auto str : objectGenerator.getObjects())
					{
						Path _path = path;
						_path.m_pathQueue.back() = str;
						objects.emplace(_path, emptyArguments, m_currentDirectory);
					}
					objectStrings.push_back(s);
					emptyArguments.clear();
				}
				// NOTE: if directory could not be found, objects will be empty
			}
			// NOTE: object doesnt have wildcard
			else
			{
				// NOTE: from partition
				if (path.m_pathQueue.front().compare(initPartition) == 0)
				{
					// cout << "TEST: from partition" << endl;
					objects.emplace(path, arguments, DiskSystem::getInstance()->getRootDirectory());
				}
				// NOTE: not from partition
				else
				{
					// cout << "TEST: isn't from partition" << endl;
					objects.emplace(path, arguments, m_currentDirectory);
				}
				// NOTE: meaning?
				objectStrings.push_back(s);

				// NOTE: different arguments to different objects
				emptyArguments.clear();
			}
		}
	}

	return objects;
}
