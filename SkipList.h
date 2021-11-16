#pragma once
#include"stdafx.h"

constexpr int MAX_LEVEL = 10;

class SKNode {
public:
	int value;
	SKNode* next[MAX_LEVEL + 1];
	int topLevel;
	SKNode() :value{ 0 }, topLevel{ 0 }
	{
		for (auto& n : next)
			n = nullptr;
	}
	SKNode(int x, int top) :value{ x }, topLevel{ top } 
	{
		for (auto& n : next)
			n = nullptr;
	}
	virtual ~SKNode() {}
};

class SkipList
{
	SKNode head, tail;
	std::mutex Lock;
public:
	SkipList();
	virtual ~SkipList();

	void Init();
	bool Add(int x);
	bool Remove(int x);
	void Find(int x, SKNode* pre[], SKNode* cur[]);
	void Print20();
};

