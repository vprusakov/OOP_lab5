#include <iostream>
#include <fstream>
#include <thread>
#include "PrimeFactors.h"
#include "ThreadPool.h"

void ControlWork(ThreadPool &tpool) {
	std::string command;
	while (tpool.GetRemainTasksCount() != 0) {
		std::cin >> command;
		if (command == "exit") {
			tpool.Exit();
		}
		else if (command == "pause") {
			tpool.Pause();
		}
		else if (command == "resume") {
			tpool.Resume();
		}
	}
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		std::cerr << "Wrong Parameters.\n";
	}

	std::ifstream fin(argv[1]);
	if (!fin.is_open()) {
		std::cerr << "Input file can't be read.\n";
	}

	uint64_t number;
	ThreadPool tpool(argv[2]);

	while (fin >> number) {
		tpool.AddTask( [number] () -> std::string {
			return PrimeFactors(number).ToString();
		});
	}

	std::thread listening(ControlWork, std::ref(tpool));
	listening.detach();

	return 0;
}