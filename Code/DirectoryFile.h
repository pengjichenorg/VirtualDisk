#pragma once

#include "File.h"

#include <map>
#include <vector>

using namespace std;

typedef map<string, File*>::iterator ChildIterator;

class DirectoryFile :
	public File
{
public:
	DirectoryFile();
	DirectoryFile(const string& name);
	DirectoryFile(vector<string> infos);
	virtual ~DirectoryFile();

	virtual void addChild(File* file);
	virtual void removeChild(const string& name, FileType type);
	virtual map<string, File*>& getChildren();
	virtual ChildIterator search(const string& name, FileType type);

protected:
	map<string, File*> m_children;
};
