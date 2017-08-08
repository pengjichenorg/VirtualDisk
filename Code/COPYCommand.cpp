#include "stdafx.h"
#include "CommandFactory.h"
#include "ErrorMessage.h"

#include <fstream>
#include <cassert>
#include <exception>

#include "GeneralFile.h"
#include "DirectoryFile.h"
#include "SymbolGeneralFile.h"
#include "SymbolDirectoryFile.h"

// NOTE: for copy command, there are some conditions as follow:
// NOTE: | source type | target type |         result         |
// NOTE: |    GF/SGF   |    GF/SGF   | copy file file         |
// NOTE: |    GF/SGF   |    DF/SDF   | copy file folder       |
// NOTE: |    DF/SDF   |    GF/SGF   | errorSyntaxMessage     |
// NOTE: |    DF/SDF   |    DF/SDF   | copy folder\* folder\* |

Msg COPYFromDisk(queue<Object> objects);

Msg COPYCommand(queue<Object> objects)
{
	if (objects.size() < 2)
	{
		// cout << "TEST: objects is empty" << endl;
		return Msg(false, errorSyntaxMessage, nullptr);
	}

	// NOTE: get target
	auto target = objects.back().m_file;
	auto targetParent = objects.back().m_fileParent;
	auto targetName = objects.back().m_path.m_pathQueue.back();

	// NOTE: ensure target directory exist or target file's directory exist
	if (target == nullptr && targetParent == nullptr)
	{
		return Msg(false, errorDirMessage, nullptr);
	}

	// NOTE: target is general file or symbol general file
	if (target != nullptr)
	{
		// NOTE: then targetName is directory file name
		if (target->getType() == FileType::generalFile || target->getType() == FileType::symlink)
		{
			return Msg(false, errorExistMessage, nullptr);
		}	
	}

	// cout << "TEST: target directory:" << (target==nullptr?"nullptr":target->getName()) << endl;
	// cout << "TEST: target parent:" << (targetParent==nullptr?"nullptr":targetParent->getName()) << endl;

	// NOTE: target must be nullptr
	if (target == nullptr)
	{
		// NOTE: source must be a general file or symbol general file
		if(objects.size() != 2)
		{
			return Msg(false, errorSyntaxMessage, nullptr);
		}
		auto sourceFile = objects.front().m_file;
		if (sourceFile == nullptr)
		{
			// NOTE: copy file from disk
			if (objects.front().m_path.m_pathQueue.front().find("@") != string::npos)
			{
				return COPYFromDisk(objects);
			}
			return Msg(false, errorFileMessage, nullptr);
		}
		else
		{
			if (sourceFile->getType() == FileType::directoryFile || sourceFile->getType() == FileType::symlinkd)
			{
				return Msg(false, errorSyntaxMessage, nullptr);
			}
			// NOTE: source file is general file
			else if(sourceFile->getType() == FileType::generalFile)
			{
				auto sf = static_cast<GeneralFile*>(sourceFile);
				GeneralFile* file = new GeneralFile(*sf);
				file->setName(targetName);

				// cout << "TEST: source file:" << file->getName() << endl;

				// NOTE: copy
				static_cast<DirectoryFile*>(targetParent)->addChild(file);
			}
			// NOTE: source file is symbol file
			else
			{
				auto sf = static_cast<SymbolGeneralFile*>(sourceFile);
				SymbolGeneralFile* file = new SymbolGeneralFile(*sf);
				file->setName(targetName);
				sf->setLinkFile(static_cast<SymbolGeneralFile*>(sourceFile)->getLinkFile());

				// cout << "TEST: source file:" << file->getName() << endl;

				// NOTE: copy
				static_cast<DirectoryFile*>(targetParent)->addChild(file);
			}
		}
	}
	// NOTE: target must be directory or symbol directory file
	else
	{
		// NOTE: get taraget directory
		File* directory;
		if (target->getType() == FileType::directoryFile)
		{
			directory = target;
		}
		else
		{
			// NOTE: get root link
			DirectoryFile* link = static_cast<SymbolDirectoryFile*>(target)->getLinkDirectory();
			try
			{
				while (link->getType() == FileType::symlinkd)
				{
					link = static_cast<SymbolDirectoryFile*>(link)->getLinkDirectory();
				}
				// NOTE: need a code that throws std::bad_alloc exception, WTF!
				link->getName();

				directory = link;
			}
			catch (std::bad_alloc)
			{
				return Msg(false, errorDirMessage, nullptr);
			}
		}
		
		// NOTE: get source files, only copy non-directory file
		vector<File*> sources;

		while (!objects.empty())
		{
			auto object = objects.front();
			if (object.m_file == nullptr)
			{
				// cout << "TEST: error file message" << endl;
				return Msg(false, errorFileMessage, nullptr);
			}
			else
			{
				// cout << "TEST: object file name:" << object.m_file->getName() << endl;
				if (object.m_file->getType() == FileType::directoryFile || object.m_file->getType() == FileType::symlinkd)
				{
					// NOTE: get dir
					File* dir;
					if (object.m_file->getType() == FileType::directoryFile)
					{
						dir = object.m_file;
					}
					else
					{
						// NOTE: get root link
						DirectoryFile* link = static_cast<SymbolDirectoryFile*>(object.m_file)->getLinkDirectory();
						try
						{
							while (link->getType() == FileType::symlinkd)
							{
								link = static_cast<SymbolDirectoryFile*>(link)->getLinkDirectory();
								// cout << "TEST: after get link, link name:" << link->getName() << endl;
								// cout << "TEST: after get link, link type:" << link->getTypeString() << endl;
							}
							// cout << "TEST: link end name:" << link->getName() << endl;
							// cout << "TEST: link end type:" << link->getTypeString() << endl;

							// NOTE: need a code that throws std::bad_alloc exception, WTF!
							link->getName();

							dir = link;
						}
						catch (std::bad_alloc)
						{
							return Msg(false, errorDirMessage, nullptr);
						}
					}

					// NOTE: add source
					for (auto it = static_cast<DirectoryFile*>(dir)->getChildren().begin();
						it != static_cast<DirectoryFile*>(dir)->getChildren().end(); it++)
					{
						if (it->second->getType() == FileType::directoryFile || it->second->getType() == FileType::symlinkd)
						{
							continue;
						}
						else
						{
							sources.push_back(it->second);
						}
					}

					objects.pop();
					continue;
				}
			}
			// cout << "TEST: push source file:" << object.m_file->getName() << endl;
			sources.push_back(object.m_file);
			objects.pop();
		}

		// NOTE: copy, dont need to change name
		for (auto source : sources)
		{
			// cout << "TEST: source:" << source->getName() << endl;
			// NOTE: source file is general file
			if (source->getType() == FileType::generalFile)
			{
				auto sf = static_cast<GeneralFile*>(source);
				GeneralFile* file = new GeneralFile(*sf);
				static_cast<DirectoryFile*>(directory)->addChild(file);
			}
			// NOTE: source file is symbol general file
			else
			{
				auto sf = static_cast<SymbolGeneralFile*>(source);
				SymbolGeneralFile* file = new SymbolGeneralFile(*sf);
				sf->setLinkFile(static_cast<SymbolGeneralFile*>(source)->getLinkFile());
				static_cast<DirectoryFile*>(targetParent)->addChild(file);
			}
		}
	}

	return Msg();
}

