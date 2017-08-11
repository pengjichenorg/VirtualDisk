#pragma once

#include "Path.h"
#include "DirectoryFile.h"

#include <vector>
#include <stack>

class ObjectGenerator
{
public:
	ObjectGenerator(const string& objectStr, stack<File*> currentDirectory);
	~ObjectGenerator();

	vector<string> getObjects();

private:
	vector<string> m_objects;
};

