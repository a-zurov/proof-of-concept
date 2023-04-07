// data_race_dcl.cpp : This file contains the 'main' function. Program execution begins and ends there.
// -------------------------------------
// g++ -pthread data_race_dcl.cpp

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
    int shared_;
    int even_;
    int odd_;
};

std::atomic<bool> x, y;
std::atomic<int> z;
std::mutex test_mutex;

pair_t write_x(SharedMem& s, std::once_flag& flag) {

    s.odd_ = (int)id_write_x + 10;

//#define __DCL__
#ifdef __DCL__
    if (!s.shared_) {
        std::lock_guard lock(test_mutex);
        if (!s.shared_) {
            s.shared_ = (int)id_write_x;
            s.odd_ = s.shared_;
        }
    }
#else
    std::call_once(flag,
        [&s] {
            s.shared_ = (int)id_write_x;
            s.odd_ = s.shared_;
        }
    );
#endif

    std::atomic_thread_fence(std::memory_order_release);
    x.store(true, std::memory_order_relaxed);
    return std::make_pair((int)id_write_x, s.odd_);
}

pair_t write_y(SharedMem& s, std::once_flag& flag) {

    s.even_ = (int)id_write_y + 10;

#ifdef __DCL__
    if (!s.shared_) {
        std::lock_guard lock(test_mutex);
        if (!s.shared_) {
            s.shared_ = (int)id_write_y;
            s.even_ = s.shared_;
        }
    }
#else
    std::call_once(flag,
        [&s] {
            s.shared_ = (int)id_write_y;
            s.even_ = s.shared_;
        }
    );
#endif

    y.store(true, std::memory_order_release);
    return std::make_pair((int)id_write_y, s.even_);
}

pair_t read_x_y(SharedMem& s, std::once_flag&) {

    while (!x.load(std::memory_order_relaxed)) {
        std::this_thread::yield();
    };
    std::atomic_thread_fence(std::memory_order_acquire);
    int k = s.even_;
    return std::make_pair((int)id_read_x_y, k);
}

pair_t read_y_x(SharedMem& s, std::once_flag&) {

    while (!y.load(std::memory_order_acquire)) {
        std::this_thread::yield();
    };
    int j = s.odd_;
    return std::make_pair((int)id_read_y_x, j);
}


int main() {

    clock_t start, finish, total_time = 0;

    std::map<std::string, int> map_result;

    std::vector<std::function<pair_t(SharedMem&, std::once_flag&)>> vecCalls;

    vecCalls.push_back(write_x);
    vecCalls.push_back(write_y);
    vecCalls.push_back(read_x_y);
    vecCalls.push_back(read_y_x);

    SharedMem s;

    ThreadPool pool(4);

    std::vector<std::future<pair_t>> results;

    const int M = 1'000'000;
    const int N = vecCalls.size();

    for (int k = 0; k < M; ++k) {

        start = clock();

        x = false;
        y = false;
        z = 0;
        s.shared_ = 0;
        s.even_ = 0;
        s.odd_ = 0;
        std::once_flag flag;

        int offset = rand() % N;

        for (int j = 0; j < N; ++j) {
            results.emplace_back(
                pool.enqueue(vecCalls[(offset + j) % N], std::ref(s), std::ref(flag))
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
