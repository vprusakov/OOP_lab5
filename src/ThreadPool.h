#pragma once
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <fstream>
#include <atomic>
#include <condition_variable>
#include <chrono>
#include <iostream>
#include <string>

class ThreadPool {
public:
	ThreadPool(std::string);
	~ThreadPool();
	void AddTask(std::function<std::string()> const &f);
	void Exit();
	void Pause();
	void Resume();
	int GetRemainTasksCount();
private:
	void Executor();
	void OpenOutputFile(std::string filename);

	std::mutex work_q_m;
	std::condition_variable work_available;
	std::condition_variable paused;
	std::vector<std::thread> threads;
	std::queue<std::function<std::string()> > work_q;
	
	bool done, exit, pause;
	std::string output_filename;
	std::ofstream fout;
};