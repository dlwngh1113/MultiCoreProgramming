#include "OList.h"

void OList::Init()
{
	while (head.next != &tail)
	{
		FNode* p = head.next;
		head.next = p->next;
		delete p;
	}
}

OList::OList()
{
	head.value = 0x80000000;
	tail.value = 0x7FFFFFFF;
	head.next = &tail;
}

OList::~OList()
{
	Init();
}

bool OList::Validate(FNode* pre, FNode* cur)
{
	FNode* node = &head;
	while (node->value <= pre->value)
	{
		if (node == pre)
			return pre->next == cur;
		node = node->next;
	}
	return false;
}

bool OList::Add(int x)
{
	FNode* pre, * cur;

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

			FNode* nNode = new FNode(x);
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

bool OList::Remove(int x)
{
	FNode* pre, * cur;

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

bool OList::Contains(int x)
{
	FNode* pre, * cur;

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
				return true;
			}
			else
			{
				pre->nodeLock.unlock();
				cur->nodeLock.unlock();
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

void OList::Print20()
{
	FNode* p = head.next;
	for (int i = 0; i < 20; ++i)
	{
		if (p == &tail)
			break;
		std::cout << p->value << ", ";
		p = p->next;
	}
	std::cout << std::endl;
}
