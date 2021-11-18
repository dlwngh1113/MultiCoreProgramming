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

	int nLevel = 0;
	while (rand() % 2)
	{
		++nLevel;
		if (MAX_LEVEL == nLevel)
			break;
	}

	while (true)
	{
		retry:
		int fLevel = Find(x, pre, cur);
		if (fLevel != -1)
		{
			if (!cur[fLevel]->isMarked)
			{
				while (!cur[fLevel]->fullyLinked);
				return false;
			}
			continue;
		}
		//노드가 존재하지 않음
		//locking 시도
		int maxLockLevel = -1;
		while(true) {
			SKNode* pred, * curr;
			int isValid = true;

			for (int i = 0; i <= nLevel; ++i)
			{
				pred = pre[i];
				curr = cur[i];
				pred->nodeLock.lock();
				maxLockLevel = i;
				isValid = (!pred->isMarked &&
					(!curr->isMarked) &&
					(pred->next[i] == curr));
				if (!isValid)
				{
					for (int i = 0; i <= maxLockLevel; ++i)
						pre[i]->nodeLock.unlock();
					goto retry;
				}
			}
			SKNode* nNode = new SKNode(x, nLevel);
			for (int i = 0; i <= nLevel; ++i)
				nNode->next[i] = cur[i];
			for (int i = 0; i <= nLevel; ++i)
				pre[i]->next[i] = nNode;
			nNode->fullyLinked = true;

			for (int i = 0; i <= maxLockLevel; ++i)
				pre[i]->nodeLock.unlock();
			return true;
		}
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
		retry:
		int lFound = Find(x, pre, cur);
		if (lFound != -1)
			victim = cur[lFound];
		if (isMarked ||
			((lFound != -1) &&
			(victim->fullyLinked) &&
			(victim->topLevel == lFound) &&
			(!victim->isMarked)))
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
				for (int i = 0; i <= topLevel; ++i)
				{
					pred = pre[i];
					pred->nodeLock.lock();
					highLocked = i;
					valid = !pred->isMarked && (pred->next[i] == victim);
					if (!valid)
					{
						for (int i = 0; i <= highLocked; ++i)
							pre[i]->nodeLock.unlock();
						goto retry;
					}
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
