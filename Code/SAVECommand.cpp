#include "stdafx.h"
#include "CommandFactory.h"
#include "ErrorMessage.h"

#include "BinaryFile.h"
#include "DirectoryFile.h"
#include "SymlinkFile.h"
#include "SymlinkdFile.h"

#include "DiskSystem.h"

#include "tinyxml2.h"

#include <fstream>
#include <cassert>

using namespace tinyxml2;

XMLElement* TraverseDirectory(DirectoryFile* file);

static XMLDocument *pDoc = nullptr;

XMLElement* createGF(BinaryFile* file);

XMLElement* createDF(DirectoryFile* file);

XMLElement* createSGF(SymlinkFile* file);

XMLElement* createSDF(SymlinkdFile* file);

Msg SAVECommand(queue<Object> objects)
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

	auto root = DiskSystem::getInstance()->getRootDirectory();

	// NOTE: create xml doc
	pDoc = new XMLDocument();
	auto declaration = pDoc->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
	pDoc->LinkEndChild(declaration);

	pDoc->LinkEndChild(TraverseDirectory(root));

	pDoc->SaveFile(pathString.substr(2).c_str());

	delete pDoc;
	pDoc = nullptr;

	return Msg();
}

XMLElement* TraverseDirectory(DirectoryFile* file)
{
	XMLElement* father = createDF(file);
	if (file->getChildren().size() <= 2)
	{
		return father;
	}
	else
	{
		for (auto it = file->getChildren().begin(); it != file->getChildren().end(); it++)
		{
			if (it->first.compare(".") == 0 || it->first.compare("..") == 0)
			{
				continue;
			}
			else
			{
				// NOTE: create xml element as fater's children
				XMLElement* child = nullptr;
				switch (it->second->getType())
				{
				case FileType::binFile:
				{
					child = createGF(static_cast<BinaryFile*>(it->second));
					break;
				}
				case FileType::dirFile:
				{
					child = TraverseDirectory(static_cast<DirectoryFile*>(it->second));
					break;
				}
				case FileType::symlink:
				{
					child = createSGF(static_cast<SymlinkFile*>(it->second));
					break;
				}
				case FileType::symlinkd:
				{
					child = createSDF(static_cast<SymlinkdFile*>(it->second));
					break;
				}
				}
				father->LinkEndChild(child);
			}
		}
		return father;
	}
}

XMLElement* createGF(BinaryFile* file)
{
	// <GeneralFile>
	// <date>file date</date>
	// <time>file time</time>
	// <size>file size</size>
	// <name>file name</name>
	// <type>file type</type>
	// <info>file info</info>
	// <parent>file parent</parent>
	XMLElement* fileElement = pDoc->NewElement("File");

	XMLElement* dateElement = pDoc->NewElement("date");
	dateElement->LinkEndChild(pDoc->NewText(file->getDate().c_str()));
	fileElement->LinkEndChild(dateElement);

	XMLElement* timeElement = pDoc->NewElement("time");
	timeElement->LinkEndChild(pDoc->NewText(file->getTime().c_str()));
	fileElement->LinkEndChild(timeElement);

	XMLElement* sizeElement = pDoc->NewElement("size");
	sizeElement->LinkEndChild(pDoc->NewText(to_string(file->getSize()).c_str()));
	fileElement->LinkEndChild(sizeElement);

	XMLElement* nameElement = pDoc->NewElement("name");
	nameElement->LinkEndChild(pDoc->NewText(file->getName().c_str()));
	fileElement->LinkEndChild(nameElement);

	XMLElement* typeElement = pDoc->NewElement("type");
	typeElement->LinkEndChild(pDoc->NewText(to_string(file->getType()).c_str()));
	fileElement->LinkEndChild(typeElement);

	XMLElement* infoElement = pDoc->NewElement("info");
	infoElement->LinkEndChild(pDoc->NewText(file->getInfo().c_str()));
	fileElement->LinkEndChild(infoElement);

	XMLElement* parentElement = pDoc->NewElement("parent");
	auto parent = file->getParent();
	parentElement->LinkEndChild(pDoc->NewText((parent == nullptr ? "nullptr" : parent->getName().c_str())));
	fileElement->LinkEndChild(parentElement);

	XMLElement* dataElement = pDoc->NewElement("data");

	// NOTE: save data to data file
	ofstream ofs;
	ofs.open((file->getName() + ".dat").c_str(), ios::binary);
	if (ofs)
	{
		// NOTE: save data file name as text
		dataElement->LinkEndChild(pDoc->NewText((file->getName() + ".dat").c_str()));
		// NOTE: save data
		string dataS;
		for (auto c : file->getBinData())
		{
			dataS.push_back(c);
		}
		ofs.write(dataS.c_str(), sizeof(unsigned char)*(dataS.size()));
		ofs.close();
	}
	else
	{
		// NOTE: save nullptr as text
		dataElement->LinkEndChild(pDoc->NewText("nullptr"));
	}
	fileElement->LinkEndChild(dataElement);

	return fileElement;
}

