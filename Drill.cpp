//#include<iostream>
//#include<set>
//#include<vector>
//#include<thread>
//#include<chrono>
//#include<mutex>
//#include<iomanip>
//
//using namespace std::chrono;
//
//constexpr int MAX_THREAD = 4;
//constexpr int NUM_TEST = 4'000;
//constexpr int NUM_RANGE = 1000;
//
//enum MethodType{DEQUE, ENQUE, CLEAR};
//typedef int InputValue;
//typedef int Response;
//
//std::string enum_str[3] = {
//	"DEQUE",
//	"ENQUE",
//	"CLEAR"
//};
//
//struct Invocation {
//	MethodType type;
//	InputValue value;
//};
//
//class Node;
//
//class Consensus {
//	int first = -1;
//	std::atomic<int> response = first;
//public:
//	void Init()
//	{
//		first = -1;
//		response = -1;
//	}
//	Node* Decide(Node*& node)
//	{
//		if (atomic_compare_exchange_strong(&response, &first, reinterpret_cast<int>(node)))
//			return node;
//		else
//			return reinterpret_cast<Node*>(response.load());
//	}
//};
//
//class Node {
//public:
//	Invocation invoc;
//	Consensus decideNext;
//	Node* next;
//	volatile int seq;
//	Node() :seq{ 0 }, next{ nullptr }
//	{
//		decideNext.Init();
//	}
//	~Node() {}
//	Node(const Invocation& input)
//	{
//		invoc = input;
//		next = nullptr;
//		seq = 0;
//	}
//	void Init()
//	{
//		decideNext.Init();
//		next = nullptr;
//		seq = 1;
//	}
//};
//
//class SeqObject {
//	std::set<int> s;
//public:
//	Response Apply(Invocation invoc)
//	{
//		int res = -1;
//		switch (invoc.type)
//		{
//		case ENQUE:
//			s.insert(invoc.value);
//			break;
//		case DEQUE:
//			res = s.erase(invoc.value);
//			break;
//		case CLEAR:
//			s.clear();
//			break;
//		}
//		return res;
//	}
//};
//
//class LFUniversal {
//	Node* head[MAX_THREAD];
//	Node tail;
//	Node* max()
//	{
//		Node* max = head[0];
//		for (int i = 0; i < MAX_THREAD; ++i)
//		{
//			if (head[i]->seq > max->seq)
//				max = head[i];
//		}
//		return max;
//	}
//public:
//	LFUniversal()
//	{
//		tail.seq = 1;
//		for (int i = 0; i < MAX_THREAD; ++i)
//			head[i] = &tail;
//	}
//	~LFUniversal()
//	{
//		Init();
//	}
//	void Init()
//	{
//		Node* p = tail.next;
//		if (p == nullptr)
//			return;
//
//		while (p->next)
//		{
//			Node* tmp = p;
//			p = p->next;
//			delete tmp;
//		}
//		delete p;
//
//		for (int i = 0; i < MAX_THREAD; ++i)
//			head[i] = &tail;
//
//		tail.Init();
//	}
//	Response Apply(Invocation invoc, const int threadID)
//	{
//		int i = threadID;
//		Node* prefer = new Node(invoc);
//		while (prefer->seq == 0)
//		{
//			Node* before = max();
//			Node* after = before->decideNext.Decide(prefer);
//			before->next = after;
//			after->seq = before->seq + 1;
//			head[i] = after;
//		}
//		SeqObject myObject;
//		Node* cur = tail.next;
//		while (cur != prefer)
//		{
//			myObject.Apply(cur->invoc);
//			cur = cur->next;
//		}
//		return myObject.Apply(cur->invoc);
//	}
//	void print20()
//	{
//		Node* p = tail.next;
//		for (int i = 0; i < 20; ++i)
//		{
//			std::cout << std::setw(10) << enum_str[p->invoc.type] << ", " << p->invoc.value;
//			p = p->next;
//		}
//		std::cout << std::endl;
//	}
//};
//
//LFUniversal lf;
//
//void Benchmark(int num_threads, int threadID)
//{
//	const int loops = NUM_TEST / num_threads;
//	for (int i = 0; i < loops; ++i)
//	{
//		Invocation invoc;
//		invoc.value = rand() % NUM_RANGE;
//		switch (rand() % 3)
//		{
//		case 0:
//			invoc.type = ENQUE;
//			break;
//		case 1:
//			invoc.type = DEQUE;
//			break;
//		case 2:
//			invoc.type = CLEAR;
//			break;
//		}
//		lf.Apply(invoc, threadID);
//	}
//}
//
//std::mutex sl;
//std::set<int> s;
//
//void Benchmark2(int num_threads, int threadID)
//{
//	for (int i = 0; i < NUM_TEST / num_threads; ++i)
//	{
//		int value = rand() % NUM_RANGE;
//		switch (rand() % 3)
//		{
//		case 0:
//			sl.lock();
//			s.insert(value);
//			sl.unlock();
//			break;
//		case 1:
//			sl.lock();
//			s.erase(value);
//			sl.unlock();
//			break;
//		case 2:
//			sl.lock();
//			s.count(value);
//			sl.unlock();
//			break;
//		}
//	}
//}
//
//int main()
//{
//	srand((unsigned int)time(NULL));
//	for (int i = 1; i <= MAX_THREAD; i *= 2)
//	{
//		lf.Init();
//		std::vector<std::thread> workers;
//		auto beg = high_resolution_clock().now();
//		for (int j = 0; j < i; ++j)
//			workers.emplace_back(Benchmark, MAX_THREAD, j);
//		for (auto& t : workers)
//			t.join();
//		auto end = high_resolution_clock().now();
//		auto duration = end - beg;
//		std::cout << i << " threads, exec time = " << duration_cast<milliseconds>(duration).count() << std::endl;
//		lf.print20();
//	}
//}