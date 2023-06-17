// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <map>
#include <limits>

#include <vector>
#include <sstream>

template< typename Key, typename Val>
class interval_map {

    using pair_t = std::pair<Val, Val>;
    using map_t = std::map<Key, pair_t >;

    map_t map_;

    const Val min_{ std::numeric_limits<Val>::lowest() };

public:

    const Val& operator[] (const Key& key) {

        auto it = map_.upper_bound(key);
        if (map_.begin() == it || map_.end() == it ) {
            return min_;
        }
        return  it->second.first;
    }

    void assign(const Key& keyBegin, const Key& keyEnd, const Val& value) {

        if (keyEnd <= keyBegin) {
            return;
        }

        if (map_.empty()) {
            map_.insert(std::pair<Key, pair_t>(keyBegin, pair_t(min_, value)));
            map_.insert(std::pair<Key, pair_t>(keyEnd, std::make_pair(value, min_)));
        }

        auto itStart = map_.begin();
        Key keyStart = itStart->first;
        auto itFinish = map_.end();
        Key keyFinish = (--itFinish)->first;

        if (keyEnd < keyStart || keyFinish < keyBegin) {
            map_.insert(std::pair<Key, pair_t>(keyBegin, pair_t(min_, value)));
            map_.insert(std::pair<Key, pair_t>(keyEnd, pair_t(value, min_)));
            return;
        }

        if (keyEnd == keyStart) {
            Val old_val = itStart->second.second;
            map_.insert(std::pair<Key, pair_t>(keyBegin, pair_t(min_, value)));
            map_.insert_or_assign(keyEnd, pair_t(value, old_val));
            return;
        }

        if (keyFinish == keyBegin) {
            Val old_val = itFinish->second.second;
            map_.insert(std::pair<Key, pair_t>(keyBegin, pair_t(old_val, value)));
            map_.insert(std::pair<Key, pair_t>(keyEnd, pair_t(value, min_)));
            return;
        }

        if (keyBegin <= keyStart && keyFinish <= keyEnd) {
            map_.clear();
            map_.insert_or_assign(keyBegin, pair_t(min_, value));
            map_.insert_or_assign(keyEnd, pair_t(value, min_));
        }

        if (keyBegin <= keyStart && keyEnd < keyFinish ) {
            auto itEnd = map_.upper_bound(keyEnd);
            Val old_val = itEnd->second.first;
            map_.erase(itStart, itEnd);
            map_.insert(std::pair<Key, pair_t>(keyBegin, pair_t(min_, value)));
            map_.insert_or_assign(keyEnd, pair_t(value, old_val));
        }

        if (keyStart < keyBegin && keyFinish <= keyEnd) {
            auto itBegin = map_.lower_bound(keyBegin);
            Val old_val = itBegin->second.first;
            map_.erase(itBegin, itFinish); // ??
            map_.insert_or_assign(keyBegin, pair_t(old_val, value));
            map_.insert(std::pair<Key, pair_t>(keyEnd, pair_t(value, min_)));
        }

        if (keyStart <= keyBegin && keyEnd < keyFinish) {
            auto itBegin = map_.lower_bound(keyBegin);
            auto itEnd = map_.upper_bound(keyEnd);
            Val old_val_left = itBegin->second.first;
            Val old_val_right = itEnd->second.first;
            map_.erase(itBegin, itEnd);
            map_.insert_or_assign(keyBegin, pair_t(old_val_left, value));
            map_.insert_or_assign(keyEnd, pair_t(value, old_val_right));
        }

        if (keyStart <= keyBegin && keyFinish <= keyEnd) {
            auto itBegin = map_.lower_bound(keyBegin);
            Val old_val_left = itBegin->second.first;
            map_.erase(itBegin, map_.end());
            map_.insert_or_assign(keyBegin, pair_t(old_val_left, value));
            map_.insert_or_assign(keyEnd, pair_t(value, min_));
        }
    }

