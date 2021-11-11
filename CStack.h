#pragma once
#include"stdafx.h"

using namespace std;

class Node {
public:
	int value;
	Node* next;
	Node() : value{ 0 }, next{ nullptr } {}
	Node(int x) : value{ x }, next{ nullptr }{}
	~Node() {}
};

class BackOff {
	int minDelay, maxDelay;
	int limit;
public:
	BackOff(int min, int max)
		: minDelay(min), maxDelay(max), limit(min) {}
	void InterruptedException() {
		int delay = 0;
		if (limit != 0) delay = rand() % limit;
		limit *= 2;
		if (limit > maxDelay) limit = maxDelay;
		this_thread::sleep_for(chrono::microseconds(delay));;
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