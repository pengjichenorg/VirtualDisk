#pragma once
#include "StringParserCmd.h"
class StringParserCmd1 :
	public StringParserCmd
{
public:
	StringParserCmd1(const queue<string>& strings, DirectoryFile* currentDirectory);
	~StringParserCmd1();
	virtual queue<Object> getObjects() override;
};

