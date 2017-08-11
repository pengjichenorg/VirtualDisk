#pragma once

#include "StringParserCmd.h"

#include <queue>
#include <stack>

class StringParserCmdN :
	public StringParserCmd
{
public:
	StringParserCmdN(const queue<string>& strings, stack<File*> currentDirectory);
	~StringParserCmdN();
	virtual queue<Object> getObjects() override;
};

