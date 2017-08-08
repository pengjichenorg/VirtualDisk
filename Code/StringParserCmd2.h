#pragma once
#include "StringParserCmd.h"
class StringParserCmd2 :
	public StringParserCmd
{
public:
	StringParserCmd2(const queue<string>& strings, DirectoryFile* currentDirectory);
	~StringParserCmd2();
	virtual queue<Object> getObjects() override;
};

