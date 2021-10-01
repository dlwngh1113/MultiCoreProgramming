//#include"stdafx.h"
//
//using namespace std;
//using namespace chrono;
//
//volatile int sum;
//
//constexpr int MAX_THREADS = 8;
//
//volatile bool flag[MAX_THREADS];
//volatile int label[MAX_THREADS];
//
//void Init()
//{
//	for (int i = 0; i < MAX_THREADS; ++i)
//	{
//		flag[i] = false;
//		label[i] = 0;
//	}
//}
//
//void lock(int threadNum, int my_id)
//{
//	flag[my_id] = true;
//	label[my_id] = max(label[0], label[threadNum]) + 1;
//	bool tmpFlag = true;
//	for (auto i = 0; i < threadNum; ++i)
//	{
//		while (tmpFlag)
//		{
//			if (flag[i] && (label[i] < label[my_id]))
//				tmpFlag = true;
//			else if (flag[i] && (label[i] == label[my_id]) && (i < my_id))
//				tmpFlag = true;
//			else
//				break;
//		}
//	}
//}
//
//void unlock(int my_id)
//{
//	flag[my_id] = false;
//}
//
//void worker(int threadNum, int my_id)
//{
//	const int loops = 5'000'000 / threadNum;
//	for (auto i = 0; i < loops; ++i) {
//		lock(threadNum, my_id);
//		sum += 2;
//		unlock(my_id);
//	}
//}
//
//int main()
//{
//	for (int i = 1; i <= MAX_THREADS; i *= 2)
//	{
//		sum = 0;
//		Init();
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