#include"stdafx.h"

using namespace std;

const auto SIZE = 50'000'000;
volatile int x, y;
int trace_x[SIZE], trace_y[SIZE];

void ThreadFunc1()
{
	for (int i = 0; i < SIZE; ++i)
	{
		x = i;
		atomic_thread_fence(memory_order_seq_cst);
		trace_y[i] = y;
	}
}

void ThreadFunc2()
{
	for (int i = 0; i < SIZE; ++i)
	{
		y = i;
		atomic_thread_fence(memory_order_seq_cst);
		trace_x[i] = x;
	}
}

int main()
{
	thread t1{ ThreadFunc1 };
	thread t2{ ThreadFunc2 };
	t1.join();
	t2.join();

	int cnt = 0;

	for (int i = 0; i < SIZE - 1; ++i)
	{
		if (trace_x[i] != trace_x[i + 1])continue;
		int x_value = trace_x[i];
		if (trace_y[x_value] != trace_y[x_value + 1])continue;
		if (i != trace_y[x_value])continue;
		++cnt;
	}
	cout << "Total Memory Inconsistency: " << cnt << endl;
}