#include "stdafx.h"
#include "CommandFactory.h"
#include "ErrorMessage.h"

#include <exception>
#include <cassert>

#include "VDos.h"
#include "SymbolDirectoryFile.h"

Msg CDCommand(queue<Object> objects)
{
	if (objects.empty())
	{
		// cout << "TEST: objects is empty" << endl;
		return Msg(true, ".", nullptr);
	}

	if (objects.size() > 1)
	{
		return Msg(false, errorSyntaxMessage, nullptr);
	}

	// NOTE: one object
	auto object = objects.front();

	// NOTE: lost
	if (object.m_file == nullptr)
	{
		return Msg(false, errorDirMessage, nullptr);
	}

	DirectoryFile* dir = static_cast<DirectoryFile*>(object.m_file);

	// NOTE: symbol directory file
	if (object.m_file->getType() == FileType::symlinkd)
	{
		// cout << "TEST: cd symbol directory file" << endl;
		// NOTE: get root link
		DirectoryFile* link = static_cast<SymbolDirectoryFile*>(dir)->getLinkDirectory();

		if (link != nullptr)
		{
			// cout << "TEST: link not nullptr" << endl;
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
				// TEST: print VDos::_path
				auto vPath = VDos::_path;
				while (!vPath.empty())
				{
					// cout << "TEST: vPath:" << vPath.top() << endl;
					vPath.pop();
				}
				return Msg(false, errorDirMessage, nullptr);
			}
		}
		else
		{
			// cout << "TEST: link nullptr" << endl;
			return Msg(false, errorDirMessage, nullptr);
		}
	}

	// NOTE: calculate path to print
	// cout << "TEST: output cd path:" << endl;
	auto pathQueue = object.m_path.m_pathQueue;
	while (!pathQueue.empty())
	{
		// cout << pathQueue.front() << "\\";
		// NOTE: dont do anything
		if (pathQueue.front().compare(".") == 0)
		{
			// cout << "TEST: do nothing" << endl;
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
			// cout << "TEST: _path size:" << VDos::_path.size() << endl;
			VDos::_path.push(pathQueue.front());
		}
		pathQueue.pop();
	}
	// cout << endl;

	// NOTE: change current
	return Msg(true, "", dir);
}