// static.cpp : This file contains the 'main' function. Program execution begins and ends there.
// -------------------------------------
// g++ -pthread static.cpp

#include "thread_pool.h"

#include <atomic>
#include <thread>
#include <string>
#include <sstream>
#include <map>
#include <set>

struct Resource {};

using pair_t = std::pair<int, Resource*>;

enum ID {
    id_create = 1,
    id_read_even,
    id_read_odd
};

struct SharedMem {
    Resource* even_;
    Resource* odd_;
};

std::atomic<bool> x, y;

pair_t create(SharedMem& s, std::once_flag& flag) {

#define STATIC__
#ifdef STATIC__
    static
#endif
    Resource* pRes = new Resource();

    bool once = false;
    std::call_once(flag,
        [&] {
            s.even_ = pRes;
            y.store(true, std::memory_order_release);
            once = true;
        }
    );
    if (!once) {
        s.odd_ = pRes;
        std::atomic_thread_fence(std::memory_order_release);
        x.store(true, std::memory_order_relaxed);
    }
    return std::make_pair(once ? (int)id_create - 1 : (int)id_create, pRes);
}

pair_t read_even(SharedMem& s, std::once_flag& flag) {

    while (!y.load(std::memory_order_acquire)) {
        std::this_thread::yield();
    };
#ifndef STATIC__
    delete s.even_;
#endif
    return std::make_pair((int)id_read_even, s.even_);
}

pair_t read_odd(SharedMem& s, std::once_flag& flag) {

    while (!x.load(std::memory_order_relaxed)) {
        std::this_thread::yield();
    };
    std::atomic_thread_fence(std::memory_order_acquire);
#ifndef STATIC__
    delete s.odd_;
#endif
    return std::make_pair((int)id_read_odd, s.odd_);
}


int main() {

    clock_t start, finish, total_time = 0;

    std::map<std::string, int> map_result;

    std::vector<std::function<pair_t(SharedMem&, std::once_flag&)>> vecCalls;

    vecCalls.push_back(create);
    vecCalls.push_back(create);
    vecCalls.push_back(read_odd);
    vecCalls.push_back(read_even);

    SharedMem s;

    ThreadPool pool(4);

    std::vector<std::future<pair_t>> results;

    const int L = 1;
    const int M = 10;
    const int N = vecCalls.size();

    for (int l = 0; l < L; ++l) {

        for (int k = 0; k < M; ++k) {

            start = clock();

            x = false;
            y = false;
            s.even_ = nullptr;
            s.odd_ = nullptr;
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

            finish = clock();
            total_time += finish - start;

            std::stringstream ss;
            for (const auto& res : set_result) {
                ss << '[' << res.first << ',' << static_cast<void*>(res.second) << ']';
            }
            ++map_result[ss.str()];

            results.clear();
        }

        for (const auto& [key, val] : map_result) {
            std::cout << key << ' ' << 100 * (double)val / M << '%' << '\n';
        }
        std::cout << "States = " << map_result.size() << " Time = " << (double)total_time / CLOCKS_PER_SEC << '\n';

        map_result.clear();
    }
}
