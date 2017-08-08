#include "stdafx.h"
#include "Msg.h"

Msg::Msg(bool successful, string msg, DirectoryFile* directoryFile) : m_msg(msg)
{
	m_successful = successful;
	m_directoryFile = directoryFile;
}

Msg::~Msg()
{
	m_directoryFile = nullptr;
}
