#pragma once
#include"stdafx.h"

class SPNode {
public:
	int value;
	std::shared_ptr<SPNode> next;
	std::mutex nodeLock;
	std::atomic<bool> marked;
	SPNode() :value{ 0 }, next{ nullptr }, marked{ false }{}
	SPNode(int x) :value{ x }, next{ nullptr }, marked{ false } {}
	virtual ~SPNode() {}
};

class SPLList
{
	std::shared_ptr<SPNode> head, tail;
public:
	SPLList();
	virtual ~SPLList();
	bool Validate(const std::shared_ptr<SPNode>& pre, const std::shared_ptr<SPNode>& cur);
	void Init();
	bool Add(int x);

	bool Remove(int x);

	bool Contains(int x);
	void Print20();
};

