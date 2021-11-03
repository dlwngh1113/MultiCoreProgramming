#include<iostream>
#include<set>
#include<vector>
#include<thread>
#include<chrono>

using namespace std;
using namespace chrono;

constexpr int MAX_THREAD = 8;
constexpr int NUM_TEST = 4'000;
constexpr int NUM_RANGE = 1000;

enum MethodType{DEQUE, ENQUE, CLEAR};
typedef int InputValue;
typedef int Response;

struct Invocation {
	MethodType type;
	InputValue value;
};

class Node;

class Consensus {
	int first = -1;
	std::atomic<int> response = first;
public:
	void Init()
	{
		first = response = -1;
	}
	bool isUsed()
	{
		return response != -1;
	}
	Node* Decide(Node*& node)
	{
		if (atomic_compare_exchange_strong(&response, &first, reinterpret_cast<int>(node)))
			return node;
		else
			return reinterpret_cast<Node*>(response.load());
	}
};

class Node {
public:
	Invocation invoc;
	Consensus decideNext;
	Node* next;
	volatile int seq;
	Node() :seq{ 0 }, next{ nullptr }{}
	~Node() {}
	Node(const Invocation& input)
	{
		invoc = input;
		next = nullptr;
		seq = 0;
	}
	void Init()
	{
		decideNext.Init();
		next = nullptr;
		seq = 1;
	}
};

class SeqObject {
	std::set<int> s;
public:
	Response Apply(Invocation invoc)
	{
		int res = -1;
		switch (invoc.type)
		{
		case ENQUE:
			s.insert(invoc.value);
			break;
		case DEQUE:
			res = s.erase(invoc.value);
			break;
		case CLEAR:
			s.clear();
			break;
		}
		return res;
	}
};

class LFUniversal {
	Node* head[MAX_THREAD];
	Node* announce[MAX_THREAD];
	Node tail;
	Node* max()
	{
		for (int i = 0; i < MAX_THREAD; ++i)
		{
			if (!head[i]->decideNext.isUsed())
				return head[i];
		}
	}
public:
	LFUniversal()
	{
		tail.seq = 1;
		for (int i = 0; i < MAX_THREAD; ++i)
		{
			head[i] = &tail;
			announce[i] = &tail;
		}
	}
	~LFUniversal()
	{
		Init();
	}
	void Init()
	{
		for (int i = 0; i < MAX_THREAD; ++i)
		{
			head[i] = &tail;
			announce[i] = &tail;
		}

		Node* p = tail.next ? tail.next : &tail;
		while (p->next)
		{
			Node* tmp = p;
			p = p->next;
			delete tmp;
		}
		if (&tail != p)
			delete p;

		tail.Init();
	}
	Response Apply(Invocation invoc, const int threadID)
	{
		int i = threadID;
		announce[i] = new Node(invoc);
		head[i] = max();
		while (announce[i]->seq == 0)
		{
			Node* before = head[i];
			Node* help = announce[(before->seq + 1) % MAX_THREAD];
			Node* prefer;
			if (help->seq == 0)
				prefer = help;
			else
				prefer = announce[i];
			Node* after = before->decideNext.Decide(prefer);
			before->next = after;
			after->seq = before->seq + 1;
			head[i] = after;
		}
		SeqObject myObject;
		Node* cur = tail.next;
		while (cur != announce[i])
		{
			myObject.Apply(cur->invoc);
			cur = cur->next;
		}
		head[i] = announce[i];
		return myObject.Apply(cur->invoc);
	}
};

LFUniversal lf;

void Benchmark(int num_threads, int threadID)
{
	const int loops = NUM_TEST / num_threads;
	for (int i = 0; i < loops; ++i)
	{
		Invocation invoc;
		invoc.value = rand() % NUM_RANGE;
		switch (rand() % 3)
		{
		case 0:
			invoc.type = ENQUE;
			break;
		case 1:
			invoc.type = DEQUE;
			break;
		case 2:
			invoc.type = CLEAR;
			break;
		}
		lf.Apply(invoc, threadID);
	}
}

int main()
{
	for (int i = 1; i < MAX_THREAD; i *= 2)
	{
		lf.Init();
		vector<thread> workers;
		auto beg = high_resolution_clock().now();
		for (int j = 0; j < i; ++j)
			workers.emplace_back(Benchmark, MAX_THREAD, j);
		for (auto& t : workers)
			t.join();
		auto end = high_resolution_clock().now();
		auto duration = end - beg;
		cout << i << " threads, exec time = " << duration_cast<milliseconds>(duration).count() << endl;
	}
}