#include "stdafx.h"
#include "ObjectGenerator.h"

#include <regex>

ObjectGenerator::ObjectGenerator(const string& objectStr, DirectoryFile * currentDirectory)
{
	// NOTE£ºwildcard process
	// cout << "TEST: wildcard process:" << objectStr << endl;
	// cout << "TEST: wildcard directory:" << currentDirectory->getName() << endl;

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

	auto pos = objectStr.find(wildcard);
	// NOTE: start position
	if (pos == 0)
	{
		string str = string("^") + string("\\S") + wildcard + objectStr.substr(pos + 1) + string("$");
		regex_str = str;
	}
	// NOTE: end position
	else if (pos == (objectStr.size() - 1))
	{
		string str = string("^") + objectStr.substr(0, pos) + string("\\S") + wildcard + string("$");
		regex_str = str;
	}
	// NOTE: middle position
	else
	{
		string str = string("^") + objectStr.substr(0, pos) + string("[\\S]") + wildcard + objectStr.substr(pos + 1) + string("$");
		regex_str = str;
	}

	// NOTE: get files
	for (auto it = currentDirectory->getChildren().begin(); it != currentDirectory->getChildren().end(); it++)
	{
		string name = it->first;
		// NOTE: step over . and ..
		if (it->first.compare(".") == 0 || it->first.compare("..") == 0)
		{
			continue;
		}
		if (regex_search(name, sm_str, regex_str))
		{
			for (auto x = sm_str.begin(); x != sm_str.end(); x++)
			{
				// cout << "TEST: get objectStr by wildcard:" << x->str() << endl;
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
