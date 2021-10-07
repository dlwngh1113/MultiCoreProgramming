#include"stdafx.h"

using namespace std;
using namespace chrono;

constexpr int NUM_TEST = 4'000'000;
constexpr int MAX_THREADS = 8;
constexpr int NUM_RANGE = 1000;

class CNode {
public:
	int value;
	CNode* next;
	CNode() :value{ 0 }, next{ nullptr }{}
	CNode(int x) :value{ x }, next{ nullptr } {}
	virtual ~CNode() {}
};

class FNode {
public:
	int value;
	FNode* next;
	mutex nodeLock;
	FNode() :value{ 0 }, next{ nullptr }{}
	FNode(int x) :value{ x }, next{ nullptr } {}
	virtual ~FNode() {}
};

class nullmutex
{
public:
	void lock() {}
	void unlock() {}
};

class CList {
	CNode head, tail;
	nullmutex Lock;
public:
	CList() 
	{
		head.value = 0x80000000;
		tail.value = 0x7FFFFFFF;
		head.next = &tail;
	}
	~CList()
	{
		Init();
	}
	void Init()
	{
		while (head.next != &tail)
		{
			CNode* p = head.next;
			head.next = p->next;
			delete p;
		}
	}
	bool Add(int x)
	{
		CNode* pre, *cur;
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

	bool Remove(int x)
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

	bool Contains(int x)
	{
		CNode* cur;

		cur = &head;
		Lock.lock();

		while (cur->value < x)
			cur = cur->next;

		if (cur->value == x)
		{
			Lock.unlock();
			return true;
		}

		Lock.unlock();
		return false;
	}

	void Print20()
	{
		CNode* p = head.next;
		for (int i = 0; i < 20; ++i)
		{
			if (p == &tail)
				break;
			cout << p->value << ", ";
			p = p->next;
		}
	}
};

class FList {
	FNode head, tail;
public:
	FList()
	{
		head.value = 0x80000000;
		tail.value = 0x7FFFFFFF;
		head.next = &tail;
	}
	~FList()
	{
		Init();
	}
	void Init()
	{
		while (head.next != &tail)
		{
			FNode* p = head.next;
			head.next = p->next;
			delete p;
		}
	}
	bool Add(int x)
	{
		FNode* pre, * cur;
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

		FNode* nNode = new FNode(x);
		pre->next = nNode;
		nNode->next = cur;
		cur->nodeLock.unlock();
		pre->nodeLock.unlock();
		return true;
	}

	bool Remove(int x)
	{
		FNode* pre, * cur;
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

	bool Contains(int x)
	{
		FNode* cur;
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
	void Print20()
	{
		FNode* p = head.next;
		for (int i = 0; i < 20; ++i)
		{
			if (p == &tail)
				break;
			cout << p->value << ", ";
			p = p->next;
		}
		cout << endl;
	}
};

FList list;
set<int> s;

void Benchmark(int num_threads)
{
	const int loops = NUM_TEST / num_threads;
	for (int i = 0; i < loops; ++i)
	{
		int value = rand() % NUM_RANGE;
		switch (rand() % 3)
		{
		case 0:
			list.Add(value);
			break;
		case 1:
			list.Remove(value);
			break;
		case 2:
			list.Contains(value);
			break;
		}
	}
}

mutex sl;

void Benchmark2(int num_threads)
{
	for (int i = 0; i < NUM_TEST / num_threads; ++i)
	{
		int value = rand() % NUM_RANGE;
		switch (rand() % 3)
		{
		case 0:
			sl.lock();
			s.insert(value);
			sl.unlock();
			break;
		case 1:
			sl.lock();
			s.erase(value);
			sl.unlock();
			break;
		case 2:
			sl.lock();
			s.count(value);
			sl.unlock();
			break;
		}
	}
}

int main()
{
	for (int i = 1; i <= MAX_THREADS; i *= 2)
	{
		vector<thread> workers;
		list.Init();
		auto beg = high_resolution_clock().now();
		for(int j = 0;j<i;++j)
			workers.emplace_back(Benchmark, i);
		for (auto& t : workers)
			t.join();
		auto end = high_resolution_clock().now();
		list.Print20();
		auto duration = end - beg;
		cout << i << " threads, exec time = " << duration_cast<milliseconds>(duration).count() << endl;
	}
}