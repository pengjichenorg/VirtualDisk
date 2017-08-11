#include "stdafx.h"
#include "CommandFactory.h"
#include "ErrorMessage.h"

#include "BinaryFile.h"

#include <cassert>

Msg TOUCHCommand(queue<Object> objects)
{
	if (objects.empty())
	{
		return Msg(false, errorSyntaxMessage, stack<File*>());
	}

	vector<Object> touchObjects;

	while (!objects.empty())
	{
		auto object = objects.front();
		auto currentDirectoryCopy = object.m_currentDirectory;
		// NOTE: for only C: in currentDirectory
		if (currentDirectoryCopy.size() != 1)
		{
			currentDirectoryCopy.pop();
		}

		if (object.m_currentDirectory.top() != nullptr)
		{
			return Msg(false, errorExistMessage, stack<File*>());
		}
		else if (currentDirectoryCopy.top() == nullptr)
		{
			return Msg(false, errorDirMessage, stack<File*>());
		}
		else
		{
			touchObjects.push_back(object);
		}
		objects.pop();
	}

	for (auto object : touchObjects)
	{
		auto name = object.m_path.m_pathQueue.back();
		auto currentDirectoryCopy = object.m_currentDirectory;
		// NOTE: for only C: in currentDirectory
		if (currentDirectoryCopy.size() != 1)
		{
			currentDirectoryCopy.pop();
		}
		auto dir = static_cast<DirectoryFile*>(currentDirectoryCopy.top());
		dir->addChild(new BinaryFile(name));
	}

	return Msg();
}