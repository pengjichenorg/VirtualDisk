#pragma once

#include "Path.h"
#include "DiskSystem.h"
#include "StringParser.h"
#include "DirectoryFile.h"
#include "CommandFactory.h"

#include <stack>
#include <string>

using namespace std;

class VDos
{
public:
	VDos();
	~VDos();
	void run();
	void setCurrentDirectory(DirectoryFile* currentDirectory);
	DirectoryFile* getCurrentDirectory();
	string getCurrentPath();

	// NOTE: for symbol directory path
	static stack<string> _path;

private:
	DirectoryFile* m_currentDirectory = nullptr;
	StringParser* m_stringParser = nullptr;
	CommandFactory* m_commandFactory = nullptr;

};

