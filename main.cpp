#include"SkipList.h"

using namespace std;
using namespace chrono;

int main()
{
	int sum{ 0 };
#pragma omp parallel
	{
		int num_threads = omp_get_num_threads();
		for (int i = 0; i < 50'000'000 / num_threads; ++i)
#pragma omp critical
			sum += 2;
	}
	cout << "Sum = " << sum << endl;
}