#pragma once

#include "StringParserCmd.h"

#include <queue>
#include <stack>

class StringParserCmd0 :
	public StringParserCmd
{
public:
	StringParserCmd0(const queue<string>& strings, stack<File*> currentDirectory);
	~StringParserCmd0();
	virtual queue<Object> getObjects() override;
};

