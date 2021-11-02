#include "CQueue.h"

CQueue::CQueue()
{
	head = tail = new Node(0);
}

CQueue::~CQueue()
{
	Init();
}

void CQueue::Enqueue(int x)
{
	enqLock.lock();
	Node* node = new Node(x);
	tail->next = node;
	tail = node;
	enqLock.unlock();
}

void CQueue::Init()
{
	while (head != tail)
	{
		Node* p = head;
		head = head->next;
		delete p;
	}
}

int CQueue::Dequeue()
{
	int value;
	deqLock.lock();
	if (head->next == nullptr)
		value = -1;
	else
	{
		value = head->next->value;
		Node* p = head;
		head = head->next;
		delete p;
	}

	deqLock.unlock();

	return value;
}

void CQueue::Print20()
{
	Node* p = head->next;
	for (int i = 0; i < 20; ++i)
	{
		if (p == nullptr)
			break;
		std::cout << p->value << ", ";
		p = p->next;
	}
}
