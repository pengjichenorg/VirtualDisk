#include "stdafx.h"
#include "CommandFactory.h"
#include "ErrorMessage.h"

Msg RENCommand(queue<Object> objects)
{
	if (objects.size() != 2)
	{
		// cout << "TEST: objects is empty" << endl;
		return Msg(false, errorSyntaxMessage, nullptr);
	}
	
	// NOTE: get source file
	auto sourceFile = objects.front().m_file;
	auto sourceFileParent = objects.front().m_fileParent;
	auto sourceFileName = objects.front().m_file->getName();

	// NOTE: ensure source file could be found
	if (sourceFile == nullptr)
	{
		return Msg(false, errorFileMessage, nullptr);
	}

	// cout << "TEST: source file:" << sourceFile->getName() << endl;

	objects.pop();

	// NOTE: get target file
	auto targetFile = objects.front().m_file;
	auto targetName = objects.front().m_path.m_pathQueue.back();

	// NOTE: ensure target file doesnt exist
	if (targetFile != nullptr)
	{
		return Msg(false, errorExistMessage, nullptr);
	}

	// cout << "TEST: target name:" << targetName << endl;

	// NOTE: change source file name
	sourceFile->setName(targetName);

	// NOTE: change rb-tree
	auto it = static_cast<DirectoryFile*>(sourceFileParent)->search(sourceFileName);
	static_cast<DirectoryFile*>(sourceFileParent)->getChildren().erase(it);
	static_cast<DirectoryFile*>(sourceFileParent)->getChildren().emplace(targetName, sourceFile);

	return Msg();
}