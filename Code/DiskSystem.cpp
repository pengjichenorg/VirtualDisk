#include "stdafx.h"
#include "DiskSystem.h"

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
	m_rootDirectory->addChild(new DirectoryFile("C:"));

	// test
	// C:
	// [DIR] E1
	//       E1.txt
	// C:\E1
	// [DIR] E11
	//       E11.txt

	// C:\E1
	static_cast<DirectoryFile*>(m_rootDirectory->search("C:")->second)->addChild(new DirectoryFile("E1"));
	// C:\E1.txt
	static_cast<DirectoryFile*>(m_rootDirectory->search("C:")->second)->addChild(new GeneralFile("E1.txt"));
	// C:\E1\E11
	static_cast<DirectoryFile*>(static_cast<DirectoryFile*>(m_rootDirectory->search("C:")->second)->search("E1")->second)->addChild(new DirectoryFile("E11"));
	// C:\E1\E11.txt
	static_cast<DirectoryFile*>(static_cast<DirectoryFile*>(m_rootDirectory->search("C:")->second)->search("E1")->second)->addChild(new GeneralFile("E11.txt"));
	// C:\E1L [E1]
	auto E1Dir = static_cast<DirectoryFile*>(static_cast<DirectoryFile*>(m_rootDirectory->search("C:")->second)->search("E1")->second);
	auto E1L = new SymbolDirectoryFile("E1L", E1Dir);
	static_cast<DirectoryFile*>(static_cast<DirectoryFile*>(static_cast<DirectoryFile*>(m_rootDirectory->search("C:")->second)->search("E1")->second)->search("E11")->second)->addChild(E1L);

	// C:\E1\EE1LL [E1L]
	auto E1LL = new SymbolDirectoryFile("E1LL", E1L);
	static_cast<DirectoryFile*>(m_rootDirectory->search("C:")->second)->addChild(E1LL);

	// test
	// cout << m_rootDirectory->getName() << endl;
	// cout << m_rootDirectory->search("C:")->second->getName() << endl;
	// cout << "C:\\" << static_cast<DirectoryFile*>(m_rootDirectory->search("C:")->second)->search("E1")->second->getName() << endl;
	// cout << "C:\\" << static_cast<DirectoryFile*>(m_rootDirectory->search("C:")->second)->search("E1.txt")->second->getName() << endl;
	// cout << "C:\\" << static_cast<DirectoryFile*>(m_rootDirectory->search("C:")->second)->search("E1LL")->second->getName() << endl;
	// cout << "C:\\E1\\" << static_cast<DirectoryFile*>(static_cast<DirectoryFile*>(m_rootDirectory->search("C:")->second)->search("E1")->second)->search("E11")->second->getName() << endl;
	// cout << "C:\\E1\\" << static_cast<DirectoryFile*>(static_cast<DirectoryFile*>(m_rootDirectory->search("C:")->second)->search("E1")->second)->search("E11.txt")->second->getName() << endl;
	// cout << "C:\\E1\\" << static_cast<DirectoryFile*>(static_cast<DirectoryFile*>(static_cast<DirectoryFile*>(m_rootDirectory->search("C:")->second)->search("E1")->second)->search("E11")->second)->search("E1L")->second->getName() << endl;
}

DiskSystem::~DiskSystem()
{
	if (m_rootDirectory != nullptr)
	{
		delete m_rootDirectory;
	}
	m_rootDirectory = nullptr;
}