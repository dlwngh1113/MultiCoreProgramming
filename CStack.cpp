#include "CStack.h"

CStack::CStack()
{
	top = nullptr;
}

CStack::~CStack()
{
	Init();
}

void CStack::Init()
{
	while (top != nullptr)
	{
		Node* p = top;
		top = top->next;
		delete p;
	}
}

void CStack::Push(int x)
{
	Node* nNode = new Node{ x };
	topLock.lock();
	nNode->next = top;
	top = nNode;
	topLock.unlock();
}

int CStack::Pop()
{
	topLock.lock();
	if (top == nullptr) {
		topLock.unlock();
		return -2;
	}
	Node* p = top;
	top = top->next;
	int val = p->value;
	delete p;
	topLock.unlock();
	return val;
}

void CStack::Print20()
{
	Node* p = top;
	for (int i = 0; i < 20; ++i)
	{
		cout << top->value << ", ";
		top = top->next;
	}
	cout << endl;
}
