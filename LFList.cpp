#include "LFList.h"

void LFList::Init()
{
	while (head.next.get_ptr() != &tail)
	{
		LFNode* p = head.next.get_ptr();
		head.next.set_ptr(p->next.get_ptr());
		delete p;
	}
}

LFList::LFList()
{
	head.value = 0x80000000;
	tail.value = 0x7FFFFFFF;
	head.next.set_ptr(&tail);
}

LFList::~LFList()
{
	Init();
}

void LFList::Find(int x, LFNode*& pre, LFNode*& cur)
{
	while (true)
	{
		retry:
		pre = &head;
		cur = pre->next.get_ptr();
		while (true)
		{
			//cur를 쓰레기 제거
			LFNode* succ;
			bool removed = cur->next.get_removed(succ);
			while (removed)
			{
				if (!pre->next.CAS(cur, succ, false, false))
					goto retry;
				removed = cur->next.get_removed(succ);
			}

			if (cur->value >= x)
				return;
			pre = cur;
			cur = cur->next.get_ptr();
		}
	}
}

bool LFList::Add(int x)
{
	LFNode* pre, * cur;

	while (true) {
		Find(x, pre, cur);
		if (cur->value == x){
			return false;
		}

		LFNode* nNode = new LFNode(x);
		nNode->next.set_ptr(cur);
		if (pre->next.CAS(cur, nNode, false, false))
			return true;
		delete nNode;
	}
}

bool LFList::Remove(int x)
{
	LFNode* pre, * cur;

	while (true) {
		Find(x, pre, cur);

		if (cur->value == x)
		{
			LFNode* succ = cur->next.get_ptr();
			bool snip = cur->next.set_removed(succ, true);
			if (!snip)
				continue;
			pre->next.CAS(cur, succ, false, false);
			//cur->marked = true;
			//pre->next = cur->next;
			return true;
		}
		else
			return false;
	}
}

bool LFList::Contains(int x)
{
	LFNode* cur;

	cur = &head;
	while (cur->value < x)
		cur = cur->next.get_ptr();

	return (cur->value == x) &&
		!cur->next.get_removed(cur);
}

void LFList::Print20()
{
	LFNode* p = head.next.get_ptr();
	for (int i = 0; i < 20; ++i)
	{
		if (p == &tail)
			break;
		std::cout << p->value << ", ";
		p = p->next.get_ptr();
	}
	std::cout << std::endl;
}
