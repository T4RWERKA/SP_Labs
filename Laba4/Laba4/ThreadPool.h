#pragma once

#include "TaskQueue.h"

class ThreadPool
{
public:
	ThreadPool(int thread_quantity);
	~ThreadPool();
	void WaitForAllThreads();
	void CreateThreads(TaskQueue *taskQueue);

private:
	HANDLE *threads;
	CRITICAL_SECTION criticalSection;
	int threadCount;
};