#include "stdafx.h"
#include "CommandFactory.h"
#include "ErrorMessage.h"

#include "Msg.h"
#include "Object.h"

#include "BinaryFile.h"
#include "DirectoryFile.h"
#include "SymlinkFile.h"
#include "SymlinkdFile.h"

#include "DiskSystem.h"

#include <vector>
#include <exception>
#include <cassert>

using namespace std;

bool DIRCommandForEach(Object file);
void DIRBinaryFile(BinaryFile* file);
void DIRDirectory(DirectoryFile* file);
void DIRSymlinkFile(SymlinkFile* file);
void DIRDirectoryFile(DirectoryFile* file);
void DIRSymblinkdFile(SymlinkdFile* file);

const vector<string> argumentVector = { "/s", "/ad" };

static vector<string> infos;

static bool s_Argument = false;
static bool ad_Argument = false;

Msg DIRCommand(queue<Object> objects)
{
	// NOTE: initialize argument switch
	s_Argument = false;
	ad_Argument = false;

	if (objects.size() == 1)
	{
		// NOTE: get arguments
		for (auto args = objects.front().m_arguments; !args.empty(); args.pop())
		{
			if (find(argumentVector.begin(), argumentVector.end(), args.front()) != argumentVector.end())
			{
				if (args.front().find("/s") != string::npos)
				{
					s_Argument = true;
				}
				if (args.front().find("/ad") != string::npos)
				{
					ad_Argument = true;
				}
			}
			else
			{
				s_Argument = false;
				ad_Argument = false;
				return Msg(false, errorInvalidSwitch + " - " + "\"" + args.front().substr(1) + "\"", stack<File*>());
			}
		}

		// NOTE: clear info
		infos.clear();

		if (objects.front().m_currentDirectory.top() == nullptr)
		{
			infos.clear();
			return Msg(false, errorNonFileMessage, stack<File*>());
		}
		else
		{
			if (ad_Argument)
			{
				if (objects.front().m_currentDirectory.top()->getType() != FileType::dirFile)
				{
					return Msg();
				}
			}
			if (!DIRCommandForEach(objects.front()))
			{
				return Msg(false, errorDirMessage, stack<File*>());
			}
		}

		// NOTE: print directory
		auto _file = objects.front().m_currentDirectory.top();

		// NOTE: chck if it could be access
		try
		{
			_file->getName();
		}
		catch (std::bad_alloc)
		{
			return Msg(false, errorNonFileMessage, stack<File*>());
		}

		string path;
		while (_file != DiskSystem::getInstance()->getRootDirectory())
		{
			if (path.empty())
			{
				path = _file->getName() + "\\";
			}
			else
			{
				if (path.back() == '\\')
				{
					path = path.substr(0, path.size() - 1);
				}
				path = _file->getName() + "\\" + path;
			}
			_file = _file->getParent();
		}
		cout << "\n Directory of " << path << "\n" << endl;
		
		// NOTE: print info
		for (auto info : infos)
		{
			cout << info << endl;
		}

		// NOTE: clear info
		infos.clear();
	}

	// NOTE: more than one object
	if (objects.size() > 1)
	{
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
					if (args.front().find("/ad") != string::npos)
					{
						ad_Argument = true;
					}
				}
				else
				{
					s_Argument = false;
					ad_Argument = false;
					return Msg(false, errorInvalidSwitch + " - " + "\"" + args.front().substr(1) + "\"", stack<File*>());
				}
			}

			if (object.m_currentDirectory.top() == nullptr)
			{
				infos.clear();
				return Msg(false, errorNonFileMessage, stack<File*>());
			}
			else
			{
				// NOTE: ensure every object is not nullptr
				if (ad_Argument)
				{
					if (object.m_currentDirectory.top()->getType() != FileType::dirFile)
					{
						continue;
					}
				}
				if (!DIRCommandForEach(object))
				{
					return Msg(false, errorDirMessage, stack<File*>());
				}
			}
			objects.pop();

			// NOTE: print info
			for (auto info : infos)
			{
				cout << info << endl;
			}

			// NOTE: clear info
			infos.clear();
		}
	}
	return Msg();
}

bool DIRCommandForEach(Object object)
{
	auto file = object.m_currentDirectory.top();
	switch (file->getType())
	{
	case FileType::binFile:
	{
		DIRBinaryFile(static_cast<BinaryFile*>(file));
		break;
	}
	case FileType::symlink:
	{
		DIRSymlinkFile(static_cast<SymlinkFile*>(file));
		break;
	}
	case FileType::dirFile:
	{
		DIRDirectory(static_cast<DirectoryFile*>(file));
		break;
	}
	case FileType::symlinkd:
	{
		DirectoryFile* link = nullptr;
		try
		{
			// NOTE: get root link
			link = static_cast<SymlinkdFile*>(file)->getLinkDirectory();

			while (link->getType() == FileType::symlinkd)
			{
				link = static_cast<SymlinkdFile*>(link)->getLinkDirectory();
			}
			// NOTE: need a code that throws std::bad_alloc exception, WTF!
			link->getName();
		}
		catch (std::bad_alloc)
		{
			return false;
		}
		catch (...)
		{
			return false;
		}

		DIRDirectory(static_cast<SymlinkdFile*>(link));
		break;
	}
	}
	return true;
}