    void print() {
        for (auto p : map_) {
            std::cout << p.first << ":(" << p.second.first << ',' << p.second.second << ") ";
        }
        std::cout << '\n';
    }
};

class TestRunner {

    int m_fail_count = 0;

public:
    ~TestRunner() {

        if (m_fail_count > 0) {
            std::cerr << m_fail_count << " unit tests failed. Terminate" << std::endl;
            exit(1);
        }
    }

    template <class TestFunc>
    void RunTest(TestFunc func, const std::string& test_name) {

        try {
            func();
            std::cerr << test_name << " OK\n";
        }
        catch (std::runtime_error& ex) {
            ++m_fail_count;
            std::cerr << test_name << " FAIL:\n" << ex.what() << '\n';
        }
    }
};

#define RUN_TEST(tr, func) \
    tr.RunTest(func, #func)

template< typename Key, typename T>
void AssignTogether(std::vector<T>& v1, interval_map<Key, T>& map, Key keyStart, Key keyEnd, T val) {

    map.assign(keyStart, keyEnd, val);

    std::fill(v1.begin() + keyStart, v1.begin() + keyEnd, val);

    /*
    int j = 0;
    for (auto& item : v1) {
        std::cout << j++ << ':' << item << ' ';
    }
    std::cout << '\n';
    */
}

template<class T>
void AssertEqual2(std::ostringstream& os, const T& t, const T& u) {

    if (t != u) {
        os << t << " != " << u;
        throw std::runtime_error(os.str());
    }
    os << t;
}


template< typename Key, typename T>
void Compare(const std::vector<T>& v, interval_map<Key, T>& map, Key keyStart) {

    std::ostringstream os;
    for (unsigned i = 0; i < v.size(); ++i) {
        os << i << ':';
        AssertEqual2(os, v.at(i), map[keyStart]);
        ++keyStart;
        os << ' ';
    }
}

void TestComparison2() {

    int N = 40;
    interval_map<int, int> imap;
    std::vector<int> v1(N, std::numeric_limits<int>::lowest());

    int keyBegin, keyEnd;

    for (int j = 0; j < 10000; ++j) {

        keyBegin = rand() % N;
        keyEnd = rand() % N;

        if (keyEnd <= keyBegin) {
            continue;
        }

        std::cout << '[' << keyBegin << ',' << keyEnd << ") = " << j << '\n';

        AssignTogether(v1, imap, keyBegin, keyEnd, j);

        imap.print();
    }

    Compare(v1, imap, 0);



}
int main()
{

    interval_map<int, int> imap;
    imap.assign(3, 7, 10);

    std::cout << "imap.assign(3, 7, 10)\n";
    for (int j = 0; j < 10; ++j) {
        std::cout << j << ':' << imap[j] << ' ';
    }
    std::cout << '\n';

    imap.assign(1, 3, 30);
    std::cout << "imap.assign(1, 3, 30)\n";
    for (int j = 0; j < 10; ++j) {
        std::cout << j << ':' << imap[j] << ' ';
    }
    std::cout << '\n';

    imap.assign(7, 9, 40);
    std::cout << "imap.assign(7, 9, 40)\n";
    for (int j = 0; j < 10; ++j) {
        std::cout << j << ':' << imap[j] << ' ';
    }
    std::cout << '\n';

    imap.assign(1, 5, 50);
    std::cout << "imap.assign(1, 5, 50)\n";
    for (int j = 0; j < 10; ++j) {
        std::cout << j << ':' << imap[j] << ' ';
    }
    std::cout << '\n';

    imap.assign(3, 7, 60);
    std::cout << "imap.assign(3, 7, 60)\n";
    for (int j = 0; j < 10; ++j) {
        std::cout << j << ':' << imap[j] << ' ';
    }
    std::cout << '\n';

    TestRunner tr;
    RUN_TEST(tr, TestComparison2);
}
