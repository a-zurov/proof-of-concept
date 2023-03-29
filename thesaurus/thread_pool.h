#pragma once


#include <iostream>
#include <cstring>
#include <functional>
#include <memory>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>

#define Cxx_17__ 201703L

#if defined( _MSC_VER )
#define __CXX_VER__ _MSVC_LANG
#define __PRETTY_FUNCTION__ __FUNCSIG__
#define __DELIM__ '\\'
#else
#define __CXX_VER__ __cplusplus
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

    ThreadPool(const ThreadPool& other) = delete;
    ThreadPool(ThreadPool&& other) noexcept = delete;
    ThreadPool operator=(const ThreadPool& other) = delete;
    ThreadPool& operator=(ThreadPool&& other) noexcept = delete;

    template<class F, class... _TyArgs>
    auto enqueue(F&& f, _TyArgs&&... args) -> std::future<
#if ( Cxx_17__ > __CXX_VER__ )
        typename std::result_of<F(_TyArgs...)>::type
#else
        typename std::invoke_result_t<F, _TyArgs...>
#endif
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
                        this->condition_.wait(lock, [this] {
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
auto ThreadPool::enqueue(F&& f, _TyArgs&&... args)->std::future<
#if ( Cxx_17__ > __CXX_VER__ )
typename std::result_of<F(_TyArgs...)>::type
#else
typename std::invoke_result_t<F, _TyArgs...>
#endif
>
{
#if ( Cxx_17__ > __CXX_VER__ )
    using return_type = typename std::result_of<F(_TyArgs...)>::type;
#else
    using return_type = std::invoke_result_t<F, _TyArgs...>;
#endif

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
        if (th.joinable()) {
            try {
                th.join();
            }
            catch (std::system_error& ex) {
                cout_dump_msg(ex.what());
            }
        }
    }
}
