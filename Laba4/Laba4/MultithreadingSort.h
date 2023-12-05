#pragma once

#include <string>
#include <vector>

namespace MultithreadingSort
{
	void FinalSort(std::vector<std::string>& data, std::vector<std::vector<std::string>>& stringsFromThreads);
	bool IsBufferEmpty(std::vector<std::string> data);
	int GetMinValueIndex(std::vector<std::string> data);
};
