#include "stdafx.h"
#include "CommandFactory.h"
#include "ErrorMessage.h"
#include "DiskSystem.h"

#include "File.h"
#include "BinaryFile.h"
#include "DirectoryFile.h"
#include "SymlinkFile.h"
#include "SymlinkdFile.h"

#include <algorithm>
#include <exception>
#include <cassert>

// NOTE: for del command when target is link, there are some conditions as follow:
// NOTE: | link file | link target |         result         |
// NOTE: |-----------+-------------+------------------------|
// NOTE: |    SGF    |    exist    |        del SGF         |
// NOTE: |    SGF    |  non-exist  |        del SGF         |
// NOTE: |    SDF    |    exist    |        del link\*      |
// NOTE: |    SDF    |  non-exist  |     errorDirMessage    |

const vector<string> argumentVector = { "/s" };

static bool s_Argument = false;

Msg DELCommand(queue<Object> objects)
{
	if (objects.empty())
	{
		s_Argument = false;
		return Msg(false, errorSyntaxMessage, stack<File*>());
	}

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

	// NOTE: check error path
	auto _objects = objects;
	while (!_objects.empty())
	{
		if (_objects.front().m_currentDirectory.top() == nullptr)
		{
			s_Argument = false;
			return Msg(false, errorNonFileMessage, _objects.front().m_currentDirectory);
		}
		_objects.pop();
	}

	// NOTE: delete
	while (!objects.empty())
	{
		auto objectFile = objects.front().m_currentDirectory.top();
		auto objectFileName = objectFile->getName();
		auto currentDirectoryCopy = objects.front().m_currentDirectory;
		// NOTE: for only C: in currentDirectory
		if (currentDirectoryCopy.size() != 1)
		{
			currentDirectoryCopy.pop();
		}
		auto objectParent = static_cast<DirectoryFile*>(currentDirectoryCopy.top());

		// NOTE: ensure could be found
		if (objectParent->search(objectFileName, objectFile->getType()) != objectParent->getChildren().end())
		{
			// NOTE: delete directory
			if (objectFile->getType() == FileType::dirFile)
			{
				string path;
				auto dir = objectFile;
				while (dir != DiskSystem::getInstance()->getRootDirectory())
				{
					if (path.empty())
					{
						path = dir->getName() + "\\";
					}
					else
					{
						if (path.back() == '\\')
						{
							path = path.substr(0, path.size() - 1);
						}
						path = dir->getName() + "\\" + path;
					}
					dir = dir->getParent();
				}
				path = path + "\\*, are you sure(Y/N)?";
				bool result = true;
				cout << path;
				while (result)
				{
					std::string s;
					std::cin.sync(); // clear buffer
					std::getline(std::cin, s);

					transform(s.begin(), s.end(), s.begin(), tolower);
					if (s.compare("y") == 0 || s.compare("yes") == 0)
					{
						vector<string> files;
						queue<DirectoryFile*> dirQueue;
						// NOTE: del /s dir\*
						if (s_Argument)
						{
							dirQueue.push(static_cast<DirectoryFile*>(objectFile));
							while (!dirQueue.empty())
							{
								auto _dir = dirQueue.front();
								for (auto it = _dir->getChildren().begin(); it != _dir->getChildren().end(); it++)
								{
									if (it->first.compare(".") == 0 || it->first.compare("..") == 0 || it->second->getType() == FileType::symlinkd)
									{
										continue;
									}
									else if (it->second->getType() == FileType::dirFile)
									{
										dirQueue.push(static_cast<DirectoryFile*>(it->second));
									}
									files.push_back(it->first);
								}
								for (auto file : files)
								{
									// NOTE: delete general file
									_dir->removeChild(file, FileType::binFile);
									// NOTE: delete symbol general file
									_dir->removeChild(file, FileType::symlink);
								}
								files.clear();
								dirQueue.pop();
							}
						}
						// NOTE: del dir\*
						else
						{
							for (auto it = static_cast<DirectoryFile*>(objectFile)->getChildren().begin();
							it != static_cast<DirectoryFile*>(objectFile)->getChildren().end(); it++)
							{
								if (it->first.compare(".") == 0 || it->first.compare("..") == 0 || it->second->getType() == FileType::binFile || it->second->getType() == FileType::symlinkd)
								{
									continue;
								}
								files.push_back(it->first);
							}
							for (auto file : files)
							{
								// NOTE: delete binary file
								static_cast<DirectoryFile*>(objectFile)->removeChild(file, FileType::binFile);
								// NOTE: delete symbol general file
								static_cast<DirectoryFile*>(objectFile)->removeChild(file, FileType::symlink);
							}
						}
						result = false;
					}
					else if (s.compare("n") == 0 || s.compare("no") == 0)
					{
						result = false;
					}
					else
					{
						cout << path;
					}
				}
			}
			// NOTE: delete symlinkd
			else if (objectFile->getType() == FileType::symlinkd)
			{
				DirectoryFile* link = nullptr;
				try
				{
					// NOTE: get root link
					link = static_cast<SymlinkdFile*>(objectFile)->getLinkDirectory();
					while (link->getType() == FileType::symlinkd)
					{
						link = static_cast<SymlinkdFile*>(link)->getLinkDirectory();
					}

					// NOTE: need a code that throws std::bad_alloc exception, WTF!
					link->getName();
				}
				catch (std::bad_alloc)
				{
					s_Argument = false;
					return Msg(false, errorDirMessage, stack<File*>());
				}
				catch (...)
				{
					s_Argument = false;
					return Msg(false, errorDirMessage, stack<File*>());
				}

				string path;
				auto dir = objectFile;
				while (dir != DiskSystem::getInstance()->getRootDirectory())
				{
					if (path.empty())
					{
						path = dir->getName() + "\\";
					}
					else
					{
						if (path.back() == '\\')
						{
							path = path.substr(0, path.size() - 1);
						}
						path = dir->getName() + "\\" + path;
					}
					dir = dir->getParent();
				}
				path = path + "\\*, are you sure(Y/N)?";
				bool result = true;
				cout << path;
				while (result)
				{
					std::string s;
					std::cin.sync(); // clear buffer
					std::getline(std::cin, s);

					transform(s.begin(), s.end(), s.begin(), tolower);
					if (s.compare("y") == 0 || s.compare("yes") == 0)
					{
						vector<string> files;
						for (auto it = link->getChildren().begin(); it != link->getChildren().end(); it++)
						{
							if (it->first.compare(".") == 0 || it->first.compare("..") == 0 || it->second->getType() == FileType::dirFile || it->second->getType() == FileType::symlinkd)
							{
								continue;
							}
							files.push_back(it->first);
						}
						for (auto file : files)
						{
							// NOTE: delete general file
							link->removeChild(file, FileType::binFile);
							// NOTE: delete symbol general file
							link->removeChild(file, FileType::symlink);
						}
						result = false;
					}
					else if (s.compare("n") == 0 || s.compare("no") == 0)
					{
						result = false;
					}
					else
					{
						cout << path;
					}
				}
			}
			else
			{
				if (s_Argument)
				{
					queue<DirectoryFile*> dirQueue;
					dirQueue.push(objectParent);
					while (!dirQueue.empty())
					{
						auto _dir = dirQueue.front();
						for (auto it = _dir->getChildren().begin(); it != _dir->getChildren().end(); it++)
						{
							if (it->first.compare(".") == 0 || it->first.compare("..") == 0 || it->second->getType() == FileType::symlinkd)
							{
								continue;
							}
							else if (it->second->getType() == FileType::dirFile)
							{
								dirQueue.push(static_cast<DirectoryFile*>(it->second));
							}
						}
						// NOTE: delete general file
						_dir->removeChild(objectFileName, FileType::binFile);
						// NOTE: delete symbol general file
						_dir->removeChild(objectFileName, FileType::symlink);
						dirQueue.pop();
					}
				}
				else
				{
					// NOTE: delete general file
					objectParent->removeChild(objectFileName, FileType::binFile);
					// NOTE: delete symbol general file
					objectParent->removeChild(objectFileName, FileType::symlink);
				}
			}
		}
		objects.pop();
	}

	s_Argument = false;
	return Msg();
}