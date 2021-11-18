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
	int fLevel = Find(x, pre, cur);

	while (true)
	{
		if (fLevel != -1)
		{
			if (cur[fLevel]->isMarked)
				continue;
			while (!cur[fLevel]->fullyLinked);
			return false;
		}
		//노드가 존재하지 않음
		//locking 시도
		int nLevel = 0;
		while (rand() % 2)
		{
			++nLevel;
			if (MAX_LEVEL == nLevel)
				break;
		}

		int isValid = true;
		int maxLockLevel = -1;
		for (int i = 0; i <= nLevel; ++i)
		{
			pre[i]->nodeLock.lock();
			maxLockLevel = i;
			if (!pre[i]->isMarked &&
				(!cur[i]->isMarked) &&
				(pre[i]->next[i] == cur[i]));
			else
			{
				isValid = false;
				break;
			}
		}

		if (!isValid)
		{
			for (int i = 0; i < maxLockLevel; ++i)
				pre[i]->nodeLock.unlock();
			continue;
		}
		SKNode* e = new SKNode(x, nLevel);
		for (int i = 0; i <= nLevel; ++i)
			e->next[i] = cur[i];
		for (int i = 0; i <= nLevel; ++i)
			pre[i]->next[i] = e;
		e->fullyLinked = true;

		for (int i = 0; i <= MAX_LEVEL; ++i)
			pre[i]->nodeLock.unlock();

		return true;
	}
}

bool SkipList::Remove(int x)
{
	SKNode* pre[MAX_LEVEL + 1], * cur[MAX_LEVEL + 1];
	SKNode* victim{ nullptr };
	bool isMarked{ false };
	int topLevel{ -1 };

	while (true)
	{
		int lFound = Find(x, pre, cur);
		if (lFound != -1)
			victim = cur[lFound];
		if (isMarked ||
			(lFound != -1) &&
			(victim->fullyLinked) &&
			(victim->topLevel == lFound) &&
			(!victim->isMarked))
		{
			if (!isMarked)
			{
				topLevel = victim->topLevel;
				victim->nodeLock.lock();
				if (victim->isMarked)
				{
					victim->nodeLock.unlock();
					return false;
				}
				victim->isMarked = true;
				isMarked = true;
			}
			int highLocked = -1;
			while(true)
			{
				SKNode* pred, * cur;
				bool valid{ true };
				for (int i = 0; i < topLevel; ++i)
				{
					pred = pre[i];
					pred->nodeLock.lock();
					highLocked = i;
					valid = !pred->isMarked && (pred->next[i] == victim);
					if (!valid)
					{
						for (int i = 0; i <= highLocked; ++i)
							pre[i]->nodeLock.unlock();
						continue;
					}
					for (int i = topLevel; i >= 0; --i)
					{
						pre[i]->next[i] = victim->next[i];
					}
					victim->nodeLock.unlock();
					for (int i = 0; i <= highLocked; ++i)
						pre[i]->nodeLock.unlock();
					return true;
				}
			}
		}
		else
			return true;
	}
}

bool SkipList::Contains(int x)
{
	SKNode* pre[MAX_LEVEL + 1], * cur[MAX_LEVEL + 1];
	int lFound = Find(x, pre, cur);
	return (lFound != -1 && cur[lFound]->fullyLinked &&
		!cur[lFound]->isMarked);
}

int SkipList::Find(int x, SKNode* pre[], SKNode* cur[])
{
	int fLevel = -1;
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
		if ((fLevel == -1) && cur[curLevel]->value == x)
			fLevel = curLevel;
		if (curLevel == 0)
			return fLevel;
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
