#include"stdafx.h"

using namespace std;

volatile bool done = false;
volatile int* bound;
int error{ 0 };

void Threadfunc1()
{
	for (int i = 0; i < 25'000'000; ++i)
		*bound = -(1 + *bound);
	done = true;
}

void Threadfunc2()
{
	while (false == done)
	{
		int v = *bound;
		if (v != 0 && v != -1)
			++error;
	}
}

int main()
{
	bound = new int{ 0 };
	thread t1{ Threadfunc1 };
	thread t2{ Threadfunc2 };
	t1.join();
	t2.join();
	cout << "error count: " << error << endl;
	delete bound;
}