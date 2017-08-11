#pragma once

#include "StringParserCmd.h"

#include <queue>
#include <stack>

class StringParserCmd1 :
	public StringParserCmd
{
public:
	StringParserCmd1(const queue<string>& strings, stack<File*> currentDirectory);
	~StringParserCmd1();
	virtual queue<Object> getObjects() override;
};

