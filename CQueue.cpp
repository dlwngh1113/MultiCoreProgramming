#include "CQueue.h"

bool StampLFQueue::CAS(Node* volatile * addr, Node* old, Node* newNode)
{
	int a_addr = reinterpret_cast<int>(addr);
	return std::atomic_compare_exchange_strong(
		reinterpret_cast<std::atomic_int*>(a_addr), 
		reinterpret_cast<int*>(&old), 
		reinterpret_cast<int>(newNode));
}

bool StampLFQueue::StampCAS(StampNode* addr, Node* old_node, Node* new_node, int old_stamp, int new_stamp)
{
	StampNode old_p{ old_node, old_stamp };
	StampNode new_p{ new_node, new_stamp };
	long long new_value = *(reinterpret_cast<long long*>(&new_p));
	return std::atomic_compare_exchange_strong(
		reinterpret_cast<std::atomic_llong*>(addr),
		reinterpret_cast<long long*>(&old_p),
		new_value);
}

StampLFQueue::StampLFQueue()
{
	head.ptr = tail.ptr = new Node();
	tail.stamp = 0;
}

StampLFQueue::~StampLFQueue()
{
	Init();
	delete head.ptr;
}

void StampLFQueue::Init()
{
	while (head.ptr != tail.ptr)
	{
		Node* p = head.ptr;
		head.ptr = head.ptr->next;
		delete p;
	}
}

void StampLFQueue::Enqueue(int x)
{
	Node* e = new Node(x);
	while (true)
	{
		StampNode last = tail;
		Node* next = last.ptr->next;
		if (last.ptr != tail.ptr)
			continue;
		if (nullptr == next)
		{
			if (CAS(&(last.ptr->next), nullptr, e))
			{
				StampCAS(&tail, last.ptr, e, last.stamp, last.stamp + 1);
				//CAS(&tail, last, e);
				return;
			}
		}
		else
			StampCAS(&tail, last.ptr, next, last.stamp, last.stamp + 1);
			//CAS(&tail, last, next);
	}
}

int StampLFQueue::Dequeue()
{
	while (true)
	{
		StampNode first = head;
		StampNode last = tail;
		Node* next = first.ptr->next;
		if (head.ptr != first.ptr || (first.stamp != head.stamp))
			continue;
		if (nullptr == next)
			return -1;
		if (first.ptr == last.ptr)
		{
			StampCAS(&tail, last.ptr, next, last.stamp, last.stamp + 1);
			continue;
		}
		int value = next->value;
		if (!StampCAS(&head, first.ptr, next, first.stamp, first.stamp + 1))
			continue;
		delete first.ptr;
		return value;
	}
}

void StampLFQueue::Print20()
{
	Node* p = head.ptr->next;
	for (int i = 0; i < 20; ++i)
	{
		if (p == nullptr)
			break;
		std::cout << p->value << ", ";
		p = p->next;
	}
}
