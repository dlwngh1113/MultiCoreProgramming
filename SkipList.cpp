#include "SkipList.h"

int SkipList::RandomLevel()
{
	int new_level = 0;
	while ((rand() % 2) == 1) {
		new_level++;
		if (MAX_LEVEL == new_level)
			break;
	}
	return new_level;
}

SkipList::SkipList()
{
	head.value = 0x80000000;
	tail.value = 0x7FFFFFFF;
	for (auto& h : head.next)
		h.set_ptr(&tail);
}

SkipList::~SkipList()
{
	Init();
}

void SkipList::Init()
{
	while (head.next[0].get_ptr() != &tail)
	{
		LFNode* p = head.next[0].get_ptr();
		head.next[0].set_ptr(p->next[0].get_ptr());
		delete p;
	}
	for (auto& h : head.next)
		h.set_ptr(&tail);
}

bool SkipList::Add(int x)
{
	LFNode* pre[MAX_LEVEL + 1], * cur[MAX_LEVEL + 1];

	while (true)
	{
		bool isFound = Find(x, pre, cur);
		if (isFound)
			return false;

		int topLevel = RandomLevel();
		LFNode* nNode = new LFNode(x, topLevel);
		for (int i = 0; i <= topLevel; ++i)
		{
			auto suc = cur[i];
			nNode->next[i].set_ptr(suc);
		}

		LFNode* pred = pre[0];
		LFNode* suc = cur[0];
		nNode->next[0].set_ptr(suc);
		if (!pred->next[0].CAS(suc, nNode, false, false))
		{
			delete nNode;
			continue;
		}
		for (int i = 1; i <= topLevel; ++i)
		{
			while (true)
			{
				pred = pre[i];
				suc = cur[i];
				if (pred->next[i].CAS(suc, nNode, false, false))
					break;
				Find(x, pre, cur);
			}
		}
		return true;
	}
}

bool SkipList::Remove(int x)
{
	LFNode* pre[MAX_LEVEL + 1], * cur[MAX_LEVEL + 1];

	if (!Find(x, pre, cur))
		return false;
	LFNode* target = cur[0];

	for (int i = target->topLevel; i > 0; --i)
	{
		LFNode* suc = target->next[i].get_ptr();
		while (true)
		{
			if (target->next[i].CAS(suc, suc, false, true))
				break;
			if (target->next[i].get_removed(suc))
				break;
		}
	}

	while (true)
	{
		LFNode* suc;
		bool isMarked = target->next[0].get_removed(suc);
		if (isMarked)
			return false;
		if (target->next[0].CAS(suc, suc, false, true))
		{
			Find(x, pre, cur);
			return true;
		}
	}
}

bool SkipList::Contains(int x)
{
	int bottomLevel{ 0 };
	bool isMarked = false;
	LFNode* pre = &head;
	LFNode* cur = nullptr;
	LFNode* suc = nullptr;
	for (int i = MAX_LEVEL; i >= bottomLevel; --i)
	{	
		cur = pre->next[i].get_ptr();
		while (isMarked)
		{
			cur = cur->next[i].get_ptr();
			isMarked = cur->next[i].get_removed(suc);
		}
		if (cur->value < x)
		{
			pre = cur;
			cur = suc;
		}
		else
			break;
	}

	return cur->value == x;
}

bool SkipList::Find(int x, LFNode* pre[], LFNode* cur[])
{
retry:
	int curLevel = MAX_LEVEL;
	pre[curLevel] = &head;
	while (true)
	{
		while (true)
		{
			cur[curLevel] = pre[curLevel]->next[curLevel].get_ptr();
			LFNode* suc;
			bool isRemoved = cur[curLevel]->next[curLevel].get_removed(suc);
			while (isRemoved)
			{
				if (!pre[curLevel]->next[curLevel].CAS(cur[curLevel], suc, false, false))
					goto retry;
				cur[curLevel] = suc;
				isRemoved = cur[curLevel]->next[curLevel].get_removed(suc);
			}
			if (cur[curLevel]->value >= x)
				break;
			pre[curLevel] = cur[curLevel];
		}

		if (curLevel == 0)
			return (cur[0]->value == x);
		--curLevel;
		pre[curLevel] = pre[curLevel + 1];
	}
}

void SkipList::Print20()
{
	LFNode* p = head.next[0].get_ptr();
	for (int i = 0; i < 20; ++i)
	{
		if (p == &tail)
			break;
		std::cout << p->value << ", ";
		p = p->next[0].get_ptr();
	}
}
