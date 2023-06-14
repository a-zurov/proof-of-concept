// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <ostream>
#include <type_traits>
#include <map> // ??

// For tests only:
template< typename T
    , typename = std::enable_if_t< std::is_scalar<T>::value >
>
class CKey {

    T arg_{ 0 };

public:
    CKey()
    {}

    CKey(T arg)
        : arg_(arg)
    {}

    CKey(const CKey& key)
        : arg_(key.arg_)
    {}

    CKey& operator= (const CKey& rhs) {

        if (this == &rhs) {
            return *this;
        }

        arg_ = rhs.arg_;
        return *this;
    }

    // Move assignment ??

    friend std::ostream& operator<< (std::ostream& os, const CKey<T>& key) {

        return os << key.arg_;
    }
};


// Draft of interval_map datastructure:

template<typename K, typename V>
class interval_map {

    std::map<K, V> map_; // ??

public:

    // Assign value val to interval [keyBegin, keyEnd).
    // Conforming to the C++ Standard Library conventions, the interval
    // includes keyBegin, but excludes keyEnd.
    // std::map has insert_or_assign method, which
    // overwrite previous values in the interval.
    void assign(const K& keyBegin, const K& keyEnd, const V& val) {

    }
};


int main()
{
    CKey<int> key1;
    CKey key2{ 2 };
    std::cout << key1 << ' ' << key2;
}
