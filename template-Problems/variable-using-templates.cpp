//----------------------------------------------------------------------------
// Variable and 'using' templates
//----------------------------------------------------------------------------
// https://en.cppreference.com/w/cpp/language/variable_template
//----------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <map>

// since C++11

template <typename T>
using MyMap_t = std::map<T, T>;

// since C++14

template <typename T>
class S {
    static const size_t m_nMaxSize;
public:
    size_t getMaxSize() const {
        return m_nMaxSize;
    }
};

template<typename T>
const size_t typename S<T>::m_nMaxSize = 200;

template<>
const size_t typename S<int>::m_nMaxSize = 100;

template <typename T>
class Foo : public S<T> {
    T* m_pT;
    size_t m_nSize;
};

int main()
{
    MyMap_t<std::string> map{ {"abc", "zyx"}, {"cde", "wut"} };

    std::cout << map["abc"] << " " << map["cde"] << std::endl;

    Foo <int> fooInt{};
    Foo <double> fooDouble{};

    std::cout << fooInt.getMaxSize() << " " << fooDouble.getMaxSize() << std::endl;
}
