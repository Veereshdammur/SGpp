/*
 * ThreadPool.cpp
 *
 *  Created on: 20.06.2016
 *      Author: david
 */

#include "ThreadPool.hpp"
#include <chrono>

namespace sgpp{
namespace combigrid {

ThreadPool::ThreadPool(size_t numThreads) :
		numThreads(numThreads), threads(), tasks(), poolMutex(), terminateFlag(false), useIdleCallback(false), idleCallback() {
}

ThreadPool::ThreadPool(size_t numThreads, IdleCallback idleCallback) :
		numThreads(numThreads), threads(), tasks(), poolMutex(), terminateFlag(false), useIdleCallback(true), idleCallback(idleCallback) {
}

ThreadPool::~ThreadPool() {
	triggerTermination();
	join();
}

void ThreadPool::addTask(const Task& task) {
	std::lock_guard<std::mutex> guard(poolMutex);
	tasks.push_back(task);
}

void ThreadPool::addTasks(const std::vector<Task>& newTasks) {
	std::lock_guard<std::mutex> guard(poolMutex);
	tasks.insert(tasks.cend(), newTasks.begin(), newTasks.end());
}

void ThreadPool::start() {
	for (size_t i = 0; i < numThreads; ++i) {
		threads.push_back(std::make_shared<std::thread>([=]() {
			while(true) {
				Task nextTask;

				// wait for terminate or next task
				while(true)
				{
					{
						std::lock_guard<std::mutex> guard(this->poolMutex);
						if(this->terminateFlag) {
							return;
						}
						if(!this->tasks.empty()) {
							nextTask = tasks.front();
							tasks.pop_front();
							break;
						}
					}

					// no tasks, so acquire tasks

					if(useIdleCallback) {
						std::unique_lock<std::mutex> idleLock(idleMutex);

						if(this->terminateFlag || !this->tasks.empty()) {
							continue;
						}

						idleCallback(*this);
					} else {
						std::this_thread::sleep_for(std::chrono::milliseconds(5));
					}
				}

				// execute next task
				nextTask();
			}
		}));
	}
}

void ThreadPool::triggerTermination() {
	std::lock_guard<std::mutex> guard(poolMutex);
	terminateFlag = true;
}

void ThreadPool::join() {
	for (auto thread_ptr : threads) {
		thread_ptr->join();
	}

	threads.clear();
}

// static
void ThreadPool::terminateWhenIdle(ThreadPool &tp) {
	tp.triggerTermination();
}

} /* namespace combigrid */
} /* namespace sgpp*/
