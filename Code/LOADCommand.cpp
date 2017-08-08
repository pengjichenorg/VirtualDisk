#include "stdafx.h"
#include "CommandFactory.h"
#include "ErrorMessage.h"
#include "DiskSystem.h"

#include "GeneralFile.h"
#include "DirectoryFile.h"
#include "SymbolGeneralFile.h"
#include "SymbolDirectoryFile.h"

#include "tinyxml2.h"

#include <cassert>

using namespace tinyxml2;

DirectoryFile* TraverseXML(XMLElement* element);

static XMLDocument *pDoc = nullptr;

GeneralFile* createGF(XMLElement* element);

DirectoryFile* createDF(XMLElement* element);

SymbolGeneralFile* createSGF(XMLElement* element);

SymbolDirectoryFile* createSDF(XMLElement* element);

Msg LOADCommand(queue<Object> objects)
{
	if (objects.size() != 1)
	{
		return Msg(false, errorSyntaxMessage, nullptr);
	}

	auto path = objects.front().m_path.m_pathQueue;
	string pathString;
	while (!path.empty())
	{
		pathString = pathString + "\\" + path.front();
		path.pop();
	}

	// pathString = "@\\X:\\vd.xml";

	// cout << "TEST: path string:" << pathString.substr(2) << endl;

	DirectoryFile* root = nullptr;

	// NOTE: create xml doc
	// cout << "TEST: create xml doc" << endl;
	pDoc = new XMLDocument();
		
	XMLError errorId = pDoc->LoadFile(pathString.substr(2).c_str());

	if (errorId != 0)
	{
		// NOTE: xml format error
		return Msg(false, "xml format error", nullptr);
	}

	XMLElement* rootElement = pDoc->RootElement();

	// cout << "TEST: root element:" << rootElement->Value() << endl;

	root = TraverseXML(rootElement);

	// NOTE: change root
	DiskSystem::getInstance()->setRootDirectory(root);

	// NOTE: for test
	queue<File*> fileQueue;
	fileQueue.push(root);
	while (!fileQueue.empty())
	{
		// cout << "TEST: name:" << fileQueue.front()->getName() << endl;
		if (fileQueue.front()->getType() == FileType::directoryFile)
		{
			for (auto it = static_cast<DirectoryFile*>(fileQueue.front())->getChildren().begin();
				it != static_cast<DirectoryFile*>(fileQueue.front())->getChildren().end(); it++)
			{
				// cout << "TEST: child:" << it->first << endl;
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

	return Msg(true, "", static_cast<DirectoryFile*>(root->search(initPartition)->second));
}

DirectoryFile* TraverseXML(XMLElement* element)
{
	DirectoryFile* father = createDF(element);
	// cout << "TEST: create father:" << element->FirstChildElement("name")->GetText() << endl;
	if (element->FirstChildElement("File") == nullptr)
	{
		// cout << "TEST: empty dir" << endl;
		return father;
	}
	else
	{
		for (auto directory = element->FirstChildElement("File"); directory != nullptr; directory = directory->NextSiblingElement("File"))
		{
			// cout << "TEST: child:" << directory->FirstChildElement("name")->GetText() << endl;
			// NOTE: create xml element as fater's children
			File* child = nullptr;
			switch (atoi(directory->FirstChildElement("type")->GetText()))
			{
			case FileType::generalFile:
			{
				child = createGF(directory);
				break;
			}
			case FileType::directoryFile:
			{
				child = TraverseXML(directory);
				break;
			}
			case FileType::symlink:
			{
				child = createSGF(directory);
				break;
			}
			case FileType::symlinkd:
			{
				child = createSDF(directory);
				break;
			}
			}
			// cout << "TEST: child:" << child->getName() << endl;
			// cout << "TEST: father:" << father->getName() << endl;
			father->addChild(child);
		}
		return father;
	}
}

GeneralFile* createGF(XMLElement* element)
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
			// cout << "TEST: " << file->GetText() << endl;
		}
		else
		{
			infos.push_back(" ");
			// cout << "TEST: empty text" << endl;
		}
		file = file->NextSiblingElement();
	}

	return new GeneralFile(infos);
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
			// cout << "TEST: " << file->GetText() << endl;
		}		
		else
		{
			infos.push_back(" ");
			// cout << "TEST: empty text" << endl;
		}
		file = file->NextSiblingElement();
	}

	// cout << "TEST: here" << endl;

	return new DirectoryFile(infos);
}

SymbolGeneralFile* createSGF(XMLElement* element)
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
			// cout << "TEST: " << file->GetText() << endl;
		}
		else
		{
			infos.push_back(" ");
			// cout << "TEST: empty text" << endl;
		}
		file = file->NextSiblingElement();
	}

	return new SymbolGeneralFile(infos);
}

SymbolDirectoryFile* createSDF(XMLElement* element)
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
			// cout << "TEST: " << file->GetText() << endl;
		}
		else
		{
			infos.push_back(" ");
			// cout << "TEST: empty text" << endl;
		}
		file = file->NextSiblingElement();
	}

	return new SymbolDirectoryFile(infos);
}