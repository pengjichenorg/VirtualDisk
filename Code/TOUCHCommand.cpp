#include "stdafx.h"
#include "CommandFactory.h"
#include "ErrorMessage.h"

#include "GeneralFile.h"

Msg TOUCHCommand(queue<Object> objects)
{
	if (objects.empty())
	{
		return Msg(false, errorSyntaxMessage, nullptr);
	}

	vector<Object> touchObjects;

	while (!objects.empty())
	{
		auto object = objects.front();

		if (object.m_file != nullptr)
		{
			return Msg(false, errorExistMessage, nullptr);
		}
		else if (object.m_fileParent == nullptr)
		{
			return Msg(false, errorDirMessage, nullptr);
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
		auto dir = static_cast<DirectoryFile*>(object.m_fileParent);
		dir->addChild(new GeneralFile(name));
	}

	return Msg();
}