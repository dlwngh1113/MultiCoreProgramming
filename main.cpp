#include"stdafx.h"
using namespace std;
using namespace chrono;

string words[NUM_TEST];

int main()
{
	for (int i = 0; i < NUM_TEST; ++i)
	{
		int len = rand() % 5;
		for (int j = 0; j < len; ++j)
		{
			char ch = 'A' + rand() % 26;
			words[i] += ch;
		}
	}

	cout << "Randodm Book Generated\n";

	unordered_map<string, int> table;
	auto start = system_clock::now();
	for (auto& s : words)
		table[s]++;
	auto end = system_clock::now();
	auto duration = end - start;
	cout << "single thread [BCD] = " << table["BCD"];
	cout << " Exec time = " << duration_cast<milliseconds>(duration).count() << endl;


	tbb::concurrent_unordered_map<string, atomic_int> tbb_table;
	start = system_clock::now();
	tbb::parallel_for(0, NUM_TEST, 1, [&tbb_table](int i) {
		tbb_table[words[i]]++;
		});
	end = system_clock::now();
	duration = end - start;
	cout << "thread [BCD] = " << tbb_table["BCD"];
	cout << " Exec time = " << duration_cast<milliseconds>(duration).count() << endl;
}