#include "SPLList.h"

void SPLList::Init()
{
	head->next = tail;
}

SPLList::SPLList()
{
	head = std::make_shared<SPNode>(0x80000000);
	tail = std::make_shared<SPNode>(0x7FFFFFFF);
	head->next = tail;
}

SPLList::~SPLList()
{
	Init();
}

bool SPLList::Validate(const std::shared_ptr<SPNode>& pre, const std::shared_ptr<SPNode>& cur)
{
	return (false == pre->marked) &&
		(false == cur->marked) &&
		(pre->next == cur);
}

bool SPLList::Add(int x)
{
	std::shared_ptr<SPNode> pre, cur;

	while (true) {
		pre = head;
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

			std::shared_ptr<SPNode> nNode = std::make_shared<SPNode>(x);
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

bool SPLList::Remove(int x)
{
	std::shared_ptr<SPNode> pre, cur;

	while (true) {
		pre = head;
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
				//이 부분이 critical section이 아닌가?
				pre->next = cur->next;
				//
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

bool SPLList::Contains(int x)
{
	std::shared_ptr<SPNode> cur;

	cur = head;
	while (cur->value < x)
		cur = cur->next;

	return (cur->value == x) &&
		!cur->marked;
}

void SPLList::Print20()
{
	std::shared_ptr<SPNode> p = head->next;
	for (int i = 0; i < 20; ++i)
	{
		if (p == tail)
			break;
		std::cout << p->value << ", ";
		p = p->next;
	}
	std::cout << std::endl;
}
