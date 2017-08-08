#include "stdafx.h"
#include "CommandFactory.h"
#include "ErrorMessage.h"

Msg MDCommand(queue<Object> objects)
{
	if (objects.empty())
	{
		// cout << "TEST: objects is empty" << endl;
		return Msg(false, errorSyntaxMessage, nullptr);
	}

	while (!objects.empty())
	{
		// NOTE: if object.m_file is nullptr, output errorExistMessage
		if (objects.front().m_file == nullptr)
		{
			return Msg(false, errorExistMessage, nullptr);
		}

		objects.pop();
	}

	return Msg();
}