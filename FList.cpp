#include "FList.h"

FList::FList()
{
	head.value = 0x80000000;
	tail.value = 0x7FFFFFFF;
	head.next = &tail;
}

FList::~FList()
{
	Init();
}

void FList::Init()
{
	while (head.next != &tail)
	{
		LNode* p = head.next;
		head.next = p->next;
		delete p;
	}
}

bool FList::Add(int x)
{
	LNode* pre, * cur;
	head.nodeLock.lock();
	pre = &head;

	pre->next->nodeLock.lock();
	cur = pre->next;
	while (cur->value < x)
	{
		pre->nodeLock.unlock();
		pre = cur;
		cur = cur->next;
		cur->nodeLock.lock();
	}

	if (cur->value == x)
	{
		pre->nodeLock.unlock();
		cur->nodeLock.unlock();
		return false;
	}

	LNode* nNode = new LNode(x);
	pre->next = nNode;
	nNode->next = cur;
	cur->nodeLock.unlock();
	pre->nodeLock.unlock();
	return true;
}

bool FList::Remove(int x)
{
	LNode* pre, * cur;
	head.nodeLock.lock();
	pre = &head;

	pre->next->nodeLock.lock();
	cur = pre->next;

	while (cur->value < x)
	{
		pre->nodeLock.unlock();
		pre = cur;
		cur = cur->next;
		cur->nodeLock.lock();
	}

	if (cur->value == x)
	{
		pre->next = cur->next;
		cur->nodeLock.unlock();
		pre->nodeLock.unlock();
		delete cur;
		return true;
	}

	cur->nodeLock.unlock();
	pre->nodeLock.unlock();
	return false;
}

bool FList::Contains(int x)
{
	LNode* cur;
	head.nodeLock.lock();
	cur = &head;

	while (cur->value < x)
	{
		cur->nodeLock.unlock();
		cur = cur->next;
		cur->nodeLock.lock();
	}

	if (cur->value == x)
	{
		cur->nodeLock.unlock();
		return true;
	}

	cur->nodeLock.unlock();
	return false;
}

void FList::Print20()
{
	LNode* p = head.next;
	for (int i = 0; i < 20; ++i)
	{
		if (p == &tail)
			break;
		std::cout << p->value << ", ";
		p = p->next;
	}
	std::cout << std::endl;
}
