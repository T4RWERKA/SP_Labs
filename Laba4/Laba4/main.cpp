#include <iostream>
#include <locale>
#include <Windows.h>
#include <algorithm>
#include <thread>
#include <string>
#include <vector>
#include "Console.h"
#include "MultithreadingSort.h"
#include "ThreadPool.h"

void Sort(void* stringArr)
{
	std::vector<std::string>* strings = (std::vector<std::string>*)stringArr;
	std::sort(strings->begin(), strings->end());
}

// Split strings to threads and create task queue
void FillTaskQueue(TaskQueue *result, std::vector<std::string> data, int threadsCount, std::vector<std::vector<std::string>>& stringsForThreads)
{
	int remainder = threadsCount - (data.size() % threadsCount);
	int stringsInThread = data.size() / threadsCount;
	int stringCounter = 0;

	for (int i = 0; i < threadsCount; i++)
	{
		if (i >= remainder)
			stringsInThread++;

		stringsForThreads[i].clear();
		int threadStringCounter = 0;
		while ((threadStringCounter != stringsInThread) && (stringCounter != data.size()))
		{
			stringsForThreads[i].push_back(data[stringCounter]);
			stringCounter++;
			threadStringCounter++;
		}

		result->Push(new Task(Sort, &stringsForThreads[i]));
	}
}

int main()
{
	std::vector<std::string> data;
	std::vector<std::vector<std::string>> threadsStrings;
	TaskQueue *taskQueue = new TaskQueue();
	int threadCount;

	Console::OutputReadFromFile(data);
	threadCount = Console::GetThreadCount(data.size());

	threadsStrings.resize(threadCount);
	FillTaskQueue(taskQueue, data, threadCount, threadsStrings);

	ThreadPool* threadsPool = new ThreadPool(threadCount);
	threadsPool->CreateThreads(taskQueue);
	threadsPool->WaitForAllThreads();

	MultithreadingSort::FinalSort(data, threadsStrings);
	std::cout << "Strings are sorted!" << std::endl;

	Console::OutputWriteToFile(data);

	delete threadsPool;
	delete taskQueue;

	return 0;
}
