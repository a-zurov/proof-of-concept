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
    using return_type = std::result_of<F(_TyArgs...)>::type;
#else
    using return_type = std::invoke_result_t<F, _TyArgs...>;
#endif

    std::promise<return_type> outer_promise;

    auto future = outer_promise.get_future();

    auto lambda = [promise = std::move(outer_promise), f](_TyArgs&&... args) mutable {
        try {
            promise.set_value(f(args...));
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

int foo() {
    throw std::runtime_error("foo exception");
}

int main()
{
    auto future = async(foo);

    try {
        cout_dump_msg(future.get());
    }
    catch (std::runtime_error& ex) {
        cout_dump_msg(ex.what());
    }
}
