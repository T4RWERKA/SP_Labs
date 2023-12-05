#include "TaskQueue.h"

// �������� ������ �������.
TaskQueue::TaskQueue()
{
	// ������������� ����������� ������.
	InitializeCriticalSectionAndSpinCount(&criticalSection, SPINS);
	first = nullptr;
	last = nullptr;
}

// ��������� � ������� �������� �������.
Task *TaskQueue::Front()
{
	Task *returnValue;

	// ������ �������� ����������� �������.
	EnterCriticalSection(&criticalSection);
	if (IsEmpty())
	{
		returnValue = NULL;
	}
	else if (first == last)
	{
		returnValue = first->data;
		first = NULL;
		last = NULL;
	}
	else
	{
		returnValue = first->data;
		first = first->prev;
		first->next = NULL;
	}

	// ������������ ����� ������������� �� ����������� ������.
	LeaveCriticalSection(&criticalSection);
	return returnValue;
}

// ���������� �������� � �������.
void TaskQueue::Push(Task* data)
{
	// ������ �������� ����������� �������.
	EnterCriticalSection(&criticalSection);

	Node* tmp = new Node(data, NULL, NULL);

	if (IsEmpty())
	{
		first = tmp;
		last = tmp;
	}
	else
	{
		Node* tmp_last = last;
		last = tmp;
		last->next = tmp_last;
		tmp_last->prev = last;
	}

	// ������������ ����� ������������� �� ����������� ������.
	LeaveCriticalSection(&criticalSection);
}

// �������� ������� �������� �� �������.
void TaskQueue::Pop()
{
	Task* tmp = Front();
	delete(tmp);
}

bool TaskQueue::IsEmpty()
{
	return ((first == NULL) && (last == NULL));
}

TaskQueue::~TaskQueue()
{
	while (!IsEmpty())
		Pop();
	DeleteCriticalSection(&criticalSection);
}