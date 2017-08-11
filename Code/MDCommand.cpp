#include "stdafx.h"
#include "CommandFactory.h"
#include "ErrorMessage.h"

#include <cassert>

Msg MDCommand(queue<Object> objects)
{
	if (objects.empty())
	{
		return Msg(false, errorSyntaxMessage, stack<File*>());
	}

	while (!objects.empty())
	{
		if (!objects.front().m_currentDirectory.empty())
		{
			// NOTE: if top is nullptr, output errorExistMessage
			if (objects.front().m_currentDirectory.top() == nullptr)
			{
				return Msg(false, errorExistMessage, objects.front().m_currentDirectory);
			}
		}
		else
		{
			// NOTE: is object currentDirectory is empty, output error invalid name message
			return Msg(false, errorNameMessage, stack<File*>());
		}
		objects.pop();
	}

	return Msg();
}