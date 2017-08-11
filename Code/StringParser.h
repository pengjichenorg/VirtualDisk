#pragma once

#include "StringParserCmd.h"
#include "Object.h"

#include <queue>
#include <stack>
#include <vector>

using namespace std;

class StringParser
{
public:
	StringParser();
	~StringParser();
	void parse(const string & inputString, stack<File*> currentDirectory);
	string getCommand() const;
	queue<Object> getObjects() const;
	void clear();

private:
	string m_command;
	queue<Object> m_objects;
	stack<File*> m_currentDirectory;
	StringParserCmd* m_stringParserCmd = nullptr;
	const vector<string> cmd0 = { "cls" };
	const vector<string> cmd1 = { "cd", "save", "load" };
	const vector<string> cmd2 = { "ren", "copy", "move", "mklink" };
	const vector<string> cmdN = { "dir", "md", "rd", "del", "touch" };
};

