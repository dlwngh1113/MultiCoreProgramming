#include<iostream>
#include<thread>
#include<mutex>

using namespace std;

volatile bool g_ready = false;
int g_data = 0;

void Receiver()
{
	while (false == g_ready);
	cout << "got data: " << g_data << endl;
}

void Sender()
{
	g_data = 999;
	g_ready = true;
	cout << "sender finished" << endl;
}

int main()
{
	thread t1{ Receiver };
	thread t2{ Sender };

	t1.join();
	t2.join();
}