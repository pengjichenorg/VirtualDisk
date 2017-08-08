#include "stdafx.h"
#include "CommandFactory.h"
#include "ErrorMessage.h"

Msg ERRORCommand(queue<Object> objects)
{
	return Msg(false, errorCmdMessage, nullptr);
}