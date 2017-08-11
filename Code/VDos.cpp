#include "stdafx.h"
#include "VDos.h"

stack<string> VDos::_path = stack<string>();

VDos::VDos()
{
	m_stringParser = new StringParser();
	m_commandFactory = new CommandFactory();
	m_currentDirectory.push(static_cast<DirectoryFile*>(DiskSystem::getInstance()->getRootDirectory()->search(initPartition, FileType::dirFile)->second));
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
			// NOTE: if stack's top is not nullptr
			if (!msg.m_directoryFile.empty())
			{
				if (msg.m_directoryFile.top() != nullptr)
				{
					// NOTE: then change current
					m_currentDirectory = msg.m_directoryFile;
				}
			}
			else
			{
				// NOTE: for 'cd'
				if (m_stringParser->getCommand().compare("cd") == 0 && msg.m_msg.compare(".") == 0)
				{
					string path;
					auto currentDirectoryCopy = m_currentDirectory;

					while (!currentDirectoryCopy.empty())
					{
						if (path.empty())
						{
							path = currentDirectoryCopy.top()->getName();
						}
						else
						{
							path = currentDirectoryCopy.top()->getName() + "\\" + path;
						}
						currentDirectoryCopy.pop();
					}
					cout << path << "\n" << endl;
				}
			}

		}
		else
		{
			cout << msg.m_msg << endl;
		}

		// NOTE: clear string parser
		m_stringParser->clear();
	}
}

void VDos::setCurrentDirectory(stack<File*> currentDirectory)
{
	m_currentDirectory = currentDirectory;
}

stack<File*> VDos::getCurrentDirectory()
{
	return m_currentDirectory;
}

string VDos::getCurrentPath()
{
	string currentPath;

	auto pathCopy = VDos::_path;

	while (!pathCopy.empty())
	{
		currentPath = pathCopy.top() + "\\" + currentPath;
		pathCopy.pop();
	}

	if (VDos::_path.size() != 1)
	{
		if (currentPath.empty())
		{
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
