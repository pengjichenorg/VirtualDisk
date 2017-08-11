#pragma once

#include "Path.h"
#include "DiskSystem.h"
#include "StringParser.h"
#include "DirectoryFile.h"
#include "CommandFactory.h"

#include <stack>

using namespace std;

class VDos
{
public:
	VDos();
	~VDos();
	void run();
	void setCurrentDirectory(stack<File*> currentDirectory);
	stack<File*> getCurrentDirectory();
	string getCurrentPath();

	// NOTE: for symbol directory path
	static stack<string> _path;

private:
	stack<File*> m_currentDirectory;
	StringParser* m_stringParser = nullptr;
	CommandFactory* m_commandFactory = nullptr;

};

