//#include"CQueue.h"
//
//using namespace std;
//using namespace chrono;
//
//constexpr int NUM_TEST = 10'000'000;
//constexpr int MAX_THREADS = 8;
//constexpr int NUM_RANGE = 1000;
//
//StampLFQueue myQueue;
//set<int> s;
//
//void Benchmark(int num_threads)
//{
//	const int loops = NUM_TEST / num_threads;
//	for (int i = 0; i < loops; ++i)
//	{
//		if ((0 == rand() % 2) || (i < 32 / num_threads))
//			myQueue.Enqueue(i);
//		else
//			myQueue.Dequeue();
//	}
//}
//
//mutex sl;
//
//void Benchmark2(int num_threads)
//{
//	for (int i = 0; i < NUM_TEST / num_threads; ++i)
//	{
//		int value = rand() % NUM_RANGE;
//		switch (rand() % 3)
//		{
//		case 0:
//			sl.lock();
//			s.insert(value);
//			sl.unlock();
//			break;
//		case 1:
//			sl.lock();
//			s.erase(value);
//			sl.unlock();
//			break;
//		case 2:
//			sl.lock();
//			s.count(value);
//			sl.unlock();
//			break;
//		}
//	}
//}
//
//int main()
//{
//	for (int i = 1; i <= MAX_THREADS; i *= 2)
//	{
//		vector<thread> workers;
//		myQueue.Init();
//		auto beg = high_resolution_clock().now();
//		for(int j = 0;j<i;++j)
//			workers.emplace_back(Benchmark, i);
//		for (auto& t : workers)
//			t.join();
//		auto end = high_resolution_clock().now();
//		myQueue.Print20();
//		auto duration = end - beg;
//		cout << i << " threads, exec time = " << duration_cast<milliseconds>(duration).count() << endl;
//	}
//}