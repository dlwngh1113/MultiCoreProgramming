#include "CList.h"

CList::CList()
{
	head.value = 0x80000000;
	tail.value = 0x7FFFFFFF;
	head.next = &tail;
}

CList::~CList()
{
	Init();
}

void CList::Init()
{
	while (head.next != &tail)
	{
		CNode* p = head.next;
		head.next = p->next;
		delete p;
	}
}

bool CList::Add(int x)
{
	CNode* pre, * cur;
	pre = &head;

	Lock.lock();

	cur = pre->next;
	while (cur->value < x)
	{
		pre = cur;
		cur = cur->next;
	}

	if (cur->value == x)
	{
		Lock.unlock();
		return false;
	}

	CNode* nNode = new CNode(x);
	pre->next = nNode;
	nNode->next = cur;
	Lock.unlock();
	return true;
}

bool CList::Remove(int x)
{
	CNode* pre, * cur;
	pre = &head;

	Lock.lock();

	cur = pre->next;
	while (cur->value < x)
	{
		pre = cur;
		cur = cur->next;
	}

	if (cur->value == x)
	{
		pre->next = cur->next;
		delete cur;
		Lock.unlock();
		return true;
	}

	Lock.unlock();
	return false;
}

bool CList::Contains(int x)
{
	CNode* cur;

	cur = &head;
	Lock.lock_shared();

	while (cur->value < x)
		cur = cur->next;

	if (cur->value == x)
	{
		Lock.unlock_shared();
		return true;
	}

	Lock.unlock_shared();
	return false;
}

void CList::Print20()
{
	CNode* p = head.next;
	for (int i = 0; i < 20; ++i)
	{
		if (p == &tail)
			break;
		std::cout << p->value << ", ";
		p = p->next;
	}
}
