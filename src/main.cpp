#include <iostream>
#include <fstream>
#include <thread>
#include "PrimeFactors.h"
#include "ThreadPool.h"


int main() {

	std::ifstream file("../in.txt");

	uint64_t number;

	ThreadPool tpool;

	while (file >> number) {
		tpool.AddTask( [number] () -> std::string {
			return PrimeFactors(number).ToString();
		});
	}
	system("pause");
	return 0;
}