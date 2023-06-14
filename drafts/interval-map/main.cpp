// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <ostream>
#include <type_traits>
#include <map> // ??
#include <optional>
#include <memory>

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

template< typename Key
    , typename V
    , typename Compare = std::less<Key>
    >
class interval_map {

    using map_t = std::map<Key, std::optional<V>, Compare >;

    map_t map_;

public:

    std::optional<V> operator[] (const Key& key) {

        auto it = map_.upper_bound(key);
        if (map_.begin() == it) {
            return std::nullopt;
        }
        return (--it)->second;
    }

    void assign(const Key& keyBegin, const Key& keyEnd, const V& val) {

        if (keyEnd <= keyBegin) {
            return;
        }
        auto itEnd = map_.upper_bound(keyEnd);
        if (map_.begin() == itEnd) {
            map_.insert( std::pair(keyBegin, std::optional<V>(val)) );
            map_.insert( std::pair(keyEnd, std::nullopt) );
        }
    }
};


int main()
{
    /*
    CKey<int> key1;
    CKey key2{ 2 };
    std::cout << key1 << ' ' << key2;
    */

    interval_map<int, int> imap;
    imap.assign(2, 4, 10);

    for (int j = 0; j < 5; ++j) {
        auto opt = imap[j];
        std::cout << j << ':' << (opt.has_value() ? opt.value() : -1) << ' ';
    }
}
