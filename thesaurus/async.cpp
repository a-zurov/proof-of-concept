// async.cpp : This file contains the 'main' function. Program execution begins and ends there.
// -----------------------------------------------------------------------
// g++ -pthread async.cpp

#include <iostream>
#include <cstring>
#include <thread>
#include <future>
#include <functional>

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


template<typename F, typename... _TyArgs>
auto async(F&& f, _TyArgs&&... args)
{
#if ( Cxx_17__ > __CXX_VER__ )
    using return_type = typename std::result_of<F(_TyArgs...)>::type;
#else
    using return_type = std::invoke_result_t<F, _TyArgs...>;
#endif

    std::promise<return_type> outer_promise;
    auto future = outer_promise.get_future();
    auto function = (std::decay_t<F>)f;

    auto lambda = [promise = std::move(outer_promise), function](_TyArgs&&... args) mutable {
        try {
            promise.set_value(function(args...));
        }
        catch (...) {
            promise.set_exception(std::current_exception());
        }
    };

    std::thread(std::move(lambda)
        , std::forward<_TyArgs>(args)...
    ).detach();

    return future;
}

template<typename F, typename... _TyArgs>
auto async2(F&& f, _TyArgs&&... args)
{
    std::packaged_task<std::remove_reference_t<F>> outer_task(f);
    auto future = outer_task.get_future();

    auto lambda = [task = std::move(outer_task)](_TyArgs&&... args) mutable {
        task(args...);
    };

    std::thread(
        std::move(lambda),
        std::forward<_TyArgs>(args)...
    ).detach();

    return future;
}

int foo() {
    throw std::runtime_error("foo exception");
}

struct S {
    std::string s_;
    ~S() {
        cout_dump_msg(s_);
    }
    S(const std::string s) : s_(s) {
        cout_dump_msg(s_);
    }
    S(const S& other) : s_(other.s_) {
        cout_dump_msg(s_ << ' ' << other.s_);
    }
    S(S&& other) noexcept : s_(std::move(other.s_)) {
        cout_dump_msg(s_ << ' ' << other.s_);
    }
};

int foo1(S s1, S s2) {
    cout_dump_msg(s1.s_ << ' ' << s2.s_);
    throw std::runtime_error("foo1 exception");
}


int main()
{
    auto future1 = async(foo);
    auto future2 = async2(foo);

    try {
        cout_dump_msg(future1.get());
    }
    catch (std::runtime_error& ex) {
        cout_dump_msg(ex.what());
    }

    try {
        cout_dump_msg(future2.get());
    }
    catch (std::runtime_error& ex) {
        cout_dump_msg(ex.what());
    }

    S s{ "xyz" };
    auto future3 = async(foo1, S{"abc"}, std::move(s));
    auto future4 = async2(foo1, S{"qwe"}, S{"abc"});

    try {
        cout_dump_msg(future3.get());
    }
    catch (std::runtime_error& ex) {
        cout_dump_msg(ex.what());
    }

    try {
        cout_dump_msg(future4.get());
    }
    catch (std::runtime_error& ex) {
        cout_dump_msg(ex.what());
    }
}
