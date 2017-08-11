#include "stdafx.h"
#include "Object.h"
#include "DiskSystem.h"

#include "BinaryFile.h"
#include "DirectoryFile.h"
#include "SymlinkFile.h"
#include "SymlinkdFile.h"

// NOTE: |                           Object Condition                            |
// NOTE: |-----------------------------------------------------------------------|
// NOTE: |    path    |  arguments  |     directory    |  file   |  file parent  |
// NOTE: |------------+-------------+------------------+---------+---------------|
// NOTE: |   empty    |   whatever  |     whatever     | nullptr |    nullptr    |
// NOTE: |   size 1   |  arguments  |     directory    |  object | object parent |
// NOTE: | . in path  |  arguments  |     directory    |    .    |   . parent    |
// NOTE: | .. in path |  arguments  | directory parent |    ..   |   .. parent   |
// NOTE: |  symlinkd  |  arguments  |     directory    |  object | object parent |

bool Object::createMode = false;

bool Object::rdMode = false;

Object::Object(const Path& path, queue<string> arguments, stack<File*> currentDirectory) : m_path(path), m_arguments(arguments), m_currentDirectory(currentDirectory)
{
	// NOTE: check m_currentDirectory
	if (!m_currentDirectory.empty())
	{
		auto TEST_DIR = m_currentDirectory;
		while (!TEST_DIR.empty())
		{
			TEST_DIR.pop();
		}

		if (m_path.m_pathQueue.empty())
		{
			// NOTE: clear m_currentDirectory or push nullptr?
			m_currentDirectory.push(nullptr);
			m_currentDirectory.push(nullptr);
		}
		else
		{
			auto pathQueue = m_path.m_pathQueue;

			// NOTE: deal each path
			while (!pathQueue.empty())
			{
				// NOTE: get path str
				auto _path = pathQueue.front();
				// NOTE: upward
				if (_path.compare("..") == 0)
				{
					// NOTE: only left the last one
					if (m_currentDirectory.size() != 1)
					{
						m_currentDirectory.pop();
					}
				}
				// NOTE: no change
				else if (_path.compare(".") == 0)
				{

				}
				// NOTE: downward, means path could be found or create
				// NOTE: path is a general file
				// NOTE: path is a symbol general file
				// NOTE: path is a directory file or
				// NOTE: path is a symbol directory file
				else
				{
					// NOTE: check if it could be access
					try
					{
						m_currentDirectory.top()->getName();
					}
					catch (std::bad_alloc)
					{
						m_currentDirectory.top() = nullptr;
						return;
					}
					// NOTE: find directory file
					if (static_cast<DirectoryFile*>(m_currentDirectory.top())->search(_path, FileType::dirFile) != static_cast<DirectoryFile*>(m_currentDirectory.top())->getChildren().end())
					{
						auto dir = static_cast<DirectoryFile*>(m_currentDirectory.top())->search(_path, FileType::dirFile)->second;
						m_currentDirectory.push(static_cast<DirectoryFile*>(dir));
					}
					// NOTE: find symbol directory file
					else if (static_cast<DirectoryFile*>(m_currentDirectory.top())->search(_path, FileType::symlinkd) != static_cast<DirectoryFile*>(m_currentDirectory.top())->getChildren().end())
					{
						// NOTE: treat SymlinkdFile as DirectoryFile
						auto dir = static_cast<DirectoryFile*>(m_currentDirectory.top())->search(_path, FileType::symlinkd)->second;
						m_currentDirectory.push(static_cast<DirectoryFile*>(dir));
					}
					// NOTE: path is a general file or symbol general file
					else if (static_cast<DirectoryFile*>(m_currentDirectory.top())->search(_path, FileType::binFile) != static_cast<DirectoryFile*>(m_currentDirectory.top())->getChildren().end() || static_cast<DirectoryFile*>(m_currentDirectory.top())->search(_path, FileType::symlink) != static_cast<DirectoryFile*>(m_currentDirectory.top())->getChildren().end())
					{
						auto file = static_cast<DirectoryFile*>(m_currentDirectory.top())->search(_path, FileType::binFile)->second;
						// NOTE: file parent has been set
						if (file == nullptr)
						{
							// NOTE: file parent has been set
							file = static_cast<DirectoryFile*>(m_currentDirectory.top())->search(_path, FileType::symlink)->second;
						}

						m_currentDirectory.push(file);
						// NOTE: binary file or symlink file couldn't be in path, so I return
						// IDEA: if pathQueue.size != 1, I can set nullptr here cause a binary file or symlink file is in path
						return;
					}
					// NOTE: path doesn't exist
					else
					{
						if (createMode)
						{
							// NOTE: check name
							if (_path.find('/') != string::npos ||
								_path.find('\\') != string::npos ||
								_path.find(':') != string::npos ||
								_path.find('*') != string::npos ||
								_path.find('?') != string::npos ||
								_path.find('"') != string::npos ||
								_path.find('<') != string::npos ||
								_path.find('>') != string::npos ||
								_path.find('|') != string::npos)
							{
								// NOTE: clear current directory
								while (!m_currentDirectory.empty())
								{
									m_currentDirectory.pop();
								}
								break;
							}
							else
							{
								auto childDir = new DirectoryFile(_path);
								static_cast<DirectoryFile*>(m_currentDirectory.top())->addChild(childDir);
								m_currentDirectory.push(childDir);
							}
						}
						else
						{
							// NOTE: push a nullptr
							m_currentDirectory.push(nullptr);
							break;
						}
					}
				}
				pathQueue.pop();
			}
		}
	}
	else
	{
		// NOTE: output error message here?
	}
}