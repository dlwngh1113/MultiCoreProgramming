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
		if (0 != limit) delay = rand() % limit;
		if (0 == delay) return;
		limit += limit;
		if (limit > maxDelay) limit = maxDelay;
		_asm mov eax, delay;
	myloop:
		_asm dec eax
		_asm jnz myloop;
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