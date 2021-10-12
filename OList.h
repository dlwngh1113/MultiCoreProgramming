#pragma once
#include"FList.h"

class OList
{
	LNode head, tail;
public:
	void Init();
	OList();
	virtual ~OList();
	bool Validate(LNode* pre, LNode* cur);
	bool Add(int x);
	bool Remove(int x);
	bool Contains(int x);
	void Print20();
};

