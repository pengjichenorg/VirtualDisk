#pragma once
#include "StringParserCmd.h"

class StringParserCmd0 :
	public StringParserCmd
{
public:
	StringParserCmd0(const queue<string>& strings, DirectoryFile * currentDirectory);
	~StringParserCmd0();
	virtual queue<Object> getObjects() override;
};

