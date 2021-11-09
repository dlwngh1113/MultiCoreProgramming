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

class nullmutex {
public:
	void lock() {}
	void unlock() {}
};

class CStack
{
	Node* top;
	nullmutex topLock;
public:
	CStack();
	~CStack();
	void Init();
	void Push(int x);
	int Pop();
	void Print20();
};