Msg COPYFromDisk(queue<Object> objects)
{
	// NOTE: get source
	auto _path = objects.front().m_path.m_pathQueue;
	string _pathString;
	while (!_path.empty())
	{
		_pathString = _pathString + "\\" + _path.front();
		_path.pop();
	}

	auto sourceFilePath = _pathString.substr(2);
	auto sourceFileName = objects.front().m_path.m_pathQueue.back();

	// cout << "TEST: physical path:" << sourceFilePath << endl;

	_path = objects.back().m_path.m_pathQueue;
	_pathString.clear();
	while (!_path.empty())
	{
		_pathString = _pathString + "\\" + _path.front();
		_path.pop();
	}
	// cout << "TEST: target path:" << _pathString.substr(1) << endl;

	vector<unsigned char> fileData;
	ifstream inputFile;
	inputFile.open(sourceFilePath, ios::binary);
	if (inputFile)
	{
		inputFile.seekg(0, ios::end);
		fileData.resize((unsigned int)inputFile.tellg());
		inputFile.seekg(0, ios::beg);
		inputFile.read(reinterpret_cast<char*>(fileData.data()), fileData.size());

		inputFile.close();
	}
	else
	{
		return Msg(false, errorFileMessage, nullptr);
	}

	// NOTE: get target
	auto target = objects.back().m_file;

	// NOTE: ensure target is not exist when it is general file
	if (target != nullptr && (target->getType() == FileType::generalFile || target->getType() == FileType::symlink))
	{
		return Msg(false, errorExistMessage, nullptr);
	}

	// NOTE: get target parent
	auto targetParent = objects.back().m_fileParent;
	if (targetParent == nullptr)
	{
		return Msg(false, errorDirMessage, nullptr);
	}

	assert(target == nullptr && targetParent != nullptr);

	// NOTE: generate general file
	auto file = new GeneralFile(sourceFileName);
	file->setBinData(fileData);

	// NOTE: add file to virtual disk
	static_cast<DirectoryFile*>(targetParent)->addChild(file);

	// NOTE: test
	// cout << "TEST: ";
	// for (auto d : fileData)
	// {
	// 	cout << d;
	// }
	// cout << endl;

	return Msg();
}
