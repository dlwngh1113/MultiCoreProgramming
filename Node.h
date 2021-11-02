#
#include<iostream>

enum MethodType { DEQUE, ENQUE, CLEAR };
typedef int InputValue;
typedef int Response;

struct Invocation {
	MethodType type;
	InputValue value;
};

class Node {
public:
	Invocation invoc;
	Consensus decideNext;
	Node* next;
	volatile int seq;
	Node() :seq{ 0 }, next{ nullptr }{}
	~Node() {}
	Node(const Invocation& input)
	{
		invoc = input;
		next = nullptr;
		seq = 0;
	}
};