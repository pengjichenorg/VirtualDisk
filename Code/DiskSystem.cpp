#include "stdafx.h"
#include "DiskSystem.h"

#include "BinaryFile.h"
#include "SymlinkFile.h"
#include "SymlinkdFile.h"

DiskSystem* DiskSystem::m_instance = nullptr;

DiskSystem* DiskSystem::getInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new DiskSystem();
	}
	return m_instance;
}

void DiskSystem::destoryInstance()
{
	if (m_instance != nullptr)
	{
		delete m_instance;
	}
	m_instance = nullptr;
}

DirectoryFile* DiskSystem::getRootDirectory()
{
	return m_rootDirectory;
}

void DiskSystem::setRootDirectory(DirectoryFile* root)
{
	if (m_rootDirectory != nullptr)
	{
		delete m_rootDirectory;
	}
	m_rootDirectory = root;
}

DiskSystem::DiskSystem()
{
	m_rootDirectory = new DirectoryFile("root");

	// NOTE: for test
	auto C_Directory = new DirectoryFile("C:");
	auto E1_Directory = new DirectoryFile("E1");
	auto E11_Directory = new DirectoryFile("E11");
	auto E1_Binary = new BinaryFile("E1.txt");
	auto E11_Binary = new BinaryFile("E11.txt");
	auto E1L_LINKD = new SymlinkdFile("E1L", E1_Directory);
	auto E11L_LINKD = new SymlinkdFile("E11L", E11_Directory);

	m_rootDirectory->addChild(C_Directory);
	C_Directory->addChild(E1_Directory);
	C_Directory->addChild(E1_Binary);
	E1_Directory->addChild(E11_Directory);
	E1_Directory->addChild(E11_Binary);
	E11_Directory->addChild(E1L_LINKD);
	E11_Directory->addChild(E11L_LINKD);
}

DiskSystem::~DiskSystem()
{
	if (m_rootDirectory != nullptr)
	{
		delete m_rootDirectory;
	}
	m_rootDirectory = nullptr;
}