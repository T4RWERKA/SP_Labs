#pragma once

#include <iostream>
#include <vector>

namespace Console
{
	void OutputReadFromFile(std::vector<std::string> &stringsFromFile);
	void OutputWriteToFile(std::vector<std::string> &stringsFromFile);
	int GetThreadCount(size_t stringsCount);
};