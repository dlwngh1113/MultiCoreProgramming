#include"stdafx.h"

using namespace std;
using namespace chrono;

const int MAX_THREAD = 8;

volatile int sum;
mutex myLock;

void worker(int threadNum)
{
	const int loops = 5'000'000 / threadNum;
	for (auto i = 0; i < loops; ++i) {
		myLock.lock();
		sum += 2;
		myLock.unlock();
	}
}

int main()
{
	for (int i = 1; i <= MAX_THREAD; i *= 2)
	{
		sum = 0;
		vector<thread> workers;
		auto beg = high_resolution_clock::now();
		for (int j = 0; j < i; ++j)
			workers.emplace_back(worker, i);
		for (auto& t : workers)
			t.join();
		auto end = high_resolution_clock::now();
		cout << "thread count:" << i << "	milliseconds: " << duration_cast<milliseconds>(end - beg).count() << "msecs";
		cout << "	Sum = " << sum << endl;
	}
}