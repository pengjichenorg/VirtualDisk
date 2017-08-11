#pragma once

#include "Object.h"

#include <queue>
#include <stack>

using namespace std;

class StringParserCmd
{
public:
	StringParserCmd(const queue<string>& strings, stack<File*> currentDirectory);
	virtual ~StringParserCmd();
	virtual queue<Object> getObjects() = 0;
protected:
	queue<string> m_strings;
	stack<File*> m_currentDirectory;
};

