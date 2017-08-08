#pragma once

#include <string>
#include <vector>

#include "Path.h"
#include "DirectoryFile.h"

class ObjectGenerator
{
public:
	ObjectGenerator(const string& objectStr, DirectoryFile* currentDirectory);
	~ObjectGenerator();

	vector<string> getObjects();

private:
	vector<string> m_objects;
};

