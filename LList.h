#pragma once
#include"stdafx.h"

class LNode {
public:
	int value;
	LNode* next;
	std::mutex nodeLock;
	std::atomic<bool> marked;
	LNode() :value{ 0 }, next{ nullptr }, marked{ false }{}
	LNode(int x) :value{ x }, next{ nullptr }, marked{ false } {}
	virtual ~LNode() {}
};

class LList
{
	LNode head, tail;
public:
	LList();
	virtual ~LList();
	bool Validate(LNode* pre, LNode* cur);
	void Init();
	bool Add(int x);

	bool Remove(int x);

	bool Contains(int x);
	void Print20();
};