void DIRDirectory(DirectoryFile* file)
{
	if (file == nullptr)
	{
		return;
	}
	else
	{
		for (auto it = file->getChildren().begin(); it != file->getChildren().end(); it++)
		{
			auto child = it->second;

			// NOTE: step non-directory file
			if (ad_Argument)
			{
				if (child->getType() != FileType::dirFile)
				{
					continue;
				}
			}

			switch (child->getType())
			{
			case FileType::binFile:
			{
				DIRBinaryFile(static_cast<BinaryFile*>(child));
				break;
			}
			case FileType::symlink:
			{
				DIRSymlinkFile(static_cast<SymlinkFile*>(child));
				break;
			}
			case FileType::dirFile:
			{
				string fileInfo;
				fileInfo = fileInfo + child->getDate();
				fileInfo = fileInfo + child->getTime();
				fileInfo = fileInfo + child->getTypeString();
				fileInfo = fileInfo + it->first;
				infos.push_back(fileInfo);
				if (s_Argument)
				{
					// NOTE: step over . and ..
					if (it->first.compare(".") == 0 || it->first.compare("..") == 0)
					{
						break;
					}
					else
					{
						// NOTE: print directory
						auto _file = file;
						string path;
						while (_file != DiskSystem::getInstance()->getRootDirectory())
						{
							if (path.empty())
							{
								path = _file->getName() + "\\";
							}
							else
							{
								if (path.back() == '\\')
								{
									path = path.substr(0, path.size() - 1);
								}
								path = _file->getName() + "\\" + path;
							}
							_file = static_cast<DirectoryFile*>(_file->getParent());
						}
						cout << "\n Directory of " << path << "\n" << endl;
						// NOTE: print info
						for (auto info : infos)
						{
							cout << info << endl;
						}

						// NOTE: clear info
						infos.clear();

						// NOTE: recursive traverse
						DIRDirectory(static_cast<DirectoryFile*>(child));
					}
				}
				break;
			}
			case FileType::symlinkd:
			{
				DIRSymblinkdFile(static_cast<SymlinkdFile*>(child));
				break;
			}
			}
		}
	}
}

void DIRBinaryFile(BinaryFile* file)
{
	string fileInfo;
	fileInfo = fileInfo + file->getDate();
	fileInfo = fileInfo + file->getTime();
	fileInfo = fileInfo + file->getTypeString();

	auto sizeNum = file->getSize();
	string sizeStr = to_string(sizeNum);
	if (sizeStr.size() > 7)
	{
		// NOTE: no carry processing
		sizeStr = sizeStr.substr(0, 7);
	}
	string buff;
	auto length = sizeStr.size();
	if (length <= 3)
	{
		buff = string(9 - length, ' ') + sizeStr;
	}
	else if (length <= 6)
	{
		buff = string(9 - length - 1, ' ') + sizeStr.substr(0, length - 3) + "," + sizeStr.substr(length - 3);
	}
	else
	{
		buff = sizeStr.substr(0, 1) + "," + sizeStr.substr(1, 3) + "," + sizeStr.substr(4);
	}
	fileInfo = fileInfo + buff + " ";
	fileInfo = fileInfo + file->getName();
	fileInfo = fileInfo + file->getInfo();
	infos.push_back(fileInfo);
}

void DIRSymlinkFile(SymlinkFile* file)
{
	string fileInfo;
	fileInfo = fileInfo + file->getDate();
	fileInfo = fileInfo + file->getTime();
	fileInfo = fileInfo + file->getTypeString();
	fileInfo = fileInfo + file->getName();
	fileInfo = fileInfo + file->getInfo();
	infos.push_back(fileInfo);
}

void DIRDirectoryFile(DirectoryFile* file)
{
	string fileInfo;
	fileInfo = fileInfo + file->getDate();
	fileInfo = fileInfo + file->getTime();
	fileInfo = fileInfo + file->getTypeString();
	fileInfo = fileInfo + file->getName();
	infos.push_back(fileInfo);
}

void DIRSymblinkdFile(SymlinkdFile* file)
{
	string fileInfo;
	fileInfo = fileInfo + file->getDate();
	fileInfo = fileInfo + file->getTime();
	fileInfo = fileInfo + file->getTypeString();
	fileInfo = fileInfo + file->getName();
	fileInfo = fileInfo + file->getInfo();
	infos.push_back(fileInfo);
}