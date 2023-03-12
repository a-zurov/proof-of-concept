// thread_pool.cpp : This file contains the 'main' function. Program execution begins and ends there.
// --------------------------------------------------------------
// g++ -pthread thread_pool.cpp

#include <iostream>
#include <cstring>
#include <functional>
#include <memory>
#include <thread>
#include <vector>
#include <queue>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <future>

#if defined( _MSC_VER )
#define __PRETTY_FUNCTION__ __FUNCSIG__
#define __DELIM__ '\\'
#else
#define __DELIM__ '/'
#endif
#define __FILENAME__ ( std::strrchr( "/" __FILE__, __DELIM__ ) + 1 )
#define __MAKE_DUMP__ __FILENAME__ << " : " << __LINE__ << " | " << __PRETTY_FUNCTION__

#define cout_dump_msg(x) std::cout << __MAKE_DUMP__ << ' ' << x << '\n'

std::mutex g_mutex;

#define cout_dump_msg_lock(x) { std::lock_guard<std::mutex> lock(g_mutex); \
                                cout_dump_msg(x); }

#define ALLOW_DUMP
#ifdef ALLOW_DUMP
#define cout_dump() std::cout << __MAKE_DUMP__ << '\n'
#else
#define cout_dump()
#endif

struct ThreadPool final {

    ~ThreadPool();
    ThreadPool(size_t);
    template<class F, class... _TyArgs>
    auto enqueue(F&& f, _TyArgs&&... args) -> std::future<
        typename std::result_of<F(_TyArgs...)>::type
    >;
private:
    // the task's queue
    std::queue<std::function<void()>> queTasks_;
    // need to keep threads tracking because we should join them
    std::vector<std::thread> vecThreads_;
    // synchronization
    std::mutex mutex_;
    std::condition_variable condition_;
    bool stop_;
};

ThreadPool::ThreadPool(size_t nThreads) : stop_(false)
{
    for (size_t i = 0; i < nThreads; ++i) {
        vecThreads_.emplace_back(
            [this]() {
                for (;;) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->mutex_);
                        this->condition_.wait( lock, [this] {
                            return this->stop_ || !this->queTasks_.empty();
                            }
                        );
                        if (this->stop_ && this->queTasks_.empty())
                            return;
                        task = std::move(this->queTasks_.front());
                        this->queTasks_.pop();
                    }
                    task();
                }
            }
        );
    }
}

// add new task to the pool
template<class F, class... _TyArgs>
auto ThreadPool::enqueue(F&& f, _TyArgs&&... args) -> std::future<
    typename std::result_of<F(_TyArgs...)>::type
>
{
    using return_type = typename std::result_of<F(_TyArgs...)>::type;

    auto spTask = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<_TyArgs>(args)...)
        );

    std::future<return_type> res = spTask->get_future();
    {
        std::unique_lock<std::mutex> lock(mutex_);

        // don't allow enqueueing after stopping the pool
        if (stop_) throw std::runtime_error("enqueue on stopped ThreadPool");

        queTasks_.emplace([spTask]() { (*spTask)(); });
    }
    condition_.notify_one();

    return res;
}

// the destructor joins all threads
inline ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(mutex_);
        stop_ = true;
    }
    condition_.notify_all();

    for (std::thread& th : vecThreads_) {
        th.join();
    }
}

int foo(int x, int& y) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return x * y++;
};

int main() {

    ThreadPool pool(4);

    std::vector<std::future<int>> results;

    int N = 8;
    std::vector<int> vec(N);
    for (int i = 0; i < N; ++i) {
        results.emplace_back(
           pool.enqueue(foo, i, std::ref(vec[i] = i))
        );
    }
    int j = 0;
//#define __RACE_CONDITION__
#ifdef __RACE_CONDITION__
    for (auto&& result : results)
        cout_dump_msg(result.get() << ':' << vec[j++]);
#else
    for (auto&& result : results) {
        int res = result.get();
        cout_dump_msg(res << ':' << ++j);
    }
#endif
    results.clear();

    for (int i = 0; i < N; ++i) {
        results.emplace_back(
            pool.enqueue(
                [i]() {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    cout_dump_msg_lock(i << ':' << std::hex << std::this_thread::get_id());
                    return i * i;
                }
            )
        );
    }
    int k = 0;
    for (auto&& result : results) {
//#define __DEADLOCK__
#ifdef __DEADLOCK__
        cout_dump_msg_lock(result.get());
#else
        auto res = result.get();
        cout_dump_msg_lock(std::dec << res << ':' << ++k);
#endif
    }
}
