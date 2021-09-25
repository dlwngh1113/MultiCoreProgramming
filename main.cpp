//#include<iostream>
//#include<thread>
//#include<mutex>
//#include<chrono>
//#include<vector>
//#include<atomic>
//
//using namespace std;
//using namespace chrono;
//
//const int MAX_THREAD = 16;
//
//volatile int sum[MAX_THREAD * 16];
//mutex myLock;
//
//void worker(int threadNum, const int thread_id)
//{
//	const int loops = 50'000'000 / threadNum;
//	for (auto i = 0; i < loops; ++i) {
//		sum[thread_id * 16] += 2;
//	}
//}
//
//int main()
//{
//	for (int i = 1; i <= MAX_THREAD; i *= 2)
//	{
//		int total_sum{ 0 };
//		for (auto& v : sum)
//			v = 0;
//		vector<thread> workers;
//		auto beg = high_resolution_clock::now();
//		for (int j = 0; j < i; ++j)
//			workers.emplace_back(worker, i, j);
//		for (auto& t : workers)
//			t.join();
//		for (auto& v : sum)
//			total_sum += v;
//		auto end = high_resolution_clock::now();
//		cout << "thread count:" << i << "	milliseconds: " << duration_cast<milliseconds>(end - beg).count() << "msecs";
//		cout << "	Sum = " << total_sum << endl;
//	}
//}