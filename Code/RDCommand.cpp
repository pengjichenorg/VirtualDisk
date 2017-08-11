#include "stdafx.h"
#include "CommandFactory.h"
#include "ErrorMessage.h"

#include "DirectoryFile.h"
#include "SymlinkdFile.h"   

#include <exception>
#include <cassert>

const vector<string> argumentVector = { "/s" };

static bool s_Argument = false;

Msg RDCommand(queue<Object> objects)
{
	// NOTE: initialize argument switch
	s_Argument = false;

	if (objects.empty())
	{
		return Msg(false, errorSyntaxMessage, stack<File*>());
	}

	while (!objects.empty())
	{
		auto object = objects.front();

		// NOTE: get arguments
		for (auto args = objects.front().m_arguments; !args.empty(); args.pop())
		{
			if (find(argumentVector.begin(), argumentVector.end(), args.front()) != argumentVector.end())
			{
				if (args.front().find("/s") != string::npos)
				{
					s_Argument = true;
				}
			}
			else
			{
				s_Argument = false;
				return Msg(false, errorInvalidSwitch + " - " + "\"" + args.front().substr(1) + "\"", stack<File*>());
			}
		}

		// NOTE: remove directory
		if (object.m_currentDirectory.top() == nullptr)
		{
			// NOTE: pay attention, this is to avoid object which is nullptr
			// NOTE: in rd command, object which is nullptr is from two condition:
			// NOTE: condition1, rd object is a general file or symbol general file
			// NOTE: condition2, rd object doesn't exist
			if (objects.size() > 1)
			{
				objects.pop();
				continue;
			}
			return Msg(false, errorNonFileMessage, stack<File*>());
		}
		else
		{
			// NOTE: for directory link file which its link is not exist
			try
			{
				auto objectName = object.m_currentDirectory.top()->getName();
				
				if (object.m_currentDirectory.top()->getType() == FileType::dirFile)
				{
					if (static_cast<DirectoryFile*>(object.m_currentDirectory.top())->getChildren().size() > 2)
					{
						if (s_Argument)
						{
							auto currentDirectoryCopy = object.m_currentDirectory;
							// NOTE: for only C: in currentDirectory
							if (currentDirectoryCopy.size() != 1)
							{
								currentDirectoryCopy.pop();
							}
							static_cast<DirectoryFile*>(currentDirectoryCopy.top())->removeChild(objectName, FileType::dirFile);
						}
						else
						{
							return Msg(false, errorNonEmptyMessage, stack<File*>());
						}
					}
					else
					{
						if (s_Argument)
						{
							queue<DirectoryFile*> dirQueue;
							auto currentDirectoryCopy = object.m_currentDirectory;
							// NOTE: for only C: in currentDirectory
							if (currentDirectoryCopy.size() != 1)
							{
								currentDirectoryCopy.pop();
							}
							dirQueue.push(static_cast<DirectoryFile*>(currentDirectoryCopy.top()));
							while (!dirQueue.empty())
							{
								auto _dir = dirQueue.front();
								for (auto it = _dir->getChildren().begin(); it != _dir->getChildren().end(); it++)
								{
									if (it->first.compare(".") == 0 || it->first.compare("..") == 0)
									{
										continue;
									}
									else if (it->second->getType() == FileType::dirFile && it->first.compare(objectName) != 0)
									{
										dirQueue.push(static_cast<DirectoryFile*>(it->second));
									}
								}
								_dir->removeChild(objectName, FileType::dirFile);
								dirQueue.pop();
							}
						}
						else
						{
							auto currentDirectoryCopy = object.m_currentDirectory;
							// NOTE: for only C: in currentDirectory
							if (currentDirectoryCopy.size() != 1)
							{
								currentDirectoryCopy.pop();
							}
							static_cast<DirectoryFile*>(currentDirectoryCopy.top())->removeChild(objectName, FileType::dirFile);
						}
					}
				}
				else if (object.m_currentDirectory.top()->getType() == FileType::symlinkd)
				{
					auto currentDirectoryCopy = object.m_currentDirectory;
					// NOTE: for only C: in currentDirectory
					if (currentDirectoryCopy.size() != 1)
					{
						currentDirectoryCopy.pop();
					}
					auto objectParent = currentDirectoryCopy.top();
					try
					{
						auto child = object.m_currentDirectory.top();
						// NOTE: child may be nullptr
						if (child == nullptr)
						{
							throw child;
						}
						// NOTE: child may be can't access
						else
						{
							child->getName();
						}

						auto childName = child->getName();
						auto childType = child->getType();

						auto parent = objectParent;

						// NOTE: parent may be nullptr
						if (parent == nullptr)
						{
							throw parent;
						}
						// NOTE: parent may be can't access(its resource has been released)
						else
						{
							parent->getName();
						}

						static_cast<DirectoryFile*>(objectParent)->removeChild(childName, childType);
					}
					catch (std::bad_alloc)
					{
						return Msg(false, errorDirMessage, stack<File*>());
					}
					catch (...)
					{
						return Msg(false, errorDirMessage, stack<File*>());
					}
				}
				else
				{
					// NOTE: rd general file and symbol general file shoule be here
					continue;
				}
			}
			// NOTE: direcotry link file's link is not exist
			catch (std::bad_alloc)
			{
				// NOTE: for rd directory link which its link is not exist
				auto currentDirectoryCopy = object.m_currentDirectory;
				// NOTE: for only C: in currentDirectory
				if (currentDirectoryCopy.size() != 1)
				{
					currentDirectoryCopy.pop();
				}
				if (currentDirectoryCopy.top() != nullptr)
				{
					auto linkName = object.m_path.m_pathQueue.back();
					static_cast<DirectoryFile*>(currentDirectoryCopy.top())->removeChild(linkName, FileType::symlinkd);
					return Msg();
				}
				else
				{
					return Msg(false, errorDirMessage, stack<File*>());
				}
			}
			catch (...)
			{
				return Msg(false, errorDirMessage, stack<File*>());
			}
		}
		objects.pop();
	}
	return Msg();
}