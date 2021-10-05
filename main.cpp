//#include"stdafx.h"
//
//using namespace std;
//using namespace chrono;
//
//volatile int sum;
//
//atomic<unsigned char> flag[2] = { 0, 0 };
//volatile int victim = 0;
//
//void p_lock(int my_id)
//{
//	int other = 1 - my_id;
//	flag[my_id] = 1;
//	victim = my_id;
//	while ((flag[other].fetch_add(0) == 1) && my_id == victim);
//}
//
//void p_unlock(int my_id)
//{
//	flag[my_id] = false;
//}
//
//void worker(int threadNum, int my_id)
//{
//	const int loops = 50'000'000 / threadNum;
//	for (auto i = 0; i < loops; ++i) {
//		p_lock(my_id);
//		sum += 2;
//		p_unlock(my_id);
//	}
//}
//
//int main()
//{
//	for (int i = 2; i <= 2; i *= 2)
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