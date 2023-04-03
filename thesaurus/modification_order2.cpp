// modification_order2.cpp : This file contains the 'main' function. Program execution begins and ends there.
// -------------------------------------
// g++ -pthread modification_order2.cpp

#include "thread_pool.h"

#include <atomic>
#include <thread>
#include <cassert>
#include <string>
#include <map>
#include <set>


using pair_t = std::pair<int, int>;

enum ID {
    id_write_x = 1,
    id_write_y,
    id_read_x_y,
    id_read_y_x
};

struct SharedMem {
    int odd_{};
    char padding[__hardware_destructive_interference_size__]{};
    int even_{};
};

std::atomic<bool> x, x1, y;
std::atomic<int> z;

pair_t write_x(SharedMem& s) {

    x.store(true, std::memory_order_relaxed);
    s.odd_ = (int)id_write_x;
    std::atomic_thread_fence(std::memory_order_release);
    x1.store(true, std::memory_order_relaxed);
    return std::make_pair((int)id_write_x, s.odd_);
}

pair_t write_y(SharedMem& s) {

//#define REORDER_TUNNELLING
#ifdef REORDER_TUNNELLING
    const int N = 10;
    volatile int a[N]{};
#endif

    s.even_ = (int)id_write_y;
    y.store(true, std::memory_order_release);

#ifdef REORDER_TUNNELLING
    for (int j = 0; j < N; ++j) {
        a[j] = (int)id_write_y + j + 10;
        s.even_ = a[j];
    }
#endif
    return std::make_pair((int)id_write_y, s.even_);
}

pair_t read_x_y(SharedMem& s) {

    while (!x1.load(std::memory_order_relaxed)) {
        std::this_thread::yield();
    };
    std::atomic_thread_fence(std::memory_order_acquire);
    while (!x.load(std::memory_order_relaxed)) {
        std::this_thread::yield();
    };
    int k = s.odd_;
    if (y.load(std::memory_order_acquire)) {
        k = s.even_;
        ++z;
    }
    return std::make_pair((int)id_read_x_y, k);
}

pair_t read_y_x(SharedMem& s) {

    while (!y.load(std::memory_order_acquire)) {
        std::this_thread::yield();
    };
    int k = s.even_;
    if (x.load(std::memory_order_relaxed)) {
        k = s.odd_;
        ++z;
    }
    return std::make_pair((int)id_read_y_x, k);
}


int main() {

    clock_t start, finish, total_time = 0;

    std::map<std::string, int> map_result;

    std::vector<std::function<pair_t(SharedMem&)>> vecCalls;

    vecCalls.push_back(write_x);
    vecCalls.push_back(write_y);
    vecCalls.push_back(read_x_y);
    vecCalls.push_back(read_y_x);

    SharedMem s;

    ThreadPool pool(4);

    std::vector<std::future<pair_t>> results;

    const int M = 100'000;
    const int N = vecCalls.size();

    for (int k = 0; k < M; ++k) {

        start = clock();

        x = false;
        x1 = false;
        y = false;
        z = 0;
        s.odd_ = 0;
        s.even_ = 0;

        int offset = rand() % N;

        for (int j = 0; j < N; ++j) {
            results.emplace_back(
                pool.enqueue(vecCalls[(offset + j) % N], std::ref(s))
            );
        }

        std::set<pair_t> set_result;
        for (auto&& res : results) {
            try {
                set_result.insert(res.get());
            }
            catch (std::runtime_error& ex) {
                cout_dump_msg(ex.what());
            }
        }
#if defined( SC_DRF__ ) // expectations formal declaration
        assert(z.load() != 0);
#endif

        finish = clock();
        total_time += finish - start;

        std::string str_result;
        for (const auto& res : set_result) {
            str_result.append('[' + std::to_string(res.first) + ',' + std::to_string(res.second) + ']');
        }
        str_result.append(':' + std::to_string(z.load()));

        ++map_result[str_result];

        results.clear();
    }

    for (const auto& [key, val] : map_result) {
        std::cout << key << ' ' << 100 * (double)val / M << '%' << '\n';
    }
    std::cout << "States = " << map_result.size() << " Time = " << (double)total_time / CLOCKS_PER_SEC;
}
