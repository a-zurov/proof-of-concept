// packaged_task.cpp : This file contains the 'main' function. Program execution begins and ends there.
// -----------------------------------------------------------------------
// g++ -std=c++17 -pthread packaged_task.cpp

#include <iostream>
#include <thread>
#include <future>
#include <functional>

//#define __SYNC__

int sum(int x, int y) { return x + y; }

std::future<int> foo()
{
    std::packaged_task<int()> task(std::bind(sum, 2, 11));
    std::future<int> res = task.get_future();

#ifdef __SYNC__
    task();
#else
    std::thread th(std::move(task));
    th.detach();
#endif // __SYNC__

    return res;
}

template<class F, class... _TyArgs>
auto bar(F&& f, _TyArgs&&... args) -> std::future<
    typename std::invoke_result_t<F, _TyArgs...>
>
{
    std::packaged_task<std::invoke_result_t<F, _TyArgs...>()> task(
        std::bind(std::forward<F>(f), std::forward<_TyArgs>(args)...)
    );
    std::future< std::invoke_result_t<F, _TyArgs...> > res = task.get_future();

#ifdef __SYNC__
    task();
#else
    std::thread th(std::move(task));
    th.detach();
#endif // __SYNC__

    return res;
}

int main()
{
    auto x = foo();
    auto y = bar(sum, 2, 11);
    std::cout << x.get() << ' ' << y.get() << '\n';
}
