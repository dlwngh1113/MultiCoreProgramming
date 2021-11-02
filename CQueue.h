#pragma once
#include"stdafx.h"

class Node {
public:
	int value;
	Node* volatile next;
	Node() :value{ 0 }, next{ nullptr }{}
	Node(int x) :value{ x }, next{ nullptr }{}
	virtual ~Node() {}
};

class CQueue
{
	Node* volatile head;
	Node* volatile tail;
	bool CAS(Node* volatile * addr, Node* old, Node* newNode);
public:
	CQueue();
	virtual ~CQueue();
	void Enqueue(int x);
	void Init();
	int Dequeue();
	void Print20();
};

