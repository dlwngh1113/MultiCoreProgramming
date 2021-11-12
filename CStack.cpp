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
	while (true)
	{
		Node* ptr = top;
		Node* nNode = new Node{ x };
		nNode->next = ptr;
		if (!CAS(ptr, nNode))
		{
			bool timeOut = false;
			int ret = ary.Visit(x, &timeOut);
			if (!timeOut)
			{
				if (ret == -1)
					break;
				else
					continue;
			}
			else
				continue;
		}
		else
			break;
	}
}

int CStack::Pop()
{
	while (true)
	{
		Node* p = top;
		if (p == nullptr)
			return -2;
		if (CAS(p, p->next))
			return p->value;
		else
		{
			bool timeOut = false;
			int ret = ary.Visit(-1, &timeOut);
			if (!timeOut)
			{
				if (ret != -1)
					return ret;
				else
					continue;
			}
			else
				continue;
		}
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
