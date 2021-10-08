#pragma once
#include"stdafx.h"

class CNode {
public:
	int value;
	CNode* next;
	CNode() :value{ 0 }, next{ nullptr }{}
	CNode(int x) :value{ x }, next{ nullptr } {}
	virtual ~CNode() {}
};

class CList {
	CNode head, tail;
	std::mutex Lock;
public:
	CList();
	virtual ~CList();

	void Init();
	bool Add(int x);
	bool Remove(int x);
	bool Contains(int x);
	void Print20();
};