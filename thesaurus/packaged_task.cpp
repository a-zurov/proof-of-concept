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
    auto future_foo = foo();
    auto future_bar = bar(sum, 2, 11);
    std::cout << future_foo.get() << ' ' << future_bar.get() << '\n';

    std::promise<int> promise1;
    auto future1 = promise1.get_future();
    std::thread th1(
        [&promise1](int j, int k) {
            promise1.set_value( sum(j,k) );
        }
        , 2, 11 );

    std::promise<int> promise2;
    auto future2 = promise2.get_future();
    std::thread th2(
        [](std::promise<int>&& promise, int j, int k) {
            promise.set_value(sum(j, k));
        }
    , std::move(promise2), 2, 11);

    std::promise<int> promise3;
    auto future3 = promise3.get_future();
    std::thread th3(
        [](std::promise<int>& promise, int j, int k) {
            promise.set_value(sum(j, k));
        }
    , std::ref(promise3), 2, 11);

    std::promise<int> promise4;
    auto future4 = promise4.get_future();
    std::thread th4(
        [promise = std::move(promise4)](int j, int k) mutable {
            promise.set_value(std::move(sum(j, k)));
        }
    , 2, 11);

    std::cout << future1.get() << ' '
        << future2.get() << ' '
        << future3.get() << ' '
        << future4.get() << ' '
        << '\n';

    th1.join();
    th2.join();
    th3.join();
    th4.join();
}
