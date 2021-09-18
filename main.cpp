#include<iostream>
#include<thread>
#include<mutex>
#include<chrono>
#include<vector>
#include<atomic>

using namespace std;
using namespace chrono;


atomic <int> sum{ 0 };
mutex myLock;

void worker(int threadNum)
{
	const int loops = 50'000'000 / threadNum;
	volatile int sSum{ 0 };
	for (auto i = 0; i < loops; ++i) {
		sSum += 2;
	}
	sum += sSum;
}

int main()
{
	for (int i = 1; i <= 8; i *= 2)
	{
		sum = 0;
		vector<thread> workers;
		auto beg = high_resolution_clock::now();
		for (int j = 0; j < i; ++j)
		{
			workers.emplace_back(worker, i);
		}
		for (auto& t : workers)
		{
			t.join();
		}
		auto end = high_resolution_clock::now();
		cout << "thread count: " << i << "	milliseconds: " << duration_cast<milliseconds>(end - beg).count() << "msecs";
		cout << "	Sum = " << sum << endl;
	}
}