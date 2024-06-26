#include"TaskandResult.h"
Task::Task()
	: result_(nullptr)
{}

void Task::exec()
{
	if (result_ != nullptr)
	{
		result_->setVal(run()); 
	}
}

void Task::setResult(Result* res)
{
	result_ = res;
}

Result::Result(std::shared_ptr<Task> task, bool isValid)
	: isValid_(isValid)
	, task_(task)
{
	task_->setResult(this);
}

Any Result::get()
{
	if (!isValid_)
	{
		return "";
	}
	sem_.wait(); 
	return std::move(any_);
}

void Result::setVal(Any any) 
{
	this->any_ = std::move(any);
	sem_.post();
}
MyTask::MyTask(Func&& fc) :f(std::move(fc)){}
Any MyTask::run()
{
	return f();
}

