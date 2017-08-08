#include "stdafx.h"
#include "CommandFactory.h"


CommandFactory::CommandFactory()
{
}


CommandFactory::~CommandFactory()
{
}

function<Msg(queue<Object> objects)>& CommandFactory::createCommand(const string& command)
{
	auto it = m_commandTable.find(command);
	// cout << "TEST: command:" << command << endl;
	if (it == m_commandTable.end()) {
		// cout << "TEST: command lost" << endl;
		return error;
	}
	// cout << "TEST: command found:" << endl;
	return it->second;
}