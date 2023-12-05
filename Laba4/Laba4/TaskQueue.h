#pragma once

#include <windows.h>
#include "Task.h"

#define SPINS 3000

// Ёлемент очереди.
class Node
{
public:
	Task* data;
	Node* prev;
	Node* next;

	Node(){}

	Node(Task *data, Node *prev, Node *next) : data(data), prev(prev), next(next) {};
};

class TaskQueue
{
private:
	Node* first, * last;
	CRITICAL_SECTION criticalSection;

public:
	TaskQueue();
	~TaskQueue();

	Task* Front();
	void Push(Task* data);
	void Pop();
	bool IsEmpty();
};