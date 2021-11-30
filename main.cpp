#include"stdafx.h"
using namespace std;
using namespace chrono;

int main()
{
	atomic_int sum = 0;
	auto start = system_clock::now();
	for (int i = 0; i < NUM_TEST; ++i)
	{
		sum += 2;
	}
	auto end = system_clock::now();
	auto execTime = end - start;
	cout << "single thread sum = " << sum;
	cout << " exec time = " << duration_cast<milliseconds>(execTime).count() << endl;

	sum = 0;
	start = system_clock::now();
	constexpr int CHUNK = NUM_TEST / 8;
	tbb::parallel_for(0, NUM_TEST / CHUNK, 1, [&sum, &CHUNK](int i) {
		volatile int localSum = 0;
		for (int i = 0; i < CHUNK; ++i) localSum += 2;
		sum += localSum; });
	end = system_clock::now();
	execTime = end - start;
	cout << "tbb multi thread sum = " << sum;
	cout << " exec time = " << duration_cast<milliseconds>(execTime).count() << endl;

	sum = 0;
	start = system_clock::now();
	thread threads[4];
	for (auto& th : threads)
		th = thread{ [&sum](int num) {
		volatile int localSum = 0;
		for (int i = 0; i < NUM_TEST / num; ++i) localSum += 2;
		sum += localSum;
	}, MAX_THREADS };
	for (auto& th : threads)
		th.join();
	end = system_clock::now();
	execTime = end - start;
	cout << "multi thread sum = " << sum;
	cout << " exec time = " << duration_cast<milliseconds>(execTime).count() << endl;
}