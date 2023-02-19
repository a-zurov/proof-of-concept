// vtbl.cpp : This file contains the 'main' function. Program execution begins and ends there.
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
class CCounter
{
private:
    static std::size_t count_;

protected:
    CCounter() {
        cout_dump_msg( ++count_ << " this = " << this );
    }

    CCounter(CCounter<CountedType> const& cc) {
        cout_dump_msg( ++count_ << " this = " << this << ' ' << &cc );
    }

    CCounter(CCounter<CountedType>&& cc) noexcept {
        cout_dump_msg( ++count_ << " this = " << this << ' ' << &cc );
    }

    ~CCounter() {
        cout_dump_msg( --count_ << " this = " << this );
    }

public:
    static std::size_t alive() {
        return count_;
    }
};

template<typename CountedType>
std::size_t CCounter<CountedType>::count_ = 0;

template<typename T>
class A : public CCounter<A<T>>
{
    T a_;
};

using B = A<int>;

int main() {

    cout_dump_msg(sizeof(B));

    B a1, a2;
    cout_dump_msg(B::alive());

    {
        std::vector<B> v1{ a1, std::move(a2) };
        cout_dump_msg(B::alive());

        v1.emplace_back(B());
        cout_dump_msg(B::alive());
    }

    cout_dump_msg(B::alive());
}
