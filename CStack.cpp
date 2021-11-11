#include "CStack.h"

bool CStack::CAS(Node* old, Node* nNode)
{
	return atomic_compare_exchange_strong(
		reinterpret_cast<volatile atomic_int*>(&top), 
		reinterpret_cast<int*>(&old), 
		reinterpret_cast<int>(nNode));
}

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
	BackOff bo{ 1, 100 };
	while (true)
	{
		Node* ptr = top;
		Node* nNode = new Node{ x };
		nNode->next = ptr;
		if (!CAS(ptr, nNode))
		{
			delete nNode;
			bo.InterruptedException();
		}
		else
			break;
	}
}

int CStack::Pop()
{
	BackOff bo{ 1, 100 };
	while (true)
	{
		Node* p = top;
		if (p == nullptr)
			return -2;
		if (CAS(p, p->next))
			return p->value;
		else
			bo.InterruptedException();
	}
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
