#include "stdafx.h"
#include "CommandFactory.h"
#include "ErrorMessage.h"

#include "GeneralFile.h"
#include "DirectoryFile.h"
#include "SymbolGeneralFile.h"
#include "SymbolDirectoryFile.h"

#include <exception>

// NOTE: for move command, there are some conditions as follow:
// NOTE: | source type | target type |         result         |
// NOTE: |    GF/SGF   |    GF/SGF   | move file file         |
// NOTE: |    GF/SGF   |    DF/SDF   | move file folder       |
// NOTE: |    DF/SDF   |    GF/SGF   | errorSyntaxMessage     |
// NOTE: |    DF/SDF   |    DF/SDF   | move folder\* folder\* |

const vector<string> argumentVector = { "/y" };

static bool y_Argument = false;

Msg MOVECommand(queue<Object> objects)
{
	if (objects.size() < 2)
	{
		y_Argument = false;
		return Msg(false, errorSyntaxMessage, nullptr);
	}

	// NOTE: get arguments
	for (auto argument : objects.front().m_arguments)
	{
		if (find(argumentVector.begin(), argumentVector.end(), argument) != argumentVector.end())
		{
			if (argument.find("/y") != string::npos)
			{
				y_Argument = true;
			}
		}
		else
		{
			y_Argument = false;
			return Msg(false, errorInvalidSwitch + " - " + "\"" + argument.substr(1) + "\"", nullptr);
		}
	}

	// NOTE: get target
	auto target = objects.back().m_file;
	auto targetParent = objects.back().m_fileParent;
	auto targetName = objects.back().m_path.m_pathQueue.back();

	// NOTE: ensure target directory exist or target file's directory exist
	if (target == nullptr && targetParent == nullptr)
	{
		y_Argument = false;
		return Msg(false, errorDirMessage, nullptr);
	}

	if (target != nullptr)
	{
		// cout << "TEST: target name:" << targetName << endl;
		// cout << "TEST: target file name:" << target->getName() << endl;
		// NOTE: target is general file or symbol general file
		if (target->getType() == FileType::generalFile || target->getType() == FileType::symlink)
		{
			if (y_Argument)
			{
				// NOTE: move a file and cover target
				if (objects.size() != 2)
				{
					y_Argument = false;
					return Msg(false, errorSyntaxMessage, nullptr);
				}
				else
				{
					auto sourceFile = objects.front().m_file;
					if (sourceFile == nullptr)
					{
						y_Argument = false;
						return Msg(false, errorFileMessage, nullptr);
					}
					else
					{
						if (sourceFile->getType() == FileType::directoryFile || sourceFile->getType() == FileType::symlinkd)
						{
							y_Argument = false;
							return Msg(false, errorSyntaxMessage, nullptr);
						}
						// NOTE: source file is general file
						else if (sourceFile->getType() == FileType::generalFile)
						{
							auto sf = static_cast<GeneralFile*>(sourceFile);
							auto sfParent = sourceFile->getParent();
							GeneralFile* file = new GeneralFile(*sf);
							file->setName(targetName);

							// cout << "TEST: source file:" << file->getName() << endl;

							// NOTE: copy
							static_cast<DirectoryFile*>(targetParent)->addChild(file);
							// NOTE: delete
							static_cast<DirectoryFile*>(sfParent)->removeChild(sourceFile->getName());
						}
						// NOTE: source file is symbol file
						else
						{
							auto sf = static_cast<SymbolGeneralFile*>(sourceFile);
							auto sfParent = sourceFile->getParent();
							SymbolGeneralFile* file = new SymbolGeneralFile(*sf);
							file->setName(targetName);
							sf->setLinkFile(static_cast<SymbolGeneralFile*>(sourceFile)->getLinkFile());

							// cout << "TEST: source file:" << file->getName() << endl;

							// NOTE: copy
							static_cast<DirectoryFile*>(targetParent)->addChild(file);
							// NOTE: delete
							static_cast<DirectoryFile*>(sfParent)->removeChild(sourceFile->getName());
						}
					}
				}
				y_Argument = false;
				return Msg();
			}
			else
			{
				y_Argument = false;
				return Msg(false, errorExistMessage, nullptr);
			}
		}
		// NOTE: target is directory or symbol directory file
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
					y_Argument = false;
					return Msg(false, errorDirMessage, nullptr);
				}
			}

			// NOTE: get source files, only copy non-directory file
			vector<File*> sources;

			//  if(objects.front())

			while (objects.size() > 1)
			{
				sources.push_back(objects.front().m_file);
				// cout << "TEST: move file name:" << objects.front().m_file->getName() << endl;
				objects.pop();
			}

			// NOTE: same name error
			if (y_Argument != true)
			{
				for (auto source : sources)
				{
					if (static_cast<DirectoryFile*>(target)->search(source->getName()) != static_cast<DirectoryFile*>(target)->getChildren().end())
					{
						y_Argument = false;
						return Msg(false, errorExistMessage, nullptr);
					}
				}
			}

			//while (!objects.empty())
			//{
			//	auto object = objects.front();
			//	if (object.m_file == nullptr)
			//	{
			//		// cout << "TEST: error file message" << endl;
			//		return Msg(false, errorFileMessage, nullptr);
			//	}
			//	else
			//	{
			//		// cout << "TEST: object file name:" << object.m_file->getName() << endl;
			//		if (object.m_file->getType() == FileType::directoryFile || object.m_file->getType() == FileType::symlinkd)
			//		{
			//			// NOTE: get dir
			//			File* dir;
			//			if (object.m_file->getType() == FileType::directoryFile)
			//			{
			//				dir = object.m_file;
			//			}
			//			else
			//			{
			//				// NOTE: get root link
			//				DirectoryFile* link = static_cast<SymbolDirectoryFile*>(object.m_file)->getLinkDirectory();
			//				while (object.m_file->getType() == FileType::symlinkd)
			//				{
			//					object.m_file = static_cast<SymbolDirectoryFile*>(link)->getLinkDirectory();
			//				}
			//				dir = link;
			//			}

			//			// NOTE: add source
			//			for (auto it = static_cast<DirectoryFile*>(dir)->getChildren().begin();
			//			it != static_cast<DirectoryFile*>(dir)->getChildren().end(); it++)
			//			{
			//				if (it->second->getType() == FileType::directoryFile || it->second->getType() == FileType::symlinkd)
			//				{
			//					continue;
			//				}
			//				else
			//				{
			//					sources.push_back(it->second);
			//				}
			//			}

			//			objects.pop();
			//			continue;
			//		}
			//	}
			//	// cout << "TEST: push source file:" << object.m_file->getName() << endl;
			//	sources.push_back(object.m_file);
			//	objects.pop();
			//}

			// NOTE: move, dont need to change name
			for (auto source : sources)
			{
				// cout << "TEST: source:" << source->getName() << endl;
				// NOTE: source file is general file
				if (source->getType() == FileType::generalFile)
				{
					auto sf = static_cast<GeneralFile*>(source);
					auto sfParent = source->getParent();
					GeneralFile* file = new GeneralFile(*sf);

					// cout << "TEST: source file:" << file->getName() << endl;

					// NOTE: copy
					static_cast<DirectoryFile*>(directory)->addChild(file);
					// NOTE: delete
					static_cast<DirectoryFile*>(sfParent)->removeChild(source->getName());
				}
				// NOTE: source file is symbol general file
				else
				{
					auto sf = static_cast<SymbolGeneralFile*>(source);
					auto sfParent = source->getParent();
					SymbolGeneralFile* file = new SymbolGeneralFile(*sf);
					sf->setLinkFile(static_cast<SymbolGeneralFile*>(source)->getLinkFile());

					// cout << "TEST: source file:" << file->getName() << endl;

					// NOTE: copy
					static_cast<DirectoryFile*>(directory)->addChild(file);
					// NOTE: delete
					static_cast<DirectoryFile*>(sfParent)->removeChild(source->getName());
				}
			}
			y_Argument = false;
			return Msg();
		}
	}

	// cout << "TEST: target directory:" << (target == nullptr ? "nullptr" : target->getName()) << endl;
	// cout << "TEST: target parent:" << (targetParent == nullptr ? "nullptr" : targetParent->getName()) << endl;

	// NOTE: target is file, not directory
	if (target == nullptr)
	{
		// NOTE: source must be a general file or symbol general file
		if (objects.size() != 2)
		{
			y_Argument = false;
			return Msg(false, errorSyntaxMessage, nullptr);
		}
		auto sourceFile = objects.front().m_file;
		if (sourceFile == nullptr)
		{
			y_Argument = false;
			return Msg(false, errorFileMessage, nullptr);
		}
		else
		{
			if (sourceFile->getType() == FileType::directoryFile || sourceFile->getType() == FileType::symlinkd)
			{
				y_Argument = false;
				return Msg(false, errorSyntaxMessage, nullptr);
			}
			// NOTE: source file is general file
			else if (sourceFile->getType() == FileType::generalFile)
			{
				// NOTE: same name check
				if (static_cast<DirectoryFile*>(targetParent)->search(sourceFile->getName()) != static_cast<DirectoryFile*>(targetParent)->getChildren().end())
				{
					y_Argument = false;
					return Msg(false, errorExistMessage, nullptr);
				}

				auto sf = static_cast<GeneralFile*>(sourceFile);
				auto sfParent = sourceFile->getParent();
				GeneralFile* file = new GeneralFile(*sf);
				file->setName(targetName);

				// cout << "TEST: source file:" << file->getName() << endl;

				// NOTE: copy
				static_cast<DirectoryFile*>(targetParent)->addChild(file);
				// NOTE: delete
				static_cast<DirectoryFile*>(sfParent)->removeChild(sourceFile->getName());
			}
			// NOTE: source file is symbol file
			else
			{
				// NOTE: same name check
				if (static_cast<DirectoryFile*>(targetParent)->search(sourceFile->getName()) != static_cast<DirectoryFile*>(targetParent)->getChildren().end())
				{
					y_Argument = false;
					return Msg(false, errorExistMessage, nullptr);
				}

				auto sf = static_cast<SymbolGeneralFile*>(sourceFile);
				auto sfParent = sourceFile->getParent();
				SymbolGeneralFile* file = new SymbolGeneralFile(*sf);
				file->setName(targetName);
				sf->setLinkFile(static_cast<SymbolGeneralFile*>(sourceFile)->getLinkFile());

				// cout << "TEST: source file:" << file->getName() << endl;

				// NOTE: copy
				static_cast<DirectoryFile*>(targetParent)->addChild(file);
				// NOTE: delete
				static_cast<DirectoryFile*>(sfParent)->removeChild(sourceFile->getName());
			}
		}
	}

	y_Argument = false;
	return Msg();
}