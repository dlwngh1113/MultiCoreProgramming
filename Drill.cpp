//#include<iostream>
//#include<mutex>
//#include<thread>
//#include<chrono>
//#include<atomic>
//#include<vector>
//
//using namespace std;
//using namespace chrono;
//
//constexpr int MAX_THREADS = 8;
//volatile int sum;
//
//volatile int LOCK = 0;
//mutex myLock;
//
//void CAS_LOCK()
//{
//	int expected = 0;
//	while (true)
//	{
//		if (atomic_compare_exchange_strong(reinterpret_cast<volatile atomic<int>*>(&LOCK), &expected, 1))
//			return;
//		expected = 0;
//	}
//}
//
//void CAS_UNLOCK()
//{
//	int expected = 1;
//	while (true)
//	{
//		if (atomic_compare_exchange_strong(reinterpret_cast<volatile atomic<int>*>(&LOCK), &expected, 0))
//			return;
//		expected = 1;
//	}
//}
//
//void worker(int threadNum, int my_id)
//{
//	const int loops = 50'000'000 / threadNum;
//	for (auto i = 0; i < loops; ++i) {
//		CAS_LOCK();
//		sum = sum + 2;
//		CAS_UNLOCK();
//	}
//}
//
//int main()
//{
//	for (int i = 1; i <= MAX_THREADS; i *= 2)
//	{
//		sum = 0;
//		vector<thread> workers;
//		auto beg = high_resolution_clock::now();
//		for (int j = 0; j < i; ++j)
//			workers.emplace_back(worker, i, j);
//		for (auto& t : workers)
//			t.join();
//		auto end = high_resolution_clock::now();
//		cout << "thread count:" << i << "	milliseconds: " << duration_cast<milliseconds>(end - beg).count() << "msecs";
//		cout << "	Sum = " << sum << endl;
//	}
//}