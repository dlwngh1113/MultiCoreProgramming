#include<iostream>
#include<thread>

using namespace std;

int sum{ 0 };

void worker()
{
	for (auto i = 0; i < 25'000'000; ++i)
	{
		sum += 2;
		sum += 2;
		sum += 2;
		sum += 2;
		sum += 2;
		sum += 2;
		sum += 2;
		sum += 2;
		sum += 2;
		sum += 2;
	}
}

int main()
{
	thread t1{ worker };
	thread t2{ worker };
	t1.join();
	t2.join();

	cout << "Sum = " << sum << endl;
}