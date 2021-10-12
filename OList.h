#pragma once
#include"FList.h"

class OList
{
	FNode head, tail;
public:
	void Init();
	OList();
	virtual ~OList();
	bool Validate(FNode* pre, FNode* cur);
	bool Add(int x);
	bool Remove(int x);
	bool Contains(int x);
	void Print20();
};

