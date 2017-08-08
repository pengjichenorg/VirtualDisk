#include "stdafx.h"
#include "StringParserCmd0.h"

StringParserCmd0::StringParserCmd0(const queue<string>& strings, DirectoryFile* currentDirectory) : StringParserCmd(strings, currentDirectory)
{
}

StringParserCmd0::~StringParserCmd0()
{
}

queue<Object> StringParserCmd0::getObjects()
{
	// cout << "TEST: return an empty queue with no object" << endl;
	// NOTE: 0 cmd dont do any thing
	return queue<Object>();
}
