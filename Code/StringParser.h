#pragma once

#include "StringParserCmd.h"
#include "Object.h"

#include <queue>

using namespace std;

class StringParser
{
public:
	StringParser();
	~StringParser();
	void parse(const string & inputString, DirectoryFile * currentDirectory);
	string getCommand() const;
	queue<Object> getObjects() const;
	void clear();

private:
	string m_command;
	queue<Object> m_objects;
	DirectoryFile* m_currentDirectory = nullptr;
	StringParserCmd* m_stringParserCmd = nullptr;
	const vector<string> cmd0 = {"cls"};
	const vector<string> cmd1 = {"cd", "save", "load"};
	const vector<string> cmd2 = {"ren", "copy", "move", "mklink"};
	const vector<string> cmdN = {"dir", "md", "rd", "del", "touch"};
};

