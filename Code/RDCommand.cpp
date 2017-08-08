#include "stdafx.h"
#include "CommandFactory.h"
#include "ErrorMessage.h"

#include <exception>

#include "DirectoryFile.h"
#include "SymbolDirectoryFile.h"

const vector<string> argumentVector = { "/s" };

static bool s_Argument = false;

Msg RDCommand(queue<Object> objects)
{
	// NOTE: initialize argument switch
	s_Argument = false;

	if (objects.empty())
	{
		// cout << "TEST: objects is empty" << endl;
		return Msg(false, errorSyntaxMessage, nullptr);
	}

	while (!objects.empty())
	{
		auto object = objects.front();

		// NOTE: argument switch
		// cout << "TEST: arguments:";
		for (auto argument : objects.front().m_arguments)
		{
			if (find(argumentVector.begin(), argumentVector.end(), argument) != argumentVector.end())
			{
				if (argument.find("/s") != string::npos)
				{
					s_Argument = true;
				}
			}
			else
			{
				return Msg(false, errorInvalidSwitch + " - " + "\"" + argument.substr(1) + "\"", nullptr);
			}
		}
		// cout << endl;

		// NOTE: remove directory
		if (object.m_file == nullptr)
		{
			return Msg(false, errorNonFileMessage, nullptr);
		}
		else
		{
			auto objectName = object.m_file->getName();
			if (object.m_file->getType() == FileType::directoryFile)
			{
				// cout << "TEST: remove directory file" << endl;
				if (static_cast<DirectoryFile*>(object.m_file)->getChildren().size() > 2)
				{
					if (s_Argument)
					{
						static_cast<DirectoryFile*>(object.m_fileParent)->removeChild(objectName);
						return Msg();
					}
					else
					{
						return Msg(false, errorNonEmptyMessage, nullptr);
					}
				}
				else
				{
					if (s_Argument)
					{
						queue<DirectoryFile*> dirQueue;
						dirQueue.push(static_cast<DirectoryFile*>(object.m_fileParent));
						while(!dirQueue.empty())
						{
							auto _dir = dirQueue.front();
							for (auto it = _dir->getChildren().begin(); it != _dir->getChildren().end(); it++)
							{
								if (it->first.compare(".") == 0 || it->first.compare("..") == 0)
								{
									continue;
								}
								else if (it->second->getType() == FileType::directoryFile && it->first.compare(objectName) != 0)
								{
									dirQueue.push(static_cast<DirectoryFile*>(it->second));
								}
							}
							_dir->removeChild(objectName);
							dirQueue.pop();
						}
					}
					else
					{
						static_cast<DirectoryFile*>(object.m_fileParent)->removeChild(objectName);
					}
				}
			}
			else if (object.m_file->getType() == FileType::symlinkd)
			{
				// cout << "TEST: remove symlink directory file" << endl;
				auto objectParent = object.m_fileParent;
				static_cast<DirectoryFile*>(objectParent)->removeChild(object.m_file->getName());
			}
			else
			{
				continue;
			}
		}
		objects.pop();
	}
	return Msg();
}