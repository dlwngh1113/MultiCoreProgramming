#pragma once
#include"LockFreeExchanger.h"

using namespace std;

class Node {
public:
	int value;
	Node* next;
	Node() : value{ 0 }, next{ nullptr } {}
	Node(int x) : value{ x }, next{ nullptr }{}
	~Node() {}
};

class EliminationArray {
	int range;
	int numThreads;
	LockFreeExchanger exchanger[MAX_THREADS];
public:
	EliminationArray(int numThreads) : numThreads{ numThreads }, range{ 1 } { }
	~EliminationArray() {}
	int Visit(int value, bool* time_out) {
		int slot = rand() % range;
		bool busy;
		int ret = exchanger[slot].Exchange(value, time_out, &busy);
		if ((true == *time_out) && (range > 1)) range--;
		if ((true == busy) && (range <= numThreads / 2)) range++;
		// MAX RANGE is # of thread / 2
	}
};

class CStack
{
	Node* volatile top;
	bool CAS(Node* old, Node* nNode);
public:
	CStack();
	~CStack();
	void Init();
	void Push(int x);
	int Pop();
	void Print20();
};