#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <queue>
#include <memory>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <unordered_map>
#include"TaskandResult.h"
#include<thread>


enum class PoolMode
{
	MODE_FIXED, 
	MODE_CACHED, 
};

class Thread
{
public:
	using ThreadFunc = std::function<void(int)>;
	Thread(ThreadFunc func);
	~Thread();

	void start();

	int getId()const;
private:
	ThreadFunc func_;
	static int generateId_;
	int threadId_;  
};

class ThreadPool
{
public:
	ThreadPool();
	~ThreadPool();

	void setMode(PoolMode mode);

	void setTaskQueMaxThreshHold(int threshhold);
	void setThreadSizeThreshHold(int threshhold);

	Result submitTask(std::shared_ptr<Task> sp);

	void start(int initThreadSize = std::thread::hardware_concurrency());

	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;

private:
	void threadFunc(int threadid);

	bool checkRunningState() const;

private:
	std::unordered_map<int, std::unique_ptr<Thread>> threads_;

	int initThreadSize_; 
	int threadSizeThreshHold_; 
	std::atomic_int curThreadSize_;
	std::atomic_int idleThreadSize_;

	std::queue<std::shared_ptr<Task>> taskQue_; 
	std::atomic_int taskSize_; 
	int taskQueMaxThreshHold_; 

	std::mutex taskQueMtx_;
	std::condition_variable notFull_;
	std::condition_variable notEmpty_; 
	std::condition_variable exitCond_; 

	PoolMode poolMode_;
	std::atomic_bool isPoolRunning_;
};

#endif
