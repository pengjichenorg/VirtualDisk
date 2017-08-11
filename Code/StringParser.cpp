#include "stdafx.h"
#include "StringParser.h"

#include "StringParserCmd0.h"
#include "StringParserCmd1.h"
#include "StringParserCmd2.h"
#include "StringParserCmdN.h"

#include <regex>
#include <algorithm>

using namespace std;

StringParser::StringParser()
{
}

StringParser::~StringParser()
{
	if (m_stringParserCmd != nullptr)
	{
		delete m_stringParserCmd;
	}
	m_stringParserCmd = nullptr;
}

void StringParser::parse(const string & inputString, stack<File*> currentDirectory)
{
	string s = inputString;
	regex regex_str("\\S+");
	smatch sm_str;

	// NOTE: split inputString by blank character
	queue<string> strings;
	while (regex_search(s, sm_str, regex_str))
	{
		for (auto x = sm_str.begin(); x != sm_str.end(); x++)
		{
			auto str = x->str();
			strings.emplace(str);
		}
		s = sm_str.suffix().str();
	}

	// NOTE: empty input
	if (strings.empty())
	{
		// NOTE: input blank character, do nothing
		return;
	}

	// NOTE: get command
	m_command = strings.front();
	// NOTE: change Upper to Lower
	transform(m_command.begin(), m_command.end(), m_command.begin(), ::tolower);
	strings.pop();

	// NOTE: for createMode
	Object::createMode = false;
	if (m_command.compare("md") == 0)
	{
		Object::createMode = true;
	}

	// NOTE: for rdMode
	Object::rdMode = false;
	if (m_command.compare("rd") == 0)
	{
		Object::rdMode = true;
	}

	// NOTE: get StringParserCmd
	if (find(cmd0.begin(), cmd0.end(), m_command) != cmd0.end())
	{
		m_stringParserCmd = new StringParserCmd0(strings, currentDirectory);
	}
	else if (find(cmd1.begin(), cmd1.end(), m_command) != cmd1.end())
	{
		m_stringParserCmd = new StringParserCmd1(strings, currentDirectory);
	}
	else if (find(cmd2.begin(), cmd2.end(), m_command) != cmd2.end())
	{
		m_stringParserCmd = new StringParserCmd2(strings, currentDirectory);
	}
	else if (find(cmdN.begin(), cmdN.end(), m_command) != cmdN.end())
	{
		m_stringParserCmd = new StringParserCmdN(strings, currentDirectory);
	}
	else
	{
		m_command = "error";
		// NOTE: error command
		m_stringParserCmd = new StringParserCmd0(strings, currentDirectory);
	}
	m_currentDirectory = currentDirectory;
}

string StringParser::getCommand() const
{
	return m_command;
}

queue<Object> StringParser::getObjects() const
{
	auto objects = m_stringParserCmd->getObjects();

	// NOTE: for 'dir', cause dir default execute 'dir .'
	if (m_command.compare("dir") == 0 && objects.empty())
	{
		queue<string> arguments;
		objects.emplace(string("."), arguments, m_currentDirectory);
	}

	return objects;
}

void StringParser::clear()
{
	// NOTE: clear command
	m_command.clear();

	// NOTE: clear object
	while (!m_objects.empty())
	{
		m_objects.pop();
	}

	// NOTE: clear StringParserCmd
	if (m_stringParserCmd != nullptr)
	{
		delete m_stringParserCmd;
	}
	m_stringParserCmd = nullptr;
}
