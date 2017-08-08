#pragma once

#include <string>
#include <vector>

#include "Path.h"
#include "DirectoryFile.h"

class Object
{
public:
	Object(const Path& path, vector<string> arguments, DirectoryFile* directory);
	
	Path m_path;
	vector<string> m_arguments;
	File* m_file = nullptr;
	File* m_fileParent = nullptr;
	
	static bool createMode;
};

