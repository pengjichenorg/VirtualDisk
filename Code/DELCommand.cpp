#include "stdafx.h"
#include "CommandFactory.h"
#include "ErrorMessage.h"
#include "DiskSystem.h"

#include <algorithm>
#include <exception>

// NOTE: for del command when target is link, there are some conditions as follow:
// NOTE: | link file | link target |         result         |
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
		// cout << "TEST: objects is empty" << endl;
		s_Argument = false;
		return Msg(false, errorSyntaxMessage, nullptr);
	}

	// NOTE: get arguments
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
			s_Argument = false;
			return Msg(false, errorInvalidSwitch + " - " + "\"" + argument.substr(1) + "\"", nullptr);
		}
	}

	// NOTE: check error path
	auto _objects = objects;
	while (!_objects.empty())
	{
		if (_objects.front().m_file == nullptr)
		{
			s_Argument = false;
			return Msg(false, errorNonFileMessage, nullptr);
		}
		_objects.pop();
	}

	// NOTE: delete
	while (!objects.empty())
	{
		auto objectFile = objects.front().m_file;
		auto objectFileName = objectFile->getName();
		auto objectParent = static_cast<DirectoryFile*>(objects.front().m_fileParent);

		// NOTE: ensure could be found
		if (objectParent->search(objectFileName) != objectParent->getChildren().end())
		{
			// NOTE: delete directory
			if (objectFile->getType() == FileType::directoryFile)
			{
				// cout << "TEST: del driectory will do del dir\\*" << endl;
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
									else if (it->second->getType() == FileType::directoryFile)
									{
										dirQueue.push(static_cast<DirectoryFile*>(it->second));
									}
									files.push_back(it->first);
								}
								for (auto file : files)
								{
									_dir->removeChild(file);
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
								if (it->first.compare(".") == 0 || it->first.compare("..") == 0 || it->second->getType() == FileType::directoryFile || it->second->getType() == FileType::symlinkd)
								{
									continue;
								}
								files.push_back(it->first);
							}
							for (auto file : files)
							{
								static_cast<DirectoryFile*>(objectFile)->removeChild(file);
							}
						}
						result = false;
					}
					else if(s.compare("n") == 0 || s.compare("no") == 0)
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
				// cout << "TEST: del symlinkd will do del dir\\*" << endl;

				// NOTE: get root link
				DirectoryFile* link = static_cast<SymbolDirectoryFile*>(objectFile)->getLinkDirectory();
				try
				{
					// cout << "TEST: link directory file:" << link->getName() << endl;
					while (link->getType() == FileType::symlinkd)
					{
						// cout << "TEST: before get link:" << link->getName() << endl;
						link = static_cast<SymbolDirectoryFile*>(link)->getLinkDirectory();
						// cout << "TEST: after get link:" << link->getName() << endl;
					}
					
					// NOTE: need a code that throws std::bad_alloc exception, WTF!
					link->getName();
				}
				catch (std::bad_alloc)
				{
					s_Argument = false;
					return Msg(false, errorDirMessage, nullptr);
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
							if (it->first.compare(".") == 0 || it->first.compare("..") == 0 || it->second->getType() == FileType::directoryFile || it->second->getType() == FileType::symlinkd)
							{
								continue;
							}
							files.push_back(it->first);
						}
						for (auto file : files)
						{
							link->removeChild(file);
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
				// cout << "TEST: del non-directory file" << endl;
				if (s_Argument)
				{
					// cout << "TEST: s_argument is true" << endl;
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
							else if (it->second->getType() == FileType::directoryFile)
							{
								dirQueue.push(static_cast<DirectoryFile*>(it->second));
							}
						}
						_dir->removeChild(objectFileName);
						dirQueue.pop();
					}
				}
				else
				{
					objectParent->removeChild(objectFileName);
				}			
			}
		}
		objects.pop();
	}

	s_Argument = false;
	return Msg();
}