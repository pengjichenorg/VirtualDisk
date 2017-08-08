#include "stdafx.h"
#include "Path.h"
#include "DiskSystem.h"

Path::Path(const string& pathStr)
{
	auto str = pathStr;
	if (!str.empty())
	{
		// NOTE: 'folder', 'folder\folder', 'folder\', '\folder'
		auto pos = str.find('\\');
		while (pos != string::npos)
		{
			// NOTE: for '\folder'
			if (pos == 0)
			{
				m_pathQueue.emplace(initPartition);
			}
			// NOTE: to avoid add an empty string to queue
			auto _str = str.substr(0, pos);
			if (!_str.empty())
			{
				m_pathQueue.emplace(_str);
			}

			str = str.substr(pos + 1);
			pos = str.find('\\');
		}
		// NOTE: to avoid add an empty string to queue
		if (!str.empty())
		{
			m_pathQueue.emplace(str.substr(0, pos));
		}
	}
}