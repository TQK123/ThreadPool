#ifndef TASKANDRESULT_H
#define TASKANDRESULT_H
#include<functional>
#include<memory>
#include<mutex>
#include<atomic>
#include <condition_variable>
class Any
{
public:
	Any() = default;
	~Any() = default;
	Any(const Any&) = delete;
	Any& operator=(const Any&) = delete;
	Any(Any&&) = default;
	Any& operator=(Any&&) = default;
	template<typename T>  
	Any(T data) : base_(std::make_unique<Derive<T>>(data))
	{}
	template<typename T>
	T cast_()
	{
		Derive<T>* pd = dynamic_cast<Derive<T>*>(base_.get());
		if (pd == nullptr)
		{
			throw "type is unmatch!";
		}
		return pd->data_;
	}
private:
	class Base
	{
	public:
		virtual ~Base() = default;
	};
	template<typename T>
	class Derive : public Base
	{
	public:
		Derive(T data) : data_(data)
		{}
		T data_;  
	};

private:
	std::unique_ptr<Base> base_;
};

class Semaphore
{
public:
	Semaphore(int limit = 0)
		:resLimit_(limit)
		,isExit_(false)
	{}
	~Semaphore()
	{
		isExit_ = true;
	}

	void wait()
	{
		if (isExit_) return;
		std::unique_lock<std::mutex> lock(mtx_);
		cond_.wait(lock, [&]()->bool {return resLimit_ > 0; });
		resLimit_--;
	}
	void post()
	{
		if (isExit_) return;
		std::unique_lock<std::mutex> lock(mtx_);
		resLimit_++;
		cond_.notify_all();  
	}
private:
	std::atomic_bool isExit_;
	int resLimit_;
	std::mutex mtx_;
	std::condition_variable cond_;
};

class Task;

class Result
{
public:
	Result(std::shared_ptr<Task> task, bool isValid = true);
	Result(Result&& ) = default;
	Result& operator=(Result&& ) = default;
	~Result() = default;
	void setVal(Any any);
	Any get();
private:
	Any any_; 
	Semaphore sem_;
	std::shared_ptr<Task> task_;
	std::atomic_bool isValid_;
};

class Task
{
public:
	Task();
	~Task() = default;
	void exec();
	void setResult(Result* res);
	virtual Any run() = 0;

private:
	Result* result_; 
};

class MyTask : public Task
{
public:
	using Func = std::function<Any()>;
	MyTask(Func&& fc);
	Any run();
private:
	Func f;
};
#endif
