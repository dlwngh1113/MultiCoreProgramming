#include "SkipList.h"

SkipList::SkipList()
{
	head.value = 0x80000000;
	tail.value = 0x7FFFFFFF;
	for (auto& h : head.next)
		h = &tail;
}

SkipList::~SkipList()
{
	Init();
}

void SkipList::Init()
{
	while (head.next[0] != &tail)
	{
		SKNode* p = head.next[0];
		head.next[0] = p->next[0];
		delete p;
	}
	for (auto& h : head.next)
		h = &tail;
}

bool SkipList::Add(int x)
{
	SKNode* pre[MAX_LEVEL + 1], * cur[MAX_LEVEL + 1];
	Lock.lock();
	Find(x, pre, cur);

	if (cur[0]->value == x)
	{
		Lock.unlock();
		return false;
	}
	else
	{
		int nLevel = 0;
		while (rand() % 2) 
		{
			++nLevel;
			if (MAX_LEVEL == nLevel)
				break;
		}
		SKNode* nNode = new SKNode(x, nLevel);
		for (int i = 0; i <= nLevel; ++i)
			nNode->next[i] = cur[i];
		for (int i = 0; i <= nLevel; ++i)
			pre[i]->next[i] =nNode;
		Lock.unlock();
		return true;
	}
}

bool SkipList::Remove(int x)
{
	SKNode* pre[MAX_LEVEL + 1], * cur[MAX_LEVEL + 1];
	Lock.lock();
	Find(x, pre, cur);

	if(cur[0]->value == x)
	{
		for (int i = 0; i <= cur[0]->topLevel; ++i)
			pre[i]->next[i] = cur[i]->next[i];
		delete cur[0];
		Lock.unlock();
		return true;
	}

	Lock.unlock();
	return false;
}

void SkipList::Find(int x, SKNode* pre[], SKNode* cur[])
{
	int curLevel = MAX_LEVEL;
	pre[curLevel] = &head;

	while (true)
	{
		cur[curLevel] = pre[curLevel]->next[curLevel];
		while (cur[curLevel]->value < x)
		{
			pre[curLevel] = cur[curLevel];
			cur[curLevel] = cur[curLevel]->next[curLevel];
		}
		if (curLevel == 0)
			return;
		--curLevel;
		pre[curLevel] = pre[curLevel + 1];
	}
}

void SkipList::Print20()
{
	SKNode* p = head.next[0];
	for (int i = 0; i < 20; ++i)
	{
		if (p == &tail)
			break;
		std::cout << p->value << ", ";
		p = p->next[0];
	}
}
