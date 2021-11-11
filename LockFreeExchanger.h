#pragma once
#include"stdafx.h"
constexpr int EMPTY = 0;
constexpr int WAITING = 1;
constexpr int BUSY = 2;

class LockFreeExchanger
{
	std::atomic<unsigned int> slot;
public:
	int Exchange(int value, bool* timeOut, bool* busy);
};

