#pragma once

#include "StringParserCmd.h"

#include <queue>
#include <stack>

class StringParserCmd2 :
	public StringParserCmd
{
public:
	StringParserCmd2(const queue<string>& strings, stack<File*> currentDirectory);
	~StringParserCmd2();
	virtual queue<Object> getObjects() override;
};

