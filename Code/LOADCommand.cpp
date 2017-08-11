#include "stdafx.h"
#include "CommandFactory.h"
#include "ErrorMessage.h"

#include "BinaryFile.h"
#include "DirectoryFile.h"
#include "SymlinkFile.h"
#include "SymlinkdFile.h"

#include "DiskSystem.h"

#include "tinyxml2.h"

#include <cassert>

using namespace tinyxml2;

DirectoryFile* TraverseXML(XMLElement* element);

static XMLDocument *pDoc = nullptr;

BinaryFile* createBF(XMLElement* element);

DirectoryFile* createDF(XMLElement* element);

SymlinkFile* createSBF(XMLElement* element);

SymlinkdFile* createSDF(XMLElement* element);

Msg LOADCommand(queue<Object> objects)
{
	if (objects.size() != 1)
	{
		return Msg(false, errorSyntaxMessage, stack<File*>());
	}

	auto path = objects.front().m_path.m_pathQueue;
	string pathString;
	while (!path.empty())
	{
		pathString = pathString + "\\" + path.front();
		path.pop();
	}

	DirectoryFile* root = nullptr;

	// NOTE: create xml doc
	pDoc = new XMLDocument();

	XMLError errorId = pDoc->LoadFile(pathString.substr(2).c_str());

	if (errorId != 0)
	{
		// NOTE: xml format error
		return Msg(false, "xml format error", stack<File*>());
	}

	XMLElement* rootElement = pDoc->RootElement();

	root = TraverseXML(rootElement);

	// NOTE: change root
	DiskSystem::getInstance()->setRootDirectory(root);

	// NOTE: for test
	queue<File*> fileQueue;
	fileQueue.push(root);
	while (!fileQueue.empty())
	{
		if (fileQueue.front()->getType() == FileType::dirFile)
		{
			for (auto it = static_cast<DirectoryFile*>(fileQueue.front())->getChildren().begin();
			it != static_cast<DirectoryFile*>(fileQueue.front())->getChildren().end(); it++)
			{
				if (it->first.compare(".") == 0 || it->first.compare("..") == 0)
				{
					continue;
				}
				fileQueue.push(it->second);
			}
		}
		fileQueue.pop();
	}

	delete pDoc;

	stack<File*> currentDirectory;

	currentDirectory.push(static_cast<DirectoryFile*>(root->search(initPartition, FileType::dirFile)->second));

	return Msg(true, "", currentDirectory);
}

DirectoryFile* TraverseXML(XMLElement* element)
{
	DirectoryFile* father = createDF(element);
	if (element->FirstChildElement("File") == nullptr)
	{
		return father;
	}
	else
	{
		for (auto directory = element->FirstChildElement("File"); directory != nullptr; directory = directory->NextSiblingElement("File"))
		{
			// NOTE: create xml element as fater's children
			File* child = nullptr;
			switch (atoi(directory->FirstChildElement("type")->GetText()))
			{
			case FileType::binFile:
			{
				child = createBF(directory);
				break;
			}
			case FileType::dirFile:
			{
				child = TraverseXML(directory);
				break;
			}
			case FileType::symlink:
			{
				child = createSDF(directory);
				break;
			}
			case FileType::symlinkd:
			{
				child = createSDF(directory);
				break;
			}
			}
			father->addChild(child);
		}
		return father;
	}
}

BinaryFile* createBF(XMLElement* element)
{
	assert(string(element->FirstChildElement("type")->GetText()).compare("1") == 0);

	// NOTE: load list
	// NOTE: date, time, size, name, type, info, parent, data
	vector<string> infos;

	XMLElement* file = element->FirstChildElement();
	for (auto index = 0; index != 8; index++)
	{
		// NOTE: ensure not empty
		if (file->GetText() != nullptr)
		{
			infos.push_back(file->GetText());
		}
		else
		{
			infos.push_back(" ");
		}
		file = file->NextSiblingElement();
	}

	return new BinaryFile(infos);
}

DirectoryFile* createDF(XMLElement* element)
{
	assert(string(element->FirstChildElement("type")->GetText()).compare("0") == 0);

	// NOTE: load list
	// NOTE: date, time, size, name, type, info, parent
	vector<string> infos;

	XMLElement* file = element->FirstChildElement();
	for (auto index = 0; index != 7; index++)
	{
		// NOTE: ensure not empty
		if (file->GetText() != nullptr)
		{
			infos.push_back(file->GetText());
		}
		else
		{
			infos.push_back(" ");
		}
		file = file->NextSiblingElement();
	}

	return new DirectoryFile(infos);
}

SymlinkFile* createSBF(XMLElement* element)
{
	assert(string(element->FirstChildElement("type")->GetText()).compare("2") == 0);

	// NOTE: load list
	// NOTE: date, time, size, name, type, info, parent, link
	vector<string> infos;

	XMLElement* file = element->FirstChildElement();
	for (auto index = 0; index != 8; index++)
	{
		// NOTE: ensure not empty
		if (file->GetText() != nullptr)
		{
			infos.push_back(file->GetText());
		}
		else
		{
			infos.push_back(" ");
		}
		file = file->NextSiblingElement();
	}

	return new SymlinkFile(infos);
}

SymlinkdFile* createSDF(XMLElement* element)
{
	assert(string(element->FirstChildElement("type")->GetText()).compare("3") == 0);

	// NOTE: load list
	// NOTE: date, time, size, name, type, info, parent, link
	vector<string> infos;

	XMLElement* file = element->FirstChildElement();
	for (auto index = 0; index != 8; index++)
	{
		// NOTE: ensure not empty
		if (file->GetText() != nullptr)
		{
			infos.push_back(file->GetText());
		}
		else
		{
			infos.push_back(" ");
		}
		file = file->NextSiblingElement();
	}

	return new SymlinkdFile(infos);
}