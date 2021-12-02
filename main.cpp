#include"CList.h"

using namespace std;
using namespace chrono;

CList myList;

void Benchmark(int num_threads)
{
	for (int i = 0; i < NUM_TEST / num_threads; ++i)
	{
		int value = rand() % NUM_RANGE;
		switch (rand() % 100)
		{
		case 0:
			myList.Add(value);
			break;
		case 1:
			myList.Remove(value);
			break;
		default:
			myList.Contains(value);
			break;
		}
	}
}

int main()
{
	for (int i = 1; i <= MAX_THREADS; i *= 2)
	{
		vector<thread> workers;
		myList.Init();
		auto beg = high_resolution_clock().now();
		for (int j = 0; j < i; ++j)
			workers.emplace_back(Benchmark, i);
		for (auto& t : workers)
			t.join();
		auto end = high_resolution_clock().now();
		myList.Print20();
		auto duration = end - beg;
		cout << i << " threads, exec time = " << duration_cast<milliseconds>(duration).count() << endl;
	}
}