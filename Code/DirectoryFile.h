#pragma once
#include "File.h"

#include <map>
#include <string>
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

	DirectoryFile* getParent() const;

	void addChild(File* file);
	void removeChild(const string& name);
	map<string, File*>& getChildren();
	ChildIterator search(const string& name);

protected:
	map<string, File*> m_children;
};
