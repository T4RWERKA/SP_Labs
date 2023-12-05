#include "TaskQueue.h"

// Создание пустой очереди.
TaskQueue::TaskQueue()
{
	// Инициализация критической секции.
	InitializeCriticalSectionAndSpinCount(&criticalSection, SPINS);
	first = nullptr;
	last = nullptr;
}

// Обращение к первому элементу очереди.
Task *TaskQueue::Front()
{
	Task *returnValue;

	// Запрос владения критической секцией.
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

	// Освобождение права собственности на критическую секцию.
	LeaveCriticalSection(&criticalSection);
	return returnValue;
}

// Постановка элемента в очередь.
void TaskQueue::Push(Task* data)
{
	// Запрос владения критической секцией.
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

	// Освобождение права собственности на критическую секцию.
	LeaveCriticalSection(&criticalSection);
}

// Удаление первого элемента из очереди.
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