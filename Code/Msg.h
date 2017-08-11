#pragma once

#include "DirectoryFile.h"

#include <stack>

using namespace std;

class Msg
{
public:
	Msg(bool successful = false, string msg = "", stack<File*> directoryFile = stack<File*>());
	~Msg();

	bool m_successful;
	string m_msg;
	stack<File*> m_directoryFile;
};

