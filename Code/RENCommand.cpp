#include "stdafx.h"
#include "CommandFactory.h"
#include "ErrorMessage.h"

#include <cassert>

Msg RENCommand(queue<Object> objects)
{
	if (objects.size() != 2)
	{
		return Msg(false, errorSyntaxMessage, stack<File*>());
	}

	// NOTE: get source file
	auto sourceFile = objects.front().m_currentDirectory.top();
	auto currentDirectoryCopy = objects.front().m_currentDirectory;
	// NOTE: for only C: in currentDirectory
	if (currentDirectoryCopy.size() != 1)
	{
		currentDirectoryCopy.pop();
	}
	auto sourceFileParent = currentDirectoryCopy.top();
	auto sourceFileName = objects.front().m_currentDirectory.top()->getName();
	auto sourceFileType = objects.front().m_currentDirectory.top()->getType();

	// NOTE: ensure source file could be found
	if (sourceFile == nullptr)
	{
		return Msg(false, errorFileMessage, stack<File*>());
	}

	objects.pop();

	// NOTE: get target file
	auto targetFile = objects.front().m_currentDirectory.top();
	auto targetName = objects.front().m_path.m_pathQueue.back();

	// NOTE: ensure target file doesnt exist
	if (targetFile != nullptr)
	{
		return Msg(false, errorExistMessage, stack<File*>());
	}

	// NOTE: change source file name
	sourceFile->setName(targetName);

	// NOTE: change rb-tree
	auto it = static_cast<DirectoryFile*>(sourceFileParent)->search(sourceFileName, sourceFileType);
	static_cast<DirectoryFile*>(sourceFileParent)->getChildren().erase(it);
	static_cast<DirectoryFile*>(sourceFileParent)->getChildren().emplace(targetName, sourceFile);

	return Msg();
}