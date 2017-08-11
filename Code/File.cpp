#include "stdafx.h"
#include "File.h"

#include <Windows.h> // get system time
#include <algorithm>

File::File()
{
}

File::File(const string & name)
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);

	int month = sys.wMonth;
	string monthStr = to_string(month);
	if (month < 10)
	{
		monthStr = "0" + monthStr;
	}
	int day = sys.wDay;
	string dayStr = to_string(day);
	if (day < 10)
	{
		dayStr = "0" + dayStr;
	}
	string yearStr = to_string(sys.wYear);

	m_date = monthStr + "/" + dayStr + "/" + yearStr + "  ";

	string AP = "AM";
	int hour = sys.wHour;
	if (hour > 12)
	{
		AP = "PM";
		hour = hour - 12;
	}
	string hourStr = to_string(hour);
	if (hour < 10)
	{
		hourStr = "0" + hourStr;
	}
	int minute = sys.wMinute;
	string minuteStr = to_string(minute);
	if (minute < 10)
	{
		minuteStr = "0" + minuteStr;
	}

	m_time = hourStr + ":" + minuteStr + " " + AP + "    ";

	m_name = name;
}


File::~File()
{

}

string File::getDate() const
{
	return m_date;
}

string File::getTime() const
{
	return m_time;
}

size_t File::getSize() const
{
	return m_size;
}

string File::getName() const
{
	return m_name;
}

string File::getInfo() const
{
	return m_info;
}

FileType File::getType() const
{
	return m_type;
}

string File::getTypeString() const
{
	return m_typeString;
}

File* File::getParent() const
{
	return m_parent;
}

void File::setName(string name)
{
	m_name = name;
}

void File::setParent(File* file)
{
	m_parent = file;
}
