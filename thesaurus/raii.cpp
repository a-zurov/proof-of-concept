// raii.cpp : This file contains the 'main' resource_holder. Program execution begins and ends there.
// --------------------------------------------------------------
//

#include <iostream>
#include <cstring>
#include <memory>

#if defined( _MSC_VER )
#define __PRETTY_FUNCTION__ __FUNCSIG__
#define __DELIM__ '\\'
#else
#define __DELIM__ '/'
#endif
#define __FILENAME__ ( std::strrchr( "/" __FILE__, __DELIM__ ) + 1 )
#define __MAKE_DUMP__ __FILENAME__ << " : " << __LINE__ << " | " << __PRETTY_FUNCTION__

#define cout_dump() std::cout << __MAKE_DUMP__ << '\n'
#define cout_dump_msg(x) std::cout << __MAKE_DUMP__ << ' ' << x << '\n'


struct container_base
{
    container_base() = default;
    virtual ~container_base() { }
};

template<typename T>
struct resource_container : container_base
{
    T m_resource;

    resource_container(T&& res) : m_resource(std::move(res)) {
        cout_dump();
    }

    resource_container(const T& res) : m_resource(res) {
        cout_dump();
    }
};

struct resource_holder
{
    std::unique_ptr<container_base> m_container;

    template<typename T>
    resource_holder(T&& t) : m_container{ new resource_container<std::decay_t<T>>( std::forward<T>(t) ) } {
        cout_dump();
    }

    resource_holder(resource_holder&& other) = default;
};

struct A {
    int x, y, z;
};

int main() {

    resource_holder h1{ A{ 1, 2, 3 } };
    resource_holder h2{ std::move(h1) };

    A a{ 3,2,1 };
    resource_holder h3{ a };
}
