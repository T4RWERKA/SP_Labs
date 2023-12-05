#include "ThreadPool.h"

DWORD WINAPI DoThreadWork(LPVOID lParam)
{
	TaskQueue* tasks = (TaskQueue*)lParam;

	Task* tmp = tasks->Front();
	while (tmp != NULL)
	{
		tmp->func(tmp->params);
		tmp = tasks->Front();
	}

	return 0;
}

ThreadPool::ThreadPool(int threadCount) : 
	threadCount(threadCount)
{
	threads = new HANDLE[threadCount];
}

void ThreadPool::CreateThreads(TaskQueue *taskQueue)
{
	for (int i = 0; i < threadCount; i++)
	{
		threads[i] = CreateThread(NULL, 0, DoThreadWork, taskQueue, 0, NULL);
	}
}

ThreadPool::~ThreadPool()
{
	delete[] threads;
	DeleteCriticalSection(&criticalSection);
}

void ThreadPool::WaitForAllThreads()
{
	WaitForMultipleObjects(threadCount, threads, TRUE, INFINITE);
}
