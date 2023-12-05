#include "MultithreadingSort.h"

namespace MultithreadingSort
{
	void FinalSort(std::vector<std::string> &data, std::vector<std::vector<std::string>> &stringsFromThreads)
	{
		data.clear();

		std::vector<std::string> buff;
		std::vector<int> counter;
		counter.reserve(stringsFromThreads.size());
		for (int i = 0; i < stringsFromThreads.size(); i++)
			counter.push_back(0);

		while (true)
		{
			buff.clear();
			for (int i = 0; i < stringsFromThreads.size(); i++)
			{
				if (stringsFromThreads[i].size() > counter[i])
					buff.push_back(stringsFromThreads[i][counter[i]]);
				else
					buff.push_back("");
			}

			if (IsBufferEmpty(buff))
				break;
			int minIndex = GetMinValueIndex(buff);
			data.push_back(buff[minIndex]);
			counter[minIndex]++;
		}
	}
	
	bool IsBufferEmpty(std::vector<std::string> data)
	{
		for (int i = 0; i < data.size(); i++)
			if (data[i] != "")
				return false;
		return true;
	}

	int GetMinValueIndex(std::vector<std::string> data)
	{
		std::string minValue;
		int index;

		for (int i = 0; i < data.size(); i++)
		{
			if (data[i] != "" && (minValue > data[i] || minValue == ""))
			{
				minValue = data[i];
				index = i;
			}
		}

		return index;
	}
}
