#pragma once

#include <string>

#include "DirectoryFile.h"

using namespace std;

class Msg
{
public:
	Msg(bool successful = false, string msg = "", DirectoryFile* directoryFile = nullptr);
	~Msg();

	bool m_successful;
	string m_msg;
	DirectoryFile* m_directoryFile = nullptr;
};

