#include "LList.h"

void LList::Init()
{
	while (head.next != &tail)
	{
		LNode* p = head.next;
		head.next = p->next;
		delete p;
	}
}

LList::LList()
{
	head.value = 0x80000000;
	tail.value = 0x7FFFFFFF;
	head.next = &tail;
}

LList::~LList()
{
	Init();
}

bool LList::Validate(LNode* pre, LNode* cur)
{
	return (false == pre->marked) &&
		(false == cur->marked) &&
		(pre->next == cur);
}

bool LList::Add(int x)
{
	LNode* pre, * cur;

	while (true) {
		pre = &head;
		cur = pre->next;
		while (cur->value < x)
		{
			pre = cur;
			cur = cur->next;
		}
		pre->nodeLock.lock();
		cur->nodeLock.lock();

		if (Validate(pre, cur))
		{
			if (cur->value == x)
			{
				pre->nodeLock.unlock();
				cur->nodeLock.unlock();
				return false;
			}

			LNode* nNode = new LNode(x);
			nNode->next = cur;
			pre->next = nNode;
			cur->nodeLock.unlock();
			pre->nodeLock.unlock();
			return true;
		}
		else
		{
			pre->nodeLock.unlock();
			cur->nodeLock.unlock();
		}
	}
}

bool LList::Remove(int x)
{
	LNode* pre, * cur;

	while (true) {
		pre = &head;
		cur = pre->next;
		while (cur->value < x)
		{
			pre = cur;
			cur = cur->next;
		}
		pre->nodeLock.lock();
		cur->nodeLock.lock();

		if (Validate(pre, cur))
		{
			if (cur->value == x)
			{
				cur->marked = true;
				pre->next = cur->next;
				pre->nodeLock.unlock();
				cur->nodeLock.unlock();
				return true;
			}
			else
			{
				cur->nodeLock.unlock();
				pre->nodeLock.unlock();
				return false;
			}
		}
		else
		{
			pre->nodeLock.unlock();
			cur->nodeLock.unlock();
		}
	}
}

bool LList::Contains(int x)
{
	LNode* cur;

	cur = &head;
	while (cur->value < x)
		cur = cur->next;

	return (cur->value == x) &&
		!cur->marked;
}

void LList::Print20()
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
