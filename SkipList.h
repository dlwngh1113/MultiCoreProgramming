#pragma once
#include"stdafx.h"

constexpr int MAX_LEVEL = 10;

class SKNode {
public:
	int value;
	std::atomic_bool isMarked;
	std::atomic_bool fullyLinked;
	std::recursive_mutex nodeLock;
	SKNode* next[MAX_LEVEL + 1];
	int topLevel;
	SKNode() :value{ 0 }, topLevel{ 0 }, isMarked{ false }, fullyLinked{ false }
	{
		for (auto& n : next)
			n = nullptr;
	}
	SKNode(int x, int top) :value{ x }, topLevel{ top }, isMarked{ false }, fullyLinked{ false }
	{
		for (auto& n : next)
			n = nullptr;
	}
	virtual ~SKNode() {}
};

class SkipList
{
	SKNode head, tail;
public:
	SkipList();
	virtual ~SkipList();

	void Init();
	bool Add(int x);
	bool Remove(int x);
	bool Contains(int x);
	int Find(int x, SKNode* pre[], SKNode* cur[]);
	void Print20();
};

