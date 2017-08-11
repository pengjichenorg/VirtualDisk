#include "stdafx.h"
#include "CommandFactory.h"

#include <queue>

CommandFactory::CommandFactory()
{
}

CommandFactory::~CommandFactory()
{
}

function<Msg(queue<Object> objects)>& CommandFactory::createCommand(const string& command)
{
	auto it = m_commandTable.find(command);
	if (it == m_commandTable.end())
	{
		return error;
	}
	return it->second;
}