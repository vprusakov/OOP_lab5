#include "ThreadPool.h"

ThreadPool::ThreadPool(std::string filename) : output_filename(filename), done(false), exit(false), pause(false) {
	fout.open(filename);
	if (!fout.good()) {
		std::cerr << "Output file can't be opened.\n";
		this->Exit();
	}
	unsigned const thread_count = std::thread::hardware_concurrency();
	for (unsigned i = 0; i < thread_count; ++i) {
		threads.emplace_back(&ThreadPool::Executor, this);
	}
}
ThreadPool::~ThreadPool() {
	done = true;
	work_available.notify_all();
	unsigned const thread_count = threads.size();
	for (unsigned i = 0; i < thread_count; ++i) {
		threads[i].join();
	}
}
void ThreadPool::AddTask(std::function<std::string()> const &f) {
	std::unique_lock<std::mutex> lock(work_q_m);
	work_q.push(f);
	work_available.notify_one();
}
void ThreadPool::Exit() {
	done = true;
	exit = true;
	work_available.notify_all();
}
void ThreadPool::Pause() {
	pause = true;
	fout.close();
}
void ThreadPool::Resume() {
	pause = false;
	OpenOutputFile(output_filename);
	paused.notify_all();
	work_available.notify_all();
}
int ThreadPool::GetRemainTasksCount() {
	return work_q.size();
}
void ThreadPool::Executor() {
	while (true) {
		std::unique_lock<std::mutex> lock(work_q_m);

		work_available.wait(lock, [this] {
			return ((!work_q.empty() || done) && !pause);
		});

		if ((work_q.empty() && done) || exit) return;

		std::function<std::string()> f = std::move(work_q.front());

		work_q.pop();
		lock.unlock();

		std::string res = f();

		lock.lock();

		paused.wait(lock, [this] {
			return (!pause);
		});
		fout << res << std::endl;
	}
}
void ThreadPool::OpenOutputFile(std::string filename) {
	fout.open(filename, std::ios::app);
	if (!fout.good()) {
		std::cerr << "Output file can't be opened.\n";
		this->Exit();
	}
}