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
		if (limit != 0)
			delay = rand() % limit;
		limit *= 2;
		if (limit > maxDelay)
			limit = maxDelay;
		int start, current;
		_asm RDTSC;	//cpu에 존재하는 1클럭에 1증가하는 카운터(RD Time Stamp Counter)
		_asm mov start, eax;
		do {
			_asm RDTSC;
			_asm mov current, eax;
		} while ((current - start) < delay);
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