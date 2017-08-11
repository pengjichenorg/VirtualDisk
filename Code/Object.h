#pragma once

#include "Path.h"
#include "DirectoryFile.h"

#include <queue>
#include <stack>

class Object
{
public:
	Object(const Path& path, queue<string> arguments, stack<File*> currentDirectory);

	Path m_path;
	queue<string> m_arguments;
	stack<File*> m_currentDirectory;

	// NOTE: for md command
	static bool createMode;

	// NOTE: for rd command
	static bool rdMode;

};

