#pragma once
#include"stdafx.h"

class Node {
public:
	int value;
	Node* next;
	Node() :value{ 0 }, next{ nullptr }{}
	Node(int x) :value{ x }, next{ nullptr }{}
	virtual ~Node() {}
};

class CQueue
{
	Node* head, *tail;
	std::mutex enqLock, deqLock;
public:
	CQueue();
	virtual ~CQueue();
	void Enqueue(int x);
	void Init();
	int Dequeue();
	void Print20();
};

