#pragma once
#include"stdafx.h"

using namespace std;

class Node {
public:
	int value;
	Node* next;
	Node() : value{ 0 }, next{ nullptr } {}
	Node(int x) : value{ x }, next{ nullptr }{}
	~Node() {}
};

class CStack
{
	Node* volatile top;
	bool CAS(Node* old, Node* nNode);
public:
	CStack();
	~CStack();
	void Init();
	void Push(int x);
	int Pop();
	void Print20();
};