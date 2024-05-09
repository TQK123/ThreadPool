#include <iostream>
#include <chrono>
#include <thread>
#include "threadpool.h"
using namespace std;


int sum(int start, int end)
{
    std::this_thread::sleep_for(std::chrono::seconds(2));
    int sums = 0;
    for (int i = start; i <=end; ++i)
    {
        sums += i;
    }
    return sums;
}
bool show()
{
    std::this_thread::sleep_for(std::chrono::seconds(2));
    cout << "void 函数的返回!" << endl;
    return true;
}
int main()
{

    ThreadPool pool;
    pool.setMode(PoolMode::MODE_FIXED);
    pool.start(2);
    Result r1=pool.submitTask(std::make_shared<MyTask>(std::bind(sum, 0, 100)));
    Result r2=pool.submitTask(std::make_shared<MyTask>(std::bind(sum, 0, 100)));
    Result r3=pool.submitTask(std::make_shared<MyTask>(std::bind(sum, 0, 100)));
    Result r4=pool.submitTask(std::make_shared<MyTask>(std::bind(show)));

    cout <<"r1:" << r1.get().cast_<int>() << endl;
    cout << "r2:" << r2.get().cast_<int>() << endl;
    cout << "r3:" << r3.get().cast_<int>() << endl;
    cout << "r4:" << r4.get().cast_<bool>() << endl;

    cout << "main end!" << endl;
    return 0;

}