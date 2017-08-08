#pragma once

#include "Object.h"
#include "ObjectGenerator.h"

#include <queue>
#include <string>

using namespace std;

class StringParserCmd
{
public:
	StringParserCmd(const queue<string>& strings, DirectoryFile* currentDirectory);
	virtual ~StringParserCmd();
	virtual queue<Object> getObjects() = 0;
protected:
	queue<string> m_strings;
	DirectoryFile* m_currentDirectory;
};

