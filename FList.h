#pragma once
#include"stdafx.h"

class FNode {
public:
	int value;
	FNode* next;
	std::mutex nodeLock;
	FNode() :value{ 0 }, next{ nullptr }{}
	FNode(int x) :value{ x }, next{ nullptr } {}
	virtual ~FNode() {}
};

class FList {
	FNode head, tail;
public:
	FList();
	~FList();
	void Init();
	bool Add(int x);

	bool Remove(int x);

	bool Contains(int x);
	void Print20();
};

