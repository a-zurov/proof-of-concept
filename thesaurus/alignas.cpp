// alignas.cpp : This file contains the 'main' function. Program execution begins and ends there.
// --------------------------------------------------------------
// g++ -pthread alignas.cpp
// https://rigtorp.se/ringbuffer/

#include <iostream>
#include <cstring>
#include <ctime>
#include <cassert>
#include <cstddef>
#include <atomic>
#include <thread>
#include <vector>

#define Cxx_17__ 201703L

#if defined( _MSC_VER )
#define __CXX_VER__ _MSVC_LANG
#if ( Cxx_17__ <= __CXX_VER__ )
#define __hardware_destructive_interference_size__ std::hardware_destructive_interference_size
#endif
#else
#define __CXX_VER__ __cplusplus
#if ( Cxx_17__ <= __CXX_VER__ and Cxx_17__ <= __cpp_lib_hardware_interference_size )
#define __hardware_destructive_interference_size__ std::hardware_destructive_interference_size
#endif
#endif

#ifndef __hardware_destructive_interference_size__
#define __hardware_destructive_interference_size__ 64u
#endif

static const int g_nThreads = 4;

#define __ALIGNAS_AVOID_FALSE_SHARING__ // performance switch

struct ShardedCounter {

    struct
#ifdef __ALIGNAS_AVOID_FALSE_SHARING__
        alignas(__hardware_destructive_interference_size__)
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

// ---------------------------------------------
// Single-Producer Single-Consumer Ring Buffer

template <typename T>
struct SPSCRingBuffer {

    struct
#ifdef __ALIGNAS_AVOID_FALSE_SHARING__
    alignas(__hardware_destructive_interference_size__)
#endif
    Slot {
        T value;
    };

    std::vector<Slot> buffer_;

#ifdef __ALIGNAS_AVOID_FALSE_SHARING__
    char padding1_[__hardware_destructive_interference_size__]{};
#endif

    std::atomic<size_t> head_{ 0 };
    size_t tail_cached_{ 0 };

#ifdef __ALIGNAS_AVOID_FALSE_SHARING__
    char padding2_[__hardware_destructive_interference_size__]{};
#endif

    std::atomic<size_t> tail_{ 0 };
    size_t head_cached_{ 0 };

    explicit SPSCRingBuffer(const size_t capacity) : buffer_(capacity + 1) {}

    bool TryProduce(T value) {

        const size_t curr_tail = tail_.load();

        if (Next(curr_tail) == head_cached_) {
            head_cached_ = head_.load();
        }

        const size_t curr_head = head_cached_;

        if (IsFull(curr_head, curr_tail)) {
            return false;
        }

        buffer_[curr_tail].value = std::move(value);
        tail_.store(Next(curr_tail));
        return true;
    }

    bool TryConsume(T& value) {

        const size_t curr_head = head_.load();

        if (curr_head == tail_cached_) {
          tail_cached_ = tail_.load();
        }

        const size_t curr_tail = tail_cached_;

        if (IsEmpty(curr_head, curr_tail)) {
            return false;
        }

        value = std::move(buffer_[curr_head].value);
        head_.store(Next(curr_head));
        return true;
    }

private:
    bool IsFull(size_t head, size_t tail) const {
        return Next(tail) == head;
    }

    bool IsEmpty(size_t head, size_t tail) const {
        return tail == head;
    }

    size_t Next(size_t slot) const {
        return (slot + 1) % buffer_.size();
    }
};

class Digest {
public:
    void Feed(int value) {
        digest_ = std::hash<int>()(value) ^ (digest_ << 1);
    }

    size_t Value() const {
        return digest_;
    }

private:
    size_t digest_ = 0;
};


int main() {

    static const int nTries = 3;
    static const int nValues = 10'000'000;

    printf("sizeof(ShardedCounter::AtomicCounter) = %zu\n", sizeof(ShardedCounter::AtomicCounter));

#ifdef __ALIGNAS_AVOID_FALSE_SHARING__
    assert((sizeof(ShardedCounter::AtomicCounter) == 64u));
#endif

    clock_t start, finish;
    ShardedCounter counter;

    for (int j = 0; j < nTries; ++j) {

        start = clock();
        std::vector<std::thread> vecThreads;
        for (int i = 0; i < g_nThreads; ++i) {
            vecThreads.emplace_back([&counter, i]() {
                for (size_t j = 0; j < nValues; ++j) {
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

    // ---------------------------------------------
    // Single-Producer Single-Consumer Ring Buffer

    printf("sizeof(SPSCRingBuffer<int>) = %zu\n", sizeof(SPSCRingBuffer<int>));
    printf("alignof(SPSCRingBuffer<int>) = %zu\n", alignof(SPSCRingBuffer<int>));
#ifdef __ALIGNAS_AVOID_FALSE_SHARING__
    printf("offsetof(SPSCRingBuffer<int>, padding1_) = %zu\n", offsetof(SPSCRingBuffer<int>, padding1_));
    printf("sizeof(SPSCRingBuffer<int>::padding1_) = %zu\n", sizeof(SPSCRingBuffer<int>::padding1_));
#endif
    printf("offsetof(SPSCRingBuffer<int>, head_) = %zu\n", offsetof(SPSCRingBuffer<int>, head_));
#ifdef __ALIGNAS_AVOID_FALSE_SHARING__
    printf("offsetof(SPSCRingBuffer<int>, padding2_) = %zu\n", offsetof(SPSCRingBuffer<int>, padding2_));
    printf("sizeof(SPSCRingBuffer<int>::padding2_) = %zu\n", sizeof(SPSCRingBuffer<int>::padding2_));
#endif
    printf("offsetof(SPSCRingBuffer<int>, tail_) = %zu\n", offsetof(SPSCRingBuffer<int>, tail_));

    SPSCRingBuffer<int> buffer(1024);

    for (int j = 0; j < nTries; ++j) {

        start = clock();
        std::thread producer([&buffer]() {
            for (int i = 0; i < nValues; ++i) {
                while (!buffer.TryProduce(i)) {
                    std::this_thread::yield();
                }
            }
            });
        Digest digest;
        std::thread consumer([&]() {
            for (int i = 0; i < nValues; ++i) {
                int value;
                while (!buffer.TryConsume(value)) {
                    std::this_thread::yield();
                }
                digest.Feed(value);
            }
            });
        producer.join();
        consumer.join();
        finish = clock();
        printf("%zu time = %f\n", digest.Value(), (double)(finish - start) / CLOCKS_PER_SEC);
    }
}
