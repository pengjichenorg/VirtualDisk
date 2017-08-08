#pragma once
#include "StringParserCmd.h"
class StringParserCmdN :
	public StringParserCmd
{
public:
	StringParserCmdN(const queue<string>& strings, DirectoryFile* currentDirectory);
	~StringParserCmdN();
	virtual queue<Object> getObjects() override;
};

