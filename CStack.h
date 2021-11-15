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
	EliminationArray() :numThreads{ MAX_THREADS }, range{ 1 }{}
	~EliminationArray() {}
	int Visit(int value, bool* time_out) {
		int slot = rand() % range;
		bool busy{ false };
		int ret = exchanger[slot].Exchange(value, time_out, &busy);
		if ((*time_out) && (range > 1)) --range;
		if (busy && (range <= numThreads / 2)) ++range;
		return ret;
	}
};

class CStack
{
	Node* volatile top;
	bool CAS(Node* old, Node* nNode);
	EliminationArray ary;
public:
	CStack();
	~CStack();
	void Init();
	void Push(int x);
	int Pop();
	void Print20();
};