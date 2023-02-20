// crtp.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ------------------------------------------

#include <iostream>
#include <cstring>
#include <vector>

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#define __DELIM__ '\\'
#else
#define __DELIM__ '/'
#endif
#define __FILENAME__ ( std::strrchr( "/" __FILE__, __DELIM__ ) + 1 )
#define __MAKE_DUMP__ __FILENAME__ << " : " << __LINE__ << " | " << __PRETTY_FUNCTION__

#define cout_dump() std::cout << __MAKE_DUMP__ << '\n'
#define cout_dump_msg(x) std::cout << __MAKE_DUMP__ << ' ' << x << '\n'

template<typename CountedType>
class BaseCounter
{
private:
    static std::size_t count_;

protected:
    BaseCounter() {
        cout_dump_msg( ++count_ << " this = " << this );
    }

    BaseCounter( BaseCounter<CountedType> const& cc ) {
        cout_dump_msg( ++count_ << " this = " << this << ' ' << &cc );
    }

    BaseCounter( BaseCounter<CountedType>&& cc ) noexcept {
        cout_dump_msg( ++count_ << " this = " << this << ' ' << &cc );
    }

    ~BaseCounter() {
        cout_dump_msg( --count_ << " this = " << this );
    }

public:
    static std::size_t alive() {
        return count_;
    }
};

template<typename CountedType>
std::size_t BaseCounter<CountedType>::count_ = 0;

template<typename T>
class Derived : public BaseCounter<Derived<T>>
{
    T x_;
};

using D = Derived<int>;

int main() {

    cout_dump_msg(sizeof(D)); // EBCO

    D d1, d2;
    cout_dump_msg(D::alive());

    {
        std::vector<D> v1{ d1, std::move(d2) };
        cout_dump_msg(D::alive());

        v1.emplace_back(D());
        cout_dump_msg(D::alive());
    }

    cout_dump_msg(D::alive());
}
