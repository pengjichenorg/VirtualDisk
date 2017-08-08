#pragma once

#include <string>
#include <vector>
#include <queue>
#include <functional>
#include <unordered_map>

#include "Msg.h"
#include "Object.h"
#include "DirectoryFile.h"

using namespace std;

Msg CDCommand(queue<Object> objects);
Msg CLSCommand(queue<Object> objects);
Msg COPYCommand(queue<Object> objects);
Msg DIRCommand(queue<Object> objects);
Msg DELCommand(queue<Object> objects);
Msg ERRORCommand(queue<Object> objects);
Msg LOADCommand(queue<Object> objects);
Msg MDCommand(queue<Object> objects);
Msg MKLINKCommand(queue<Object> objects);
Msg MOVECommand(queue<Object> objects);
Msg RDCommand(queue<Object> objects);
Msg RENCommand(queue<Object> objects);
Msg SAVECommand(queue<Object> objects);
Msg TOUCHCommand(queue<Object> objects);

class CommandFactory
{
public:
	CommandFactory();
	~CommandFactory();

	function<Msg(queue<Object> objects)>& createCommand(const string& command);

private:
	unordered_map<string, function<Msg(queue<Object> objects)>> m_commandTable = {
		{ "dir", &DIRCommand },
		{ "md", &MDCommand },
		{ "rd", &RDCommand },
		{ "cd", &CDCommand },
		{ "del", &DELCommand },
		{ "copy", &COPYCommand},
		{ "ren", &RENCommand },
		{ "move", &MOVECommand },
		{ "mklink", &MKLINKCommand },
		{ "save", &SAVECommand},
		{ "load", &LOADCommand},
		{ "cls", &CLSCommand },
		{ "touch", &TOUCHCommand }
	};
	function<Msg(queue<Object> objects)> error = &ERRORCommand;
};