#include "StringUtils.h"

#include <sstream>


std::vector<std::string> StringUtils::splitString(const std::string& string, const char delimiter)
{
	std::vector<std::string> lines;
	std::stringstream ss;

	for (const char c : string)
	{
		// Drop carriage returns
		if (c == '\r')
			continue;

		if (c == delimiter)
		{
			lines.emplace_back(ss.str());
			ss.str("");
			continue;
		}
		ss << c;

	}
	if (ss.str().length() > 0)
		lines.emplace_back(ss.str());

	return lines;
}
