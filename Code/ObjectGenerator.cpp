#include "stdafx.h"
#include "ObjectGenerator.h"

#include <regex>
#include <stack>

ObjectGenerator::ObjectGenerator(const string& objectStr, stack<File*> currentDirectory)
{
	// NOTE£ºwildcard process
	// NOTE: currentDirectory top is nullptr, pop it first
	
	// NOTE: get regex
	// NOTE: * and ? cannot in same path
	regex regex_str;
	smatch sm_str;
	string wildcard;
	// NOTE: has *
	if (objectStr.find('*') != string::npos)
	{
		wildcard = "*";
	}
	// NOTE: has ?
	else
	{
		wildcard = "?";
	}

	// NOTE: change to lower and use regex search
	auto lowerObjectStr = objectStr;
	transform(lowerObjectStr.begin(), lowerObjectStr.end(), lowerObjectStr.begin(), ::tolower);

	auto pos = objectStr.find(wildcard);
	// NOTE: start position
	if (pos == 0)
	{
		string str = string("^") + string("\\S") + wildcard + lowerObjectStr.substr(pos + 1) + string("$");
		regex_str = str;
	}
	// NOTE: end position
	else if (pos == (lowerObjectStr.size() - 1))
	{
		string str = string("^") + lowerObjectStr.substr(0, pos) + string("\\S") + wildcard + string("$");
		regex_str = str;
	}
	// NOTE: middle position
	else
	{
		string str = string("^") + lowerObjectStr.substr(0, pos) + string("[\\S]") + wildcard + lowerObjectStr.substr(pos + 1) + string("$");
		regex_str = str;
	}

	// NOTE: check if it could be access
	try
	{
		currentDirectory.top()->getName();
	}
	catch(std::bad_alloc)
	{
		m_objects.clear();
		return;
	}

	// NOTE: get files
	for (auto it = static_cast<DirectoryFile*>(currentDirectory.top())->getChildren().begin(); it != static_cast<DirectoryFile*>(currentDirectory.top())->getChildren().end(); it++)
	{
		string name = it->first;
		// NOTE: step over . and ..
		if (it->first.compare(".") == 0 || it->first.compare("..") == 0)
		{
			continue;
		}

		auto lowerName = name;
		transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

		if (regex_search(lowerName, sm_str, regex_str))
		{
			for (auto x = sm_str.begin(); x != sm_str.end(); x++)
			{
				m_objects.push_back(x->str());
			}
		}
		name = sm_str.suffix().str();
	}
}

ObjectGenerator::~ObjectGenerator()
{
}

vector<string> ObjectGenerator::getObjects()
{
	return m_objects;
}
