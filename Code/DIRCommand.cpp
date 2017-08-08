#pragma once
#include "stdafx.h"

#include <cstdio>
#include <vector>
#include <exception>

#include "Msg.h"
#include "Object.h"
#include "ErrorMessage.h"
#include "DirectoryFile.h"
#include "GeneralFile.h"
#include "SymbolDirectoryFile.h"
#include "SymbolGeneralFile.h"

#include "DiskSystem.h"

using namespace std;

bool DIRCommandForEach(Object file);
void DIRDirectory(DirectoryFile* file);
void DIRGeneralFile(GeneralFile* file);
void DIRSymlinkFile(SymbolGeneralFile* file);
void DIRDirectoryFile(DirectoryFile* file);
void DIRSymblinkdFile(SymbolDirectoryFile* file);

const vector<string> argumentVector = {"/s", "/ad"};

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
		// cout << "TEST: size == 1" << endl;

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
				if (argument.find("/ad") != string::npos)
				{
					ad_Argument = true;
				}
			}
			else
			{
				return Msg(false, errorInvalidSwitch + " - " + "\"" + argument.substr(1) + "\"", nullptr);
			}
		}

		// NOTE: clear info
		infos.clear();
		if (objects.front().m_file == nullptr)
		{
			infos.clear();
			return Msg(false, errorNonFileMessage, nullptr);
		}
		else
		{
			if (ad_Argument)
			{
				if (objects.front().m_file->getType() != FileType::directoryFile)
				{
					return Msg();
				}
			}
			if (!DIRCommandForEach(objects.front()))
			{
				return Msg(false, errorDirMessage, nullptr);
			}
		}

		// NOTE: print directory
		auto _file = objects.front().m_file;
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
		// cout << "TEST: size > 1" << endl;
		while (!objects.empty())
		{
			auto object = objects.front();

			// cout << "TEST: arguments:";
			for (auto argument : objects.front().m_arguments)
			{
				if (find(argumentVector.begin(), argumentVector.end(), argument) != argumentVector.end())
				{
					if (argument.find("/s") != string::npos)
					{
						s_Argument = true;
					}
					if (argument.find("/ad") != string::npos)
					{
						ad_Argument = true;
					}
				}
				else
				{
					return Msg(false, errorInvalidSwitch + " - " + "\"" + argument.substr(1) + "\"", nullptr);
				}
			}

			if (object.m_file == nullptr)
			{
				infos.clear();
				return Msg(false, errorNonFileMessage, nullptr);
			}
			else
			{
				// NOTE: ensure every object is not nullptr
				if (ad_Argument)
				{
					if (object.m_file->getType() != FileType::directoryFile)
					{
						continue;
					}
				}
				// cout << "TEST: object:" << object.m_file->getName() << endl;
				if (!DIRCommandForEach(object))
				{
					return Msg(false, errorDirMessage, nullptr);
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
	auto file = object.m_file;
	switch (file->getType())
	{
		case FileType::generalFile:
		{
			DIRGeneralFile(static_cast<GeneralFile*>(file));
			break;
		}
		case FileType::symlink:
		{
			DIRSymlinkFile(static_cast<SymbolGeneralFile*>(file));
			break;
		}
		case FileType::directoryFile:
		{	
			DIRDirectory(static_cast<DirectoryFile*>(file));
			break;
		}
		case FileType::symlinkd:
		{
			// NOTE: get root link
			DirectoryFile* link = static_cast<SymbolDirectoryFile*>(file)->getLinkDirectory();
			try
			{
				while (link->getType() == FileType::symlinkd)
				{
					link = static_cast<SymbolDirectoryFile*>(link)->getLinkDirectory();
				}
				// NOTE: need a code that throws std::bad_alloc exception, WTF!
				link->getName();
			}
			catch (std::bad_alloc)
			{
				return false;
			}

			DIRDirectory(static_cast<SymbolDirectoryFile*>(link));
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
				if (child->getType() != FileType::directoryFile)
				{
					continue;
				}
			}

			switch (child->getType())
			{
				case FileType::generalFile:
				{
					DIRGeneralFile(static_cast<GeneralFile*>(child));
					break;
				}
				case FileType::symlink:
				{
					DIRSymlinkFile(static_cast<SymbolGeneralFile*>(child));
					break;
				}
				case FileType::directoryFile:
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

							// NOTE: recursive traverse
							DIRDirectory(static_cast<DirectoryFile*>(child));
						}
					}
					break;
				}
				case FileType::symlinkd:
				{
					DIRSymblinkdFile(static_cast<SymbolDirectoryFile*>(child));
					break;
				}
			}
		}
	}
}

void DIRGeneralFile(GeneralFile* file)
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

void DIRSymlinkFile(SymbolGeneralFile* file)
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

void DIRSymblinkdFile(SymbolDirectoryFile* file)
{
	string fileInfo;
	fileInfo = fileInfo + file->getDate();
	fileInfo = fileInfo + file->getTime();
	fileInfo = fileInfo + file->getTypeString();
	fileInfo = fileInfo + file->getName();
	fileInfo = fileInfo + file->getInfo();
	infos.push_back(fileInfo);
}