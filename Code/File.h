#pragma once

#include <string>

using namespace std;

enum FileType {
	directoryFile = 0,
	generalFile,
	symlink,
	symlinkd
};

class File
{
public:
	File();
	File(const string& name);
	virtual ~File() = 0;

	string getDate() const;
	string getTime() const;
	size_t getSize() const;
	string getName() const;
	string getInfo() const;
	FileType getType() const;
	string getTypeString() const;
	File* getParent() const;

	void setName(string name);
	virtual void setParent(File* file);

protected:
	string m_date;
	string m_time;
	FileType m_type;
	string m_typeString;
	size_t m_size = 0;
	string m_name;
	string m_info;
	File* m_parent;
};

