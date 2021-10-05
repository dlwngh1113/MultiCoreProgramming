#include"stdafx.h"

using namespace std;

volatile bool done = false;
volatile int* bound;
int error{ 0 };
mutex l;

void Threadfunc1()
{
	for (int i = 0; i < 25'000'000; ++i)
	{
		*bound = -(1 + *bound);
	}
	done = true;
}

void Threadfunc2()
{
	while (false == done)
	{
		int v = *bound;
		if (v != 0 && v != -1) {
			cout << hex << v << '\t';
			++error;
		}
	}
}

int main()
{
	int arr[64];
	long long temp = reinterpret_cast<long long>(arr + 16);
	temp = temp & 0xFFFFFFFFFFFFFFC0;
	temp -= 2;
	bound = reinterpret_cast<int*>(temp);
	*bound = 0;
	thread t1{ Threadfunc1 };
	thread t2{ Threadfunc2 };
	t1.join();
	t2.join();
	cout << "error count: " << error << endl;
}