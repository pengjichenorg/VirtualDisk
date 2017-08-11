#include "stdafx.h"
#include "StringParserCmd2.h"
#include "DiskSystem.h"
#include "ObjectGenerator.h"

// NOTE: in cmd2, string which has '/' is argumentString, others are objectString
// NOTE: if there is no objectString, then treat all arguments as path
// NOTE: there is a special condition, that is, 'cmd /arg dir/dir', when it comes, treat all arguments as path
// NOTE: on the other hand, '/' must at first position in each string, otherwise, string which has '/' is path

StringParserCmd2::StringParserCmd2(const queue<string>& strings, stack<File*> currentDirectory) : StringParserCmd(strings, currentDirectory)
{
}

StringParserCmd2::~StringParserCmd2()
{
}

queue<Object> StringParserCmd2::getObjects()
{
	// NOTE: no arguments and objects
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

	// NOTE: get strings
	while (!m_strings.empty())
	{
		// NOTE: get a string
		auto str = m_strings.front();

		// NOTE: get arguments
		if (str.find('/') != string::npos)
		{
			argumentStrings.push(str);
			allArgumentStrings.emplace(str);
		}
		// NOTE: get object
		else
		{
			// NOTE: split str
			Path path = str;
			// NOTE: get object string
			auto objectString = path.m_pathQueue.back();

			// NOTE: objectString has wildcard
			if (objectString.find('*') != string::npos || objectString.find('?') != string::npos)
			{
				Object tempObj(path, argumentStrings, m_currentDirectory);

				// NOTE: get parent in stack
				auto parent = tempObj.m_currentDirectory;
				parent.pop();

				// NOTE: ensure that directory could be found
				if (parent.top() != nullptr)
				{
					ObjectGenerator objectGenerator(objectString, parent);
					for (auto o : objectGenerator.getObjects())
					{
						Path _path = path;
						_path.m_pathQueue.back() = o;
						objects.emplace(o, argumentStrings, parent);
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
					objects.emplace(path, argumentStrings, m_currentDirectory);
				}
			}
			objectStrings.emplace(str);
			// NOTE: clear arguments for next object
			while (!argumentStrings.empty())
			{
				argumentStrings.pop();
			}
		}
		m_strings.pop();
	}

	// NOTE: for command have arguments without objects like "copy /f1 /f2", treat it as directory
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
						objects.emplace(_path, argumentStrings, parent);
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
