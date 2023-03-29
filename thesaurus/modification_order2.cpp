// modification_order2.cpp : This file contains the 'main' function. Program execution begins and ends there.
// -------------------------------------
// g++ -pthread modification_order2.cpp

#include "thread_pool.h"

#include <atomic>
#include <thread>
#include <cassert>
#include <string>
#include <map>

using pair_t = std::pair<int, int>;

enum ID {
    id_write_x = 1,
    id_write_y,
    id_read_x_y,
    id_read_y_x
};

std::atomic<bool> x, y;
std::atomic<int> z;

pair_t write_x(int& j) {

    j = (int)id_write_x;
    x.store(true);
    return std::make_pair((int)id_write_x, j);
}

pair_t write_y(int& j) {

    j = (int)id_write_y;
    y.store(true);
    return std::make_pair((int)id_write_y, j);
}

pair_t read_x_y(int& j) {

    while (!x.load()) {
        std::this_thread::yield();
    };
    int k = j;
    if (y.load()) {
        k = (k == j) ? j*10 : j;
        ++z;
    }
    return std::make_pair((int)id_read_x_y, k);
}

pair_t read_y_x(int& j) {

    while (!y.load()) {
        std::this_thread::yield();
    };
    int k = j;
    if (x.load()) {
        k = (k == j) ? j*10 : j;
        ++z;
    }
    return std::make_pair((int)id_read_y_x, k);
}

int main() {

    std::map<std::string, int> map_result;

    std::vector<std::function<pair_t(int&)>> vecCalls;

    vecCalls.push_back(write_x);
    vecCalls.push_back(write_y);
    vecCalls.push_back(read_x_y);
    vecCalls.push_back(read_y_x);

    const int N = vecCalls.size();
    const int M = 1'000'000;

    ThreadPool pool(4);

    std::vector<std::future<pair_t>> results;

    for (int k = 0; k < M; ++k) {

        x = false;
        y = false;
        z = 0;
        int offset = rand() % N;

        int shared = 0;

        for (int j = 0; j < N; ++j) {
            results.emplace_back(
                pool.enqueue(vecCalls[(offset + j) % 4], std::ref(shared))
            );
        }

        std::string str_result;
        for (auto&& result : results) {
            try {
                pair_t res = result.get();
                str_result.append('['+ std::to_string(res.first) + ',' + std::to_string(res.second)+']');
            }
            catch (std::runtime_error& ex) {
                cout_dump_msg(ex.what());
            }
        }
        assert(z.load() != 0);

        str_result.append(':' + std::to_string(z.load()));

        ++map_result[str_result];

        results.clear();
    }

    for (const auto& [key, value] : map_result) {
        std::cout << key << ' ' << value << '\n';
    }
    std::cout << "Total = " << map_result.size();
}
