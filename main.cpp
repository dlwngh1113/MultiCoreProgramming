#include<iostream>
#include<thread>
#include<mutex>
#include<chrono>
#include<vector>

using namespace std;
using namespace chrono;

int sum{ 0 };
mutex myLock;

void worker(int threadNum)
{
	for (auto i = 0; i < 50'000'000 / threadNum; ++i) {
		sum += 2;
	}
}

int main()
{
	const int threadNum = 8;
	vector<thread> workers;
	auto beg = high_resolution_clock::now();
	for (int i = 0; i < threadNum; ++i)
	{
		workers.emplace_back(worker , threadNum);
	}
	for(auto& t : workers)
	{
		t.join();
	}
	auto end = high_resolution_clock::now();
	cout << duration_cast<milliseconds>(end - beg).count() << "msecs\n";

	cout << "Sum = " << sum << endl;
}