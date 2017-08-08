#include "stdafx.h"
#include "CommandFactory.h"
#include "ErrorMessage.h"

#include "GeneralFile.h"
#include "SymbolGeneralFile.h"
#include "DirectoryFile.h"
#include "SymbolDirectoryFile.h"

const vector<string> argumentVector = { "/D" };

static bool D_Argument = false;

Msg MKLINKCommand(queue<Object> objects)
{
	if (objects.size() != 2)
	{
		D_Argument = false;
		return Msg(false, errorSyntaxMessage, nullptr);
	}

	// NOTE: in mklink command, the first object is link
	// NOTE: in mklink command, the last object is source

	// NOTE: get arguments
	for (auto argument : objects.front().m_arguments)
	{
		if (find(argumentVector.begin(), argumentVector.end(), argument) != argumentVector.end())
		{
			if (argument.find("/D") != string::npos)
			{
				D_Argument = true;
			}
		}
		else
		{
			D_Argument = false;
			return Msg(false, errorInvalidSwitch + " - " + "\"" + argument.substr(1) + "\"", nullptr);
		}
	}

	// NOTE: get source
	auto source = objects.back().m_file;
	
	// NOTE: ensure source exist
	if (source == nullptr)
	{
		D_Argument = false;
		return Msg(false, errorFileMessage, nullptr);
	}

	string sourcePath;
	auto path = objects.back().m_path.m_pathQueue;
	while (!path.empty())
	{
		sourcePath = sourcePath + "\\" + path.front();
		path.pop();
	}
	// cout << "TEST: source path:" << sourcePath.substr(1);

	// NOTE: get target
	auto target = objects.front().m_file;
	auto targetName = objects.front().m_path.m_pathQueue.back();
	auto targetParent = static_cast<DirectoryFile*>(objects.front().m_fileParent);

	// NOTE: ensure target is not exist
	if (target != nullptr)
	{
		D_Argument = false;
		return Msg(false, errorExistMessage, nullptr);
	}
	else if(targetParent == nullptr)
	{
		D_Argument = false;
		return Msg(false, errorDirMessage, nullptr);
	}
	
	// NOTE: create link file 
	if (D_Argument)
	{
		auto* file = new SymbolDirectoryFile(targetName, static_cast<DirectoryFile*>(source));
		targetParent->addChild(file);
	}
	else
	{
		auto* file = new SymbolGeneralFile(targetName, static_cast<GeneralFile*>(source));
		targetParent->addChild(file);
	}

	D_Argument = false;
	return Msg();
}