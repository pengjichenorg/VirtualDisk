#include "stdafx.h"
#include "VDos.h"

stack<string> VDos::_path = stack<string>();

VDos::VDos()
{
	m_stringParser = new StringParser();
	m_commandFactory = new CommandFactory();
	m_currentDirectory = static_cast<DirectoryFile*>(DiskSystem::getInstance()->getRootDirectory()->search(initPartition)->second);
	VDos::_path.push(initPartition);
}

VDos::~VDos()
{
	if (m_stringParser != nullptr)
	{
		delete m_stringParser;
	}
	m_stringParser = nullptr;
	if (m_commandFactory != nullptr)
	{
		delete m_commandFactory;
	}
	m_commandFactory = nullptr;
}

void VDos::run()
{
	bool result = true;
	while (result)
	{
		// NOTE: output current path
		cout << getCurrentPath();

		// NOTE: input command
		std::string inputString;
		std::cin.sync(); // clear buffer
		std::getline(std::cin, inputString);

		// NOTE: for input enter
		if (inputString.empty())
		{
			continue;
		}

		// NOTE: exit process here
		auto str = inputString;
		transform(str.begin(), str.end(), str.begin(), tolower);
		if (str.compare("exit") == 0)
		{
			result = false;
			break;
		}

		// NOTE: parse string
		m_stringParser->parse(inputString, m_currentDirectory);

		// NOTE: get command
		auto command = m_commandFactory->createCommand(m_stringParser->getCommand());

		// NOTE: execute command
		auto msg = command(m_stringParser->getObjects());

		if (msg.m_successful)
		{
			// cout << "TEST: execute command successfully" << endl;
			if (msg.m_directoryFile != nullptr)
			{
				m_currentDirectory = msg.m_directoryFile;
			}
			// NOTE: for 'cd'
			if (m_stringParser->getCommand().compare("cd") == 0 && msg.m_msg.compare(".") == 0)
			{
				string path;
				auto dir = m_currentDirectory;
				while (dir != DiskSystem::getInstance()->getRootDirectory()) {
					if (path.empty())
					{
						path = dir->getName();
					}
					else
					{
						path = dir->getName() + "\\" + path;
					}
					dir = dir->getParent();
				}
				cout << path << "\n" << endl;
			}
		}
		else
		{
			// cout << "TEST: execute command failed" << endl;
			cout << msg.m_msg << endl;
		}

		// NOTE: clear string parser
		m_stringParser->clear();

		// cout << "TEST: while end" << endl;
	}

	// cout << "TEST: run end" << endl;
}

void VDos::setCurrentDirectory(DirectoryFile* currentDirectory)
{
	m_currentDirectory = currentDirectory;
}

DirectoryFile* VDos::getCurrentDirectory()
{
	return m_currentDirectory;
}

string VDos::getCurrentPath()
{
	string currentPath;
	auto pathCopy = VDos::_path;
	// cout << "TEST: _path size:" << _path.size() << endl;
	while (!pathCopy.empty())
	{
		// cout << "TEST:" << pathCopy.top() << endl;
		currentPath = pathCopy.top() + "\\" + currentPath;
		pathCopy.pop();
	}
	if (_path.size() != 1)
	{
		if (currentPath.empty())
		{
			// cout << "TEST: current path is empty" << endl;
			// NOTE: for cd \E1LL after rd /s E1
			currentPath = initPartition + "\\";

		}
		else
		{
			currentPath.pop_back();
		}
	}
	currentPath = currentPath + ">";
	return currentPath;
}
