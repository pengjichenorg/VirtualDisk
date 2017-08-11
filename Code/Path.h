#pragma once

#include <queue>

using namespace std;

class Path
{
public:
	Path(const string& pathStr = ""); // pathStr seems like: folder\folder\folder
	queue<string> m_pathQueue;
};

