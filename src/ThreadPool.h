#pragma once
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <fstream>
#include <atomic>
#include <condition_variable>
#include <chrono>

class ThreadPool {
public:
	ThreadPool() : done(false) {
		unsigned const thread_count = std::thread::hardware_concurrency();
		for (unsigned i = 0; i < thread_count; ++i) {
			threads.emplace_back(&ThreadPool::Executor, this);
		}
	}
	~ThreadPool() {
		done = true;
		work_available.notify_all();
		unsigned const thread_count = threads.size();
		for (unsigned i = 0; i < thread_count; ++i) {
			threads[i].join();
		}
	}
	void AddTask(std::function<std::string()> const &f) {
		std::unique_lock<std::mutex> lock(work_q_m);
		work_q.push(f);
		//notified = true;
		work_available.notify_one();
	}
private:
	std::mutex work_q_m;
	std::condition_variable work_available;

	std::atomic_bool done;
	//bool notified = false;

	std::vector<std::thread> threads;
	std::queue<std::function<std::string()> > work_q;

	void Executor() {
		while (true) {
			std::unique_lock<std::mutex> lock(work_q_m);

			work_available.wait(lock, [this] {
				return (!work_q.empty() || done);
			});

			if (done && work_q.empty()) return;
			

			std::function<std::string()> f = std::move(work_q.front());

			work_q.pop();
			lock.unlock();
				
			std::string res = f();
			lock.lock();
			std::cout << res << std::endl;
		}
	}
};