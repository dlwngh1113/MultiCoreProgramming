#include<iostream>
#include<thread>

using namespace std;

int sum{ 0 };

int main()
{
	for (auto i = 0; i < 50'000'000; ++i)
		sum += 2;
	cout << "Sum = " << sum << endl;
}