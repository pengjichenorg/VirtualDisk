#include "stdafx.h"
#include "CommandFactory.h"
#include "ErrorMessage.h"

#include "BinaryFile.h"
#include "DirectoryFile.h"
#include "SymlinkFile.h"
#include "SymlinkdFile.h"

#include <fstream>
#include <cassert>
#include <exception>

// NOTE: for copy command, there are some conditions as follow:
// NOTE: | source type | target type |         result         |
// NOTE: |    BF/SBF   |    BF/SBF   | copy file file         |
// NOTE: |    BF/SBF   |    DF/SDF   | copy file folder       |
// NOTE: |    DF/SDF   |    BF/SBF   | errorSyntaxMessage     |
// NOTE: |    DF/SDF   |    DF/SDF   | copy folder\* folder\* |

Msg COPYFromDisk(queue<Object> objects);

Msg COPYCommand(queue<Object> objects)
{
	if (objects.size() < 2)
	{
		return Msg(false, errorSyntaxMessage, stack<File*>());
	}

	// NOTE: get target
	auto target = objects.back().m_currentDirectory.top();
	auto currentDirectoryCopy = objects.back().m_currentDirectory;
	// NOTE: for only C: in currentDirectory
	if (currentDirectoryCopy.size() != 1)
	{
		currentDirectoryCopy.pop();
	}
	auto targetParent = currentDirectoryCopy.top();
	auto targetName = objects.back().m_path.m_pathQueue.back();

	// NOTE: ensure target directory exist or target file's directory exist
	if (target == nullptr && targetParent == nullptr)
	{
		return Msg(false, errorDirMessage, stack<File*>());
	}

	// NOTE: target is not nullptr, then target must be directory/symlinkd
	if (target != nullptr)
	{
		if (target->getType() == FileType::binFile || target->getType() == FileType::symlink)
		{
			return Msg(false, errorExistMessage, stack<File*>());
		}
	}

	// NOTE: target must be nullptr
	if (target == nullptr)
	{
		// NOTE: source must be a general file or symbol general file
		if (objects.size() != 2)
		{
			return Msg(false, errorSyntaxMessage, stack<File*>());
		}
		auto sourceFile = objects.front().m_currentDirectory.top();
		if (sourceFile == nullptr)
		{
			// NOTE: copy file from disk
			if (objects.front().m_path.m_pathQueue.front().find("@") != string::npos)
			{
				return COPYFromDisk(objects);
			}
			return Msg(false, errorFileMessage, stack<File*>());
		}
		else
		{
			if (sourceFile->getType() == FileType::dirFile || sourceFile->getType() == FileType::symlinkd)
			{
				return Msg(false, errorSyntaxMessage, stack<File*>());
			}
			// NOTE: source file is general file
			else if (sourceFile->getType() == FileType::binFile)
			{
				auto sf = static_cast<BinaryFile*>(sourceFile);
				BinaryFile* file = new BinaryFile(*sf);
				file->setName(targetName);

				// NOTE: copy
				static_cast<DirectoryFile*>(targetParent)->addChild(file);
			}
			// NOTE: source file is symbol file
			else
			{
				auto sf = static_cast<SymlinkFile*>(sourceFile);
				SymlinkFile* file = new SymlinkFile(*sf);
				file->setName(targetName);
				sf->setLinkFile(static_cast<SymlinkFile*>(sourceFile)->getLinkFile());

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
		if (target->getType() == FileType::dirFile)
		{
			directory = target;
		}
		else
		{
			try
			{
				// NOTE: get root link
				DirectoryFile* link = static_cast<SymlinkdFile*>(target)->getLinkDirectory();
				while (link->getType() == FileType::symlinkd)
				{
					link = static_cast<SymlinkdFile*>(link)->getLinkDirectory();
				}
				// NOTE: need a code that throws std::bad_alloc exception, WTF!
				link->getName();

				directory = link;
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

		// NOTE: get source files, only copy non-directory file
		vector<File*> sources;

		while (objects.size() != 1)
		{
			auto object = objects.front();
			if (object.m_currentDirectory.top() == nullptr)
			{
				return Msg(false, errorFileMessage, stack<File*>());
			}
			else
			{
				if (object.m_currentDirectory.top()->getType() == FileType::dirFile || object.m_currentDirectory.top()->getType() == FileType::symlinkd)
				{
					// NOTE: get dir
					File* dir;
					if (object.m_currentDirectory.top()->getType() == FileType::dirFile)
					{
						dir = object.m_currentDirectory.top();
					}
					else
					{
						// NOTE: get root link
						try
						{
							DirectoryFile* link = static_cast<SymlinkdFile*>(object.m_currentDirectory.top())->getLinkDirectory();
							while (link->getType() == FileType::symlinkd)
							{
								link = static_cast<SymlinkdFile*>(link)->getLinkDirectory();
							}
				
							// NOTE: need a code that throws std::bad_alloc exception, WTF!
							link->getName();

							dir = link;
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

					// NOTE: add source
					for (auto it = static_cast<DirectoryFile*>(dir)->getChildren().begin();
					it != static_cast<DirectoryFile*>(dir)->getChildren().end(); it++)
					{
						if (it->second->getType() == FileType::dirFile || it->second->getType() == FileType::symlinkd)
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
			
			// NOTE: check if target has source
			if (static_cast<DirectoryFile*>(directory)->search(object.m_currentDirectory.top()->getName(), object.m_currentDirectory.top()->getType())
				!= static_cast<DirectoryFile*>(directory)->getChildren().end())
			{
				return Msg(false, errorExistMessage, stack<File*>());
			}
			else
			{
				sources.push_back(object.m_currentDirectory.top());
			}
			objects.pop();
		}

		// NOTE: copy, dont need to change name
		for (auto source : sources)
		{
			// NOTE: source file is general file
			if (source->getType() == FileType::binFile)
			{
				auto sf = static_cast<BinaryFile*>(source);
				BinaryFile* file = new BinaryFile(*sf);
				static_cast<DirectoryFile*>(directory)->addChild(file);
			}
			// NOTE: source file is symbol general file
			else
			{
				auto sf = static_cast<SymlinkFile*>(source);
				SymlinkFile* file = new SymlinkFile(*sf);
				sf->setLinkFile(static_cast<SymlinkFile*>(source)->getLinkFile());
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

	_path = objects.back().m_path.m_pathQueue;
	_pathString.clear();
	while (!_path.empty())
	{
		_pathString = _pathString + "\\" + _path.front();
		_path.pop();
	}

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
		return Msg(false, errorFileMessage, stack<File*>());
	}

	// NOTE: get target
	auto target = objects.back().m_currentDirectory.top();

	// NOTE: ensure target is not exist when it is general file
	if (target != nullptr && (target->getType() == FileType::binFile || target->getType() == FileType::symlink))
	{
		return Msg(false, errorExistMessage, stack<File*>());
	}

	// NOTE: get target parent
	auto currentDirectoryCopy = objects.back().m_currentDirectory;
	// NOTE: for only C: in currentDirectory
	if (currentDirectoryCopy.size() != 1)
	{
		currentDirectoryCopy.pop();
	}
	auto targetParent = currentDirectoryCopy.top();
	if (targetParent == nullptr)
	{
		return Msg(false, errorDirMessage, stack<File*>());
	}

	assert(target == nullptr && targetParent != nullptr);

	// NOTE: generate general file
	auto file = new BinaryFile(sourceFileName);
	file->setBinData(fileData);

	// NOTE: add file to virtual disk
	static_cast<DirectoryFile*>(targetParent)->addChild(file);

	return Msg();
}
