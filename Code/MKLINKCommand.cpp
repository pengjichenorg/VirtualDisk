#include "stdafx.h"
#include "CommandFactory.h"
#include "ErrorMessage.h"

#include "BinaryFile.h"
#include "SymlinkFile.h"
#include "DirectoryFile.h"
#include "SymlinkdFile.h"

#include <cassert>

const vector<string> argumentVector = { "/D" };

static bool D_Argument = false;

Msg MKLINKCommand(queue<Object> objects)
{
	if (objects.size() != 2)
	{
		D_Argument = false;
		return Msg(false, errorSyntaxMessage, stack<File*>());
	}

	// NOTE: in mklink command, the first object is link
	// NOTE: in mklink command, the last object is source

	// NOTE: get arguments
	for (auto args = objects.front().m_arguments; !args.empty(); args.pop())
	{
		if (find(argumentVector.begin(), argumentVector.end(), args.front()) != argumentVector.end())
		{
			if (args.front().find("/D") != string::npos)
			{
				D_Argument = true;
			}
		}
		else
		{
			D_Argument = false;
			return Msg(false, errorInvalidSwitch + " - " + "\"" + args.front().substr(1) + "\"", stack<File*>());
		}
	}

	// NOTE: get source
	auto source = objects.back().m_currentDirectory.top();

	// NOTE: ensure source exist
	if (source == nullptr)
	{
		D_Argument = false;
		return Msg(false, errorFileMessage, stack<File*>());
	}

	string sourcePath;
	auto path = objects.back().m_path.m_pathQueue;
	while (!path.empty())
	{
		sourcePath = sourcePath + "\\" + path.front();
		path.pop();
	}
	
	// NOTE: get target
	auto target = objects.front().m_currentDirectory.top();
	auto targetName = objects.front().m_path.m_pathQueue.back();
	auto currentDirectoryCopy = objects.front().m_currentDirectory;
	// NOTE: for only C: in currentDirectory
	if (currentDirectoryCopy.size() != 1)
	{
		currentDirectoryCopy.pop();
	}
	auto targetParent = currentDirectoryCopy.top();

	// NOTE: ensure target is not exist
	if (target != nullptr)
	{
		D_Argument = false;
		return Msg(false, errorExistMessage, stack<File*>());
	}
	else if (targetParent == nullptr)
	{
		D_Argument = false;
		return Msg(false, errorDirMessage, stack<File*>());
	}

	// NOTE: create link file 
	if (D_Argument)
	{
		auto* file = new SymlinkdFile(targetName, static_cast<DirectoryFile*>(source));
		static_cast<DirectoryFile*>(targetParent)->addChild(file);
	}
	else
	{
		auto* file = new SymlinkFile(targetName, static_cast<BinaryFile*>(source));
		static_cast<DirectoryFile*>(targetParent)->addChild(file);
	}

	D_Argument = false;
	return Msg();
}