#include "stdafx.h"
#include "Path.h"
#include "DiskSystem.h"

// NOTE: |          Path Condition           |
// NOTE: |-----------------------------------|
// NOTE: |   path string   |      queue      |
// NOTE: |-----------------+-----------------|
// NOTE: |      folder     |      folder     |
// NOTE: | folder1\folder2 | folder1,folder2 |
// NOTE: |     folder\     |      folder     |
// NOTE: |     \folder     |     C:,folder   |
// NOTE: |    \\folder     |      empty      |
// NOTE: | folder\\folder  |      empty      |
// NOTE: |     folder\\    |      empty      |

Path::Path(const string& pathStr)
{
	auto str = pathStr;
	if (!str.empty())
	{
		auto pos = str.find('\\');
		while (pos != string::npos)
		{
			// NOTE: for '\folder', convert to C:\folder
			if (pos == 0)
			{
				str = initPartition + str;
			}
			else
			{
				// NOTE: to avoid add an empty string to queue
				auto _str = str.substr(0, pos);
				if (!_str.empty())
				{
					m_pathQueue.emplace(_str);
				}
				str = str.substr(pos + 1);
			}
			pos = str.find('\\');
			// NOTE: avoid folder\\folder, only in \folder, pos could be 0
			if (pos == 0)
			{
				// NOTE: clear queue
				while (!m_pathQueue.empty())
				{
					m_pathQueue.pop();
				}
				break;
			}
		}
		// NOTE: to avoid add an empty string to queue
		if (!str.empty())
		{
			m_pathQueue.emplace(str.substr(0, pos));
		}
	}
}