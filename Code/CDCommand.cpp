#include "stdafx.h"
#include "CommandFactory.h"
#include "ErrorMessage.h"

#include "VDos.h"
#include "File.h"
#include "BinaryFile.h"
#include "DirectoryFile.h"
#include "SymlinkFile.h"
#include "SymlinkdFile.h"

#include <exception>
#include <cassert>

// NOTE: |           for cd command               |
// NOTE: |----------------------------------------|
// NOTE: |     objects   |         result         |
// NOTE: |---------------+------------------------|
// NOTE: |      empty    |   print current path   |
// NOTE: |    size > 1   |   errorSyntaxMessage   |
// NOTE: |   BinaryFile  | errorDirInvalidMessage |
// NOTE: |  SymlinkFile  | errorDirInvalidMessage |
// NOTE: | DirectoryFile |         cd dir         |
// NOTE: | SymlinkdFile  |       cd smlinkd       |


Msg CDCommand(queue<Object> objects)
{
	if (objects.empty())
	{
		// NOTE: for cd print
		return Msg(true, ".", stack<File*>());
	}
	else if (objects.size() > 1)
	{
		return Msg(false, errorSyntaxMessage, stack<File*>());
	}
	else
	{
		// NOTE: one object
		auto object = objects.front();

		// NOTE: lost
		if (object.m_currentDirectory.top() == nullptr)
		{
			// NOTE: for 'cd /'
			if (object.m_path.m_pathQueue.empty())
			{
				// NOTE: clear VDos::_path
				while (VDos::_path.size() != 1)
				{
					VDos::_path.pop();
				}
				// NOTE: return partition directory
				stack<File*> currentDirectory;
				currentDirectory.push(DiskSystem::getInstance()->getRootDirectory()->search(initPartition, FileType::dirFile)->second);
				return Msg(true, "", currentDirectory);
			}
			else
			{
				return Msg(false, errorDirMessage, object.m_currentDirectory);
			}
		}

		// NOTE: object is DirectoryFile
		if (object.m_currentDirectory.top()->getType() == FileType::dirFile)
		{

		}
		// NOTE: object is SymlinkdFile
		else if (object.m_currentDirectory.top()->getType() == FileType::symlinkd)
		{
			// NOTE: to trigger try throw catch
			try
			{
				// NOTE: throw std:;bad_alloc exception, means its linker is invalid
				auto topDir = static_cast<SymlinkdFile*>(object.m_currentDirectory.top());
				auto link = topDir->getLinkDirectory();
				if (link == nullptr)
				{
					throw link;
				}
				else
				{
					link->getName();
				}
			}
			catch (std::bad_alloc)
			{
				return Msg(false, errorDirMessage, stack<File*>());
			}
			catch (DirectoryFile*)
			{
				return Msg(false, errorDirMessage, stack<File*>());
			}
			// NOTE: for secure, no crash!
			catch (...)
			{
				return Msg(false, errorDirMessage, stack<File*>());
			}
		}
		// NOTE: object is BinaryFile or SymlinkFile
		else
		{
			object.m_currentDirectory.top() = nullptr;
			return Msg(false, errorDirInvalidMessage, object.m_currentDirectory);
		}

		// NOTE: calculate path to print
		auto pathQueue = object.m_path.m_pathQueue;
		while (!pathQueue.empty())
		{
			// NOTE: dont do anything
			if (pathQueue.front().compare(".") == 0)
			{
			
			}
			// NOTE: pop last one(use stack)
			else if (pathQueue.front().compare("..") == 0)
			{
				if (VDos::_path.size() == 1)
				{
					break;
				}
				else
				{
					VDos::_path.pop();
				}
			}
			// NOTE: push one(use stack)
			else
			{
				// NOTE: for cd command
				if (pathQueue.front().compare(initPartition) != 0)
				{
					VDos::_path.push(pathQueue.front());
				}
			}
			pathQueue.pop();
		}

		// NOTE: change current
		return Msg(true, "", object.m_currentDirectory);
	}
}