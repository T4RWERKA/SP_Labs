#pragma once

class Task
{
public:
	typedef void(*TaskFunction)(void *);

	TaskFunction func;
	void* params;

	Task(TaskFunction func, void *params) : func(func), params(params) {};
};
