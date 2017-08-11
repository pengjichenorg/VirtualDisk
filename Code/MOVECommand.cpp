#include "stdafx.h"
#include "CommandFactory.h"
#include "ErrorMessage.h"

#include "BinaryFile.h"
#include "DirectoryFile.h"
#include "SymlinkFile.h"
#include "SymlinkdFile.h"

#include <exception>
#include <cassert>

// NOTE: for move command, there are some conditions as follow:
// NOTE: | source type | target type |         result         |
// NOTE: |    BF/SBF   |    BF/SBF   | move file file         |
// NOTE: |    BF/SBF   |    DF/SDF   | move file folder       |
// NOTE: |    DF/SDF   |    BF/SBF   | errorSyntaxMessage     |
// NOTE: |    DF/SDF   |    DF/SDF   | move folder\* folder\* |

const vector<string> argumentVector = { "/y" };

static bool y_Argument = false;

Msg MOVECommand(queue<Object> objects)
{
	if (objects.size() < 2)
	{
		y_Argument = false;
		return Msg(false, errorSyntaxMessage, stack<File*>());
	}

	// NOTE: get arguments
	for (auto args = objects.front().m_arguments; !args.empty(); args.pop())
	{
		if (find(argumentVector.begin(), argumentVector.end(), args.front()) != argumentVector.end())
		{
			if (args.front().find("/y") != string::npos)
			{
				y_Argument = true;
			}
		}
		else
		{
			y_Argument = false;
			return Msg(false, errorInvalidSwitch + " - " + "\"" + args.front().substr(1) + "\"", stack<File*>());
		}
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
		y_Argument = false;
		return Msg(false, errorDirMessage, stack<File*>());
	}

	if (target != nullptr)
	{
		// NOTE: target is general file or symbol general file
		if (target->getType() == FileType::binFile || target->getType() == FileType::symlink)
		{
			if (y_Argument)
			{
				// NOTE: move a file and cover target
				if (objects.size() != 2)
				{
					y_Argument = false;
					return Msg(false, errorSyntaxMessage, stack<File*>());
				}
				else
				{
					auto sourceFile = objects.front().m_currentDirectory.top();
					if (sourceFile == nullptr)
					{
						y_Argument = false;
						return Msg(false, errorFileMessage, stack<File*>());
					}
					else
					{
						if (sourceFile->getType() == FileType::dirFile || sourceFile->getType() == FileType::symlinkd)
						{
							y_Argument = false;
							return Msg(false, errorSyntaxMessage, stack<File*>());
						}
						// NOTE: source file is general file
						else if (sourceFile->getType() == FileType::binFile)
						{
							auto sf = static_cast<BinaryFile*>(sourceFile);
							auto sfParent = sourceFile->getParent();
							BinaryFile* file = new BinaryFile(*sf);
							file->setName(targetName);

							// NOTE: copy
							static_cast<DirectoryFile*>(targetParent)->addChild(file);
							// NOTE: delete
							static_cast<DirectoryFile*>(sfParent)->removeChild(sourceFile->getName(), sourceFile->getType());
						}
						// NOTE: source file is symbol file
						else
						{
							auto sf = static_cast<SymlinkFile*>(sourceFile);
							auto sfParent = sourceFile->getParent();
							SymlinkFile* file = new SymlinkFile(*sf);
							file->setName(targetName);
							sf->setLinkFile(static_cast<SymlinkFile*>(sourceFile)->getLinkFile());

							// NOTE: copy
							static_cast<DirectoryFile*>(targetParent)->addChild(file);
							// NOTE: delete
							static_cast<DirectoryFile*>(sfParent)->removeChild(sourceFile->getName(), sourceFile->getType());
						}
					}
				}
				y_Argument = false;
				return Msg();
			}
			else
			{
				y_Argument = false;
				return Msg(false, errorExistMessage, stack<File*>());
			}
		}
		// NOTE: target is directory or symbol directory file
		else
		{
			// NOTE: get taraget directory
			File* directory = nullptr;
			if (target->getType() == FileType::dirFile)
			{
				directory = target;
			}
			else if (target->getType() == FileType::symlinkd)
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
					y_Argument = false;
					return Msg(false, errorDirMessage, stack<File*>());
				}
				catch (...)
				{
					y_Argument = false;
					return Msg(false, errorDirMessage, stack<File*>());
				}
			}

			// NOTE: get source files, only copy non-directory file
			vector<File*> sources;

			while (objects.size() > 1)
			{
				sources.push_back(objects.front().m_currentDirectory.top());
				objects.pop();
			}

			// NOTE: same name error
			if (y_Argument != true)
			{
				for (auto source : sources)
				{
					if (static_cast<DirectoryFile*>(target)->search(source->getName(), source->getType()) != static_cast<DirectoryFile*>(target)->getChildren().end())
					{
						y_Argument = false;
						return Msg(false, errorExistMessage, stack<File*>());
					}
				}
			}

			// NOTE: move, dont need to change name
			for (auto source : sources)
			{
				// NOTE: source file is binary file
				if (source->getType() == FileType::binFile)
				{
					auto sf = static_cast<BinaryFile*>(source);
					auto sfParent = source->getParent();
					BinaryFile* file = new BinaryFile(*sf);

					// NOTE: copy
					static_cast<DirectoryFile*>(directory)->addChild(file);
					// NOTE: delete
					static_cast<DirectoryFile*>(sfParent)->removeChild(source->getName(), source->getType());
				}
				// NOTE: source file is symbol binary file
				else if (source->getType() == FileType::symlink)
				{
					auto sf = static_cast<SymlinkFile*>(source);
					auto sfParent = source->getParent();
					SymlinkFile* file = new SymlinkFile(*sf);
					sf->setLinkFile(static_cast<SymlinkFile*>(source)->getLinkFile());

					// NOTE: copy
					static_cast<DirectoryFile*>(directory)->addChild(file);
					// NOTE: delete
					static_cast<DirectoryFile*>(sfParent)->removeChild(source->getName(), source->getType());
				}
			}
			y_Argument = false;
			return Msg();
		}
	}

	// NOTE: target is file, not directory
	if (target == nullptr)
	{
		// NOTE: source must be a general file or symbol general file
		if (objects.size() != 2)
		{
			y_Argument = false;
			return Msg(false, errorSyntaxMessage, stack<File*>());
		}
		auto sourceFile = objects.front().m_currentDirectory.top();
		if (sourceFile == nullptr)
		{
			y_Argument = false;
			return Msg(false, errorFileMessage, stack<File*>());
		}
		else
		{
			if (sourceFile->getType() == FileType::dirFile || sourceFile->getType() == FileType::symlinkd)
			{
				y_Argument = false;
				return Msg(false, errorSyntaxMessage, stack<File*>());
			}
			// NOTE: source file is general file
			else if (sourceFile->getType() == FileType::binFile)
			{
				// NOTE: same name check
				if (static_cast<DirectoryFile*>(targetParent)->search(sourceFile->getName(), sourceFile->getType()) != static_cast<DirectoryFile*>(targetParent)->getChildren().end())
				{
					y_Argument = false;
					return Msg(false, errorExistMessage, stack<File*>());
				}

				auto sf = static_cast<BinaryFile*>(sourceFile);
				auto sfParent = sourceFile->getParent();
				BinaryFile* file = new BinaryFile(*sf);
				file->setName(targetName);

				// NOTE: copy
				static_cast<DirectoryFile*>(targetParent)->addChild(file);
				// NOTE: delete
				static_cast<DirectoryFile*>(sfParent)->removeChild(sourceFile->getName(), sourceFile->getType());
			}
			// NOTE: source file is symbol file
			else
			{
				// NOTE: same name check
				if (static_cast<DirectoryFile*>(targetParent)->search(sourceFile->getName(), sourceFile->getType()) != static_cast<DirectoryFile*>(targetParent)->getChildren().end())
				{
					y_Argument = false;
					return Msg(false, errorExistMessage, stack<File*>());
				}

				auto sf = static_cast<SymlinkFile*>(sourceFile);
				auto sfParent = sourceFile->getParent();
				SymlinkFile* file = new SymlinkFile(*sf);
				file->setName(targetName);
				sf->setLinkFile(static_cast<SymlinkFile*>(sourceFile)->getLinkFile());

				// NOTE: copy
				static_cast<DirectoryFile*>(targetParent)->addChild(file);
				// NOTE: delete
				static_cast<DirectoryFile*>(sfParent)->removeChild(sourceFile->getName(), sourceFile->getType());
			}
		}
	}

	y_Argument = false;
	return Msg();
}