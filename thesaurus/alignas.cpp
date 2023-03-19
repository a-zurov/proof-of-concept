// alignas.cpp : This file contains the 'main' function. Program execution begins and ends there.
// --------------------------------------------------------------
// g++ -pthread alignas.cpp

#include <iostream>
#include <cstring>
#include <ctime>
#include <cassert>
#include <atomic>
#include <thread>
#include <vector>

//#define __ALIGNAS_64__

static const int g_nThreads = 4;

struct ShardedCounter {

    struct
#ifdef __ALIGNAS_64__
    alignas(64)
#endif
    AtomicCounter {

        std::atomic<size_t> value_{0};

        void Increment() {
            value_.fetch_add(1);
        }

        size_t Get() const {
            return value_.load();
        }
    };

    AtomicCounter counters_[g_nThreads];

    void Increment(int j) {
        counters_[j].Increment();
    }

    size_t Get() const {
        size_t sum = 0;
        for (const auto& counter: counters_) {
            sum += counter.Get();
        }
        return sum;
    }
};

int main() {

    printf("sizeof(ShardedCounter::AtomicCounter) = %zu\n", sizeof(ShardedCounter::AtomicCounter));

#ifdef __ALIGNAS_64__
    assert((sizeof(ShardedCounter::AtomicCounter) == 64u));
#endif

    clock_t start, finish;
    ShardedCounter counter;
    start = clock();
    std::vector<std::thread> vecThreads;
    for (int i = 0; i < g_nThreads; ++i) {
        vecThreads.emplace_back([&counter, i]() {
            for (size_t j = 0; j < 10'000'000; ++j) {
                counter.Increment(i);
            }
        });
    }
    for (auto& t : vecThreads) {
        t.join();
    }
    finish = clock();
    printf("%zu time = %f\n", counter.Get(), (double)(finish - start) / CLOCKS_PER_SEC);
}
