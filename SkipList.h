#pragma once
#include"stdafx.h"

constexpr int MAX_LEVEL = 10;

class LFNode;

class MPTR {
	std::atomic<int> V;
public:
	MPTR() : V{ 0 } {}
	virtual ~MPTR() {}

	void set_ptr(LFNode* p)
	{
		V = reinterpret_cast<int>(p);
	}
	LFNode* get_ptr()
	{
		return reinterpret_cast<LFNode*>(V & 0xFFFFFFFE);
	}
	bool set_removed(LFNode*& p, bool b)
	{
		int tmpV = reinterpret_cast<int>(p) | 0x01;
		int curV = reinterpret_cast<int>(p) & 0xFFFFFFFE;
		return std::atomic_compare_exchange_strong(&V, &curV, tmpV);
	}
	bool get_removed(LFNode*& p)
	{
		int curV = V;
		p = reinterpret_cast<LFNode*>(curV & 0xFFFFFFFE);
		return (curV & 0x1) == 1;
	}
	bool CAS(LFNode* old_p, LFNode* new_p, bool old_r, bool new_r)
	{
		int old_v = reinterpret_cast<int>(old_p);
		if (old_r)old_v++;
		int new_v = reinterpret_cast<int>(new_p);
		if (new_r)new_v++;
		return std::atomic_compare_exchange_strong(&V, &old_v, new_v);
	}
};

class LFNode {
public:
	int value;
	MPTR next[MAX_LEVEL + 1];
	int topLevel;
	LFNode() :value{ 0 }, topLevel{ 0 } {}
	LFNode(int x, int level) :value{ x }, topLevel{ level } {}
	virtual ~LFNode() {}
};

class SkipList
{
	LFNode head, tail;
	int RandomLevel();
public:
	SkipList();
	virtual ~SkipList();

	void Init();
	bool Add(int x);
	bool Remove(int x);
	bool Contains(int x);
	bool Find(int x, LFNode* pre[], LFNode* cur[]);
	void Print20();
};

