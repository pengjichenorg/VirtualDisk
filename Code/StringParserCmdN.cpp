#include "stdafx.h"
#include "StringParserCmdN.h"
#include "DiskSystem.h"
#include "ObjectGenerator.h"


// NOTE: in cmdN, string which has '/' is argumentString, others are objectString
// NOTE: if there is no objectString, then treat all arguments as path
// NOTE: there is a special condition, that is, 'cmd /arg dir/dir', when it comes, treat all arguments as path
// NOTE: on the other hand, '/' must at first position in each string, otherwise, string which has '/' is path

// NOTE: test
// NOTE: dir
// NOTE: dir .
// NOTE: dir /e1
// NOTE: dir /e1 /e1
// NOTE: dir /ad e1
// NOTE: dir \e1
// NOTE: dir /ad \e1\e11

StringParserCmdN::StringParserCmdN(const queue<string>& strings, stack<File*> currentDirectory) : StringParserCmd(strings, currentDirectory)
{
}

StringParserCmdN::~StringParserCmdN()
{
}

queue<Object> StringParserCmdN::getObjects()
{
	// NOTE: no argumentString and objectString
	if (m_strings.empty())
	{
		return queue<Object>();
	}

	// NOTE: final objects
	queue<Object> objects;
	// NOTE: if objectStrings is empty, then treat /arg as object 
	queue<string> objectStrings;
	// NOTE: save all arguments, use in special condition
	queue<string> allArgumentStrings;
	// NOTE: arguments for an object
	queue<string> argumentStrings;

	while (!m_strings.empty())
	{
		auto objectString = m_strings.front();

		// NOTE: get argument
		if (objectString.find('/') == 0)
		{
			argumentStrings.emplace(objectString);
			allArgumentStrings.emplace(objectString);
		}
		// NOTE: get object
		else
		{
			Path path = objectString;
			// NOTE: get object from path
			auto objStr = path.m_pathQueue.back();

			// NOTE: only if object has wildcard
			if (objStr.find('*') != string::npos || objStr.find('?') != string::npos)
			{
				// NOTE: generate a temporary object to get object's directory
				Object tempObj(path, argumentStrings, m_currentDirectory);

				// NOTE: get parent in stack
				auto parent = tempObj.m_currentDirectory;
				// NOTE: invalid name, parent will be empty
				if (parent.empty())
				{
					objects.emplace(path, argumentStrings, parent);
				}
				// NOTE: non-invalid name
				else
				{
					parent.pop();
	
					// NOTE: ensure that directory could be found
					if (parent.top() != nullptr)
					{
						ObjectGenerator objectGenerator(objectString, parent);
						for (auto o : objectGenerator.getObjects())
						{
							Path _path = path;
							_path.m_pathQueue.back() = o;
							objects.emplace(_path, argumentStrings, parent);
						}
					}
					// NOTE: if directory could not be found, parent.top will be nullptr
				}
			}
			// NOTE: objectString doesn't have wildcard
			else
			{
				// NOTE: from partition
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
					objects.emplace(path, argumentStrings, m_currentDirectory);
				}
			}
			// NOTE: save objectStrings for special condition
			objectStrings.emplace(objectString);
			// NOTE: clear arguments for next object
			while (!argumentStrings.empty())
			{
				argumentStrings.pop();
			}
		}
		m_strings.pop();
	}

	// NOTE: special condition
	if (!allArgumentStrings.empty() && objectStrings.empty())
	{
		// NOTE: change '/' to '\'
		while (!allArgumentStrings.empty())
		{
			while (allArgumentStrings.front().find('/') != string::npos)
			{
				allArgumentStrings.front()[allArgumentStrings.front().find('/')] = '\\';
			}
			objectStrings.emplace(allArgumentStrings.front());
			allArgumentStrings.pop();
		}

		// NOTE: each object
		while (!objectStrings.empty())
		{
			auto objectString = objectStrings.front();

			// NOTE: get object
			Path path = objectString;
			// NOTE: get object from path
			auto objStr = path.m_pathQueue.back();

			// NOTE: only if object has wildcard
			if (objStr.find('*') != string::npos || objStr.find('?') != string::npos)
			{
				// NOTE: generate a temporary object to get object's directory
				Object tempObj(path, argumentStrings, m_currentDirectory);

				// NOTE: get parent in stack
				auto parent = tempObj.m_currentDirectory;
				parent.pop();
		
				// NOTE: ensure that directory could be found
				if (parent.top() != nullptr)
				{
					ObjectGenerator objectGenerator(objectString, tempObj.m_currentDirectory);
					for (auto o : objectGenerator.getObjects())
					{
						Path _path = path;
						_path.m_pathQueue.back() = o;
						objects.emplace(_path, argumentStrings, m_currentDirectory);
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
					// dir.push(DiskSystem::getInstance()->getRootDirectory());
					// NOTE: if I use init partition here, then i should pop path
					dir.push(DiskSystem::getInstance()->getRootDirectory()->search(initPartition, FileType::dirFile)->second);
					path.m_pathQueue.pop();
					objects.emplace(path, argumentStrings, dir);
				}
				// NOTE: not from partition
				else
				{
					objects.emplace(path, argumentStrings, m_currentDirectory);
				}
			}
			objectStrings.pop();
		}
	}

	return objects;
}
