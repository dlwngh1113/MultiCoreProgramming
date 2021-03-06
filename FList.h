#pragma once
#include"stdafx.h"

class LNode {
public:
	int value;
	LNode* next;
	std::mutex nodeLock;
	LNode() :value{ 0 }, next{ nullptr }{}
	LNode(int x) :value{ x }, next{ nullptr } {}
	virtual ~LNode() {}
};

class FList {
	LNode head, tail;
public:
	FList();
	~FList();
	void Init();
	bool Add(int x);

	bool Remove(int x);

	bool Contains(int x);
	void Print20();
};

