#include "CQueue.h"

bool CQueue::CAS(Node* volatile * addr, Node* old, Node* newNode)
{
	int a_addr = reinterpret_cast<int>(addr);
	return std::atomic_compare_exchange_strong(
		reinterpret_cast<std::atomic_int*>(a_addr), 
		reinterpret_cast<int*>(&old), 
		reinterpret_cast<int>(newNode));
}

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
	Node* e = new Node(x);
	while (true)
	{
		Node* last = tail;
		Node* next = last->next;
		if (last != tail)
			continue;
		if (nullptr == next)
		{
			if (CAS(&last->next, nullptr, e))
			{
				CAS(&tail, last, e);
				return;
			}
		}
		else
			CAS(&tail, last, next);
	}
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
	while (true)
	{
		Node* first = head;
		Node* last = tail;
		Node* next = first->next;
		if (head != first)
			continue;
		if (nullptr == next)
			return -1;
		if (first == last)
		{
			CAS(&tail, last, next);
			continue;
		}
		int value = next->value;
		if (!CAS(&head, first, next))
			continue;
		delete first;
		return value;
	}
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