XMLElement* createDF(DirectoryFile* file)
{
	// <DirectoryFile>
	// <date>file date</date>
	// <time>file time</time>
	// <size>file size</size>
	// <name>file name</name>
	// <type>file type</type>
	// <info>file info</info>
	// <parent>file parent</parent>
	XMLElement* fileElement = pDoc->NewElement("File");

	XMLElement* dateElement = pDoc->NewElement("date");
	dateElement->LinkEndChild(pDoc->NewText(file->getDate().c_str()));
	fileElement->LinkEndChild(dateElement);

	XMLElement* timeElement = pDoc->NewElement("time");
	timeElement->LinkEndChild(pDoc->NewText(file->getTime().c_str()));
	fileElement->LinkEndChild(timeElement);

	XMLElement* sizeElement = pDoc->NewElement("size");
	sizeElement->LinkEndChild(pDoc->NewText(to_string(file->getSize()).c_str()));
	fileElement->LinkEndChild(sizeElement);

	XMLElement* nameElement = pDoc->NewElement("name");
	nameElement->LinkEndChild(pDoc->NewText(file->getName().c_str()));
	fileElement->LinkEndChild(nameElement);

	XMLElement* typeElement = pDoc->NewElement("type");
	typeElement->LinkEndChild(pDoc->NewText(to_string(file->getType()).c_str()));
	fileElement->LinkEndChild(typeElement);

	XMLElement* infoElement = pDoc->NewElement("info");
	infoElement->LinkEndChild(pDoc->NewText(file->getInfo().c_str()));
	fileElement->LinkEndChild(infoElement);

	XMLElement* parentElement = pDoc->NewElement("parent");
	auto parent = file->getParent();
	parentElement->LinkEndChild(pDoc->NewText((parent == nullptr ? "nullptr" : parent->getName().c_str())));
	fileElement->LinkEndChild(parentElement);

	return fileElement;
}

XMLElement* createSGF(SymlinkFile* file)
{
	// <SymbolGeneralFile>
	// <date>file date</date>
	// <time>file time</time>
	// <size>file size</size>
	// <name>file name</name>
	// <type>file type</type>
	// <info>file info</info>
	// <parent>file parent</parent>
	// <link>file link</link>
	XMLElement* fileElement = pDoc->NewElement("File");

	XMLElement* dateElement = pDoc->NewElement("date");
	dateElement->LinkEndChild(pDoc->NewText(file->getDate().c_str()));
	fileElement->LinkEndChild(dateElement);

	XMLElement* timeElement = pDoc->NewElement("time");
	timeElement->LinkEndChild(pDoc->NewText(file->getTime().c_str()));
	fileElement->LinkEndChild(timeElement);

	XMLElement* sizeElement = pDoc->NewElement("size");
	sizeElement->LinkEndChild(pDoc->NewText(to_string(file->getSize()).c_str()));
	fileElement->LinkEndChild(sizeElement);

	XMLElement* nameElement = pDoc->NewElement("name");
	nameElement->LinkEndChild(pDoc->NewText(file->getName().c_str()));
	fileElement->LinkEndChild(nameElement);

	XMLElement* typeElement = pDoc->NewElement("type");
	typeElement->LinkEndChild(pDoc->NewText(to_string(file->getType()).c_str()));
	fileElement->LinkEndChild(typeElement);

	XMLElement* infoElement = pDoc->NewElement("info");
	infoElement->LinkEndChild(pDoc->NewText(file->getInfo().c_str()));
	fileElement->LinkEndChild(infoElement);

	XMLElement* parentElement = pDoc->NewElement("parent");
	auto parent = file->getParent();
	parentElement->LinkEndChild(pDoc->NewText((parent == nullptr ? "nullptr" : parent->getName().c_str())));
	fileElement->LinkEndChild(parentElement);

	XMLElement* linkElement = pDoc->NewElement("link");
	auto link = file->getLinkFile();
	linkElement->LinkEndChild(pDoc->NewText((link == nullptr ? "nullptr" : link->getName().c_str())));
	fileElement->LinkEndChild(linkElement);

	return fileElement;
}

XMLElement* createSDF(SymlinkdFile* file)
{
	// <SymlinkdFile>
	// <date>file date</date>
	// <time>file time</time>
	// <size>file size</size>
	// <name>file name</name>
	// <type>file type</type>
	// <info>file info</info>
	// <parent>file parent</parent>
	// <link>file link</link>
	XMLElement* fileElement = pDoc->NewElement("File");

	XMLElement* dateElement = pDoc->NewElement("date");
	dateElement->LinkEndChild(pDoc->NewText(file->getDate().c_str()));
	fileElement->LinkEndChild(dateElement);

	XMLElement* timeElement = pDoc->NewElement("time");
	timeElement->LinkEndChild(pDoc->NewText(file->getTime().c_str()));
	fileElement->LinkEndChild(timeElement);

	XMLElement* sizeElement = pDoc->NewElement("size");
	sizeElement->LinkEndChild(pDoc->NewText(to_string(file->getSize()).c_str()));
	fileElement->LinkEndChild(sizeElement);

	XMLElement* nameElement = pDoc->NewElement("name");
	nameElement->LinkEndChild(pDoc->NewText(file->getName().c_str()));
	fileElement->LinkEndChild(nameElement);

	XMLElement* typeElement = pDoc->NewElement("type");
	typeElement->LinkEndChild(pDoc->NewText(to_string(file->getType()).c_str()));
	fileElement->LinkEndChild(typeElement);

	XMLElement* infoElement = pDoc->NewElement("info");
	infoElement->LinkEndChild(pDoc->NewText(file->getInfo().c_str()));
	fileElement->LinkEndChild(infoElement);

	XMLElement* parentElement = pDoc->NewElement("parent");
	auto parent = file->getParent();
	parentElement->LinkEndChild(pDoc->NewText((parent == nullptr ? "nullptr" : parent->getName().c_str())));
	fileElement->LinkEndChild(parentElement);

	XMLElement* linkElement = pDoc->NewElement("link");
	auto link = file->getLinkDirectory();
	linkElement->LinkEndChild(pDoc->NewText((link == nullptr ? "nullptr" : link->getName().c_str())));
	fileElement->LinkEndChild(linkElement);

	return fileElement;
}