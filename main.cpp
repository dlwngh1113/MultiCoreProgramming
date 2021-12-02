#include"stdafx.h"

using namespace std;
using namespace chrono;
using namespace tbb::detail::d1;

class FibTask : public task {
public:
	const long n;
	long* const sum;
	FibTask(long n_, long* sum_) : n(n_), sum(sum_) {}
	task* execute() { // Overrides virtual function task::execute
		if (n <= 2) {
			*sum = SerialFib(n);
		}
		else {
			long x, y;
			FibTask& a = *new(allocate_child()) FibTask(n - 1, &x);
			FibTask& b = *new(allocate_child()) FibTask(n - 2, &y);
			// Set ref_count to "two children plus one for the wait".
			set_ref_count(3);
			// Start b running.
			spawn(b);
			// Start a running and wait for all children (a and b).
			spawn_and_wait_for_all(a);
			// Do the sum
			*sum = x + y;
		}
		return NULL;
	}
};

int SerialFib(int n);

int SerialFib(int n)
{
	if (n <= 2)
		return n;
	else
		return SerialFib(n - 1) + SerialFib(n - 2);
}

int main()
{
	constexpr int F_INDEX = 45;
	volatile int fibo{ 0 };
	auto start = system_clock::now();
	fibo = SerialFib(F_INDEX);
	auto end = system_clock::now();
	auto duration = end - start;
	cout << "single thread fibo[" << F_INDEX << "] - " << fibo;
	cout << " Exec time = " << duration_cast<milliseconds>(duration).count() << endl;
}