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

class StampNode {
public:
	Node* volatile ptr;
	int volatile stamp;
};

class StampLFQueue
{
	StampNode head, tail;
	bool CAS(Node* volatile * addr, Node* old, Node* newNode);
	bool StampCAS(StampNode* addr, Node* old_node, Node* new_node, int old_stamp, int new_stamp);
public:
	StampLFQueue();
	virtual ~StampLFQueue();
	void Init();
	void Enqueue(int x);
	int Dequeue();
	void Print20();
};

