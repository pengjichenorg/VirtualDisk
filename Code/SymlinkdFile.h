#pragma once

#include "DirectoryFile.h"

#include <vector>
#include <map>

using namespace std;

class SymlinkdFile :
	public DirectoryFile
{
public:
	SymlinkdFile();
	SymlinkdFile(const string& name, DirectoryFile* link);
	SymlinkdFile(vector<string> infos);
	virtual ~SymlinkdFile();

	virtual void addChild(File* file);
	virtual void removeChild(const string& name, FileType type);
	virtual map<string, File*>& getChildren();
	virtual ChildIterator search(const string& name, FileType type);

	DirectoryFile* getLinkDirectory();
	void setLinkDirectory(DirectoryFile* link);

private:
	DirectoryFile* m_link;
};

