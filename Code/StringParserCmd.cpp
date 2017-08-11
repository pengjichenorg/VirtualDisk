#include "stdafx.h"
#include "StringParserCmd.h"

StringParserCmd::StringParserCmd(const queue<string>& strings, stack<File*> currentDirectory) : m_strings(strings)
{
	m_currentDirectory = currentDirectory;
}

StringParserCmd::~StringParserCmd()
{

}
