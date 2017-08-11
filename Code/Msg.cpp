#include "stdafx.h"
#include "Msg.h"

Msg::Msg(bool successful, string msg, stack<File*> directoryFile) : m_msg(msg), m_directoryFile(directoryFile)
{
	m_successful = successful;
}

Msg::~Msg()
{

}
