// ABA_problem_solution.cpp : This file contains the 'main' function. Program execution begins and ends there.
// -------------------------------------
// MSVC only

#include "thread_pool.h"

#include <atomic>
#include <memory.h>
#include <optional>
#include <cassert>
#include <sstream>
#include <map>

template <typename T>
class lockfree_stack
{
    template <typename U>
    struct Node
    {
        U value_;
        std::shared_ptr <Node<U>> spNext_;
    };

#if ( Cxx_20__ <= __CXX_VER__ )
    std::atomic<std::shared_ptr<Node<T>>> spHead_;
#else
    std::shared_ptr<Node<T>> spHead_;
#endif

public:
    lockfree_stack() : spHead_(std::make_shared<Node<T>>()) {}

    std::optional<T> top() const noexcept {
#if (  Cxx_20__ <= __CXX_VER__ )
        auto spTop = spHead_.load();
#else
        auto spTop = std::atomic_load(&spHead_);
#endif
        return spTop != nullptr ? std::optional<T>(spTop->value_) : std::nullopt;
    }

    size_t size() const noexcept {

        size_t depth = 0u;
#if (  Cxx_20__ <= __CXX_VER__ )
        auto spTop = spHead_.load();
#else
        auto spTop = std::atomic_load(&spHead_);
#endif
        while (spTop) {
            ++depth;
            spTop = spTop->spNext_;
        }
        return depth;
    }

    std::optional<T> pop_front() noexcept {

#if (  Cxx_20__ <= __CXX_VER__ )
        auto spTop = spHead_.load();
        while (spTop && !spHead_.compare_exchange_weak( spTop           // expected node in head
                                                      , spTop->spNext_) // desired new head node
              ) {}
#else
        auto spTop = std::atomic_load(&spHead_);
        while ( spTop && !std::atomic_compare_exchange_weak( &spHead_
                                                           , &spTop          // expected node in head
                                                           , spTop->spNext_) // desired new head node
              ) {}
#endif
        if (spTop) {
            return std::optional<T>(spTop->value_);
        }
        return std::nullopt;
    }

    void clear() {
         while(pop_front()){}
    }

    void push_front(const T& value) noexcept {

#if (  Cxx_20__ <= __CXX_VER__ )
        auto spNewNode = std::make_shared<Node<T>>(Node<int>{value, spHead_.load()});
        while (!spHead_.compare_exchange_weak( spNewNode->spNext_ // expected node in head
                                             , spNewNode )        // desired new head node
              ) {}
#else
        auto spNewNode = std::make_shared<Node<T>>(Node<int>{value, std::atomic_load(&spHead_)});
        while (!std::atomic_compare_exchange_weak( &spHead_
                                                 , &spNewNode->spNext_ // expected node in head
                                                 , spNewNode )         // desired new head node
              ) {}
#endif
    }

    std::optional<T> push_front_test(const T& value) noexcept {

        push_front(value);
        return std::optional<T>(-value);
    }

#define ABA__
#ifdef ABA__
    std::optional<T> pop_front_ABA(std::once_flag& flag) noexcept {

#if (  Cxx_20__ <= __CXX_VER__ )
        auto spTop = spHead_.load();
        while (spTop && !spHead_.compare_exchange_weak( spTop           // expected node in head
                                                      , spTop->spNext_) // desired new head node
            ) {
            std::call_once(flag, [] {
                    std::this_thread::yield();
                }
            );
        }
#else
        auto spTop = std::atomic_load(&spHead_);
        while ( spTop && !std::atomic_compare_exchange_weak( &spHead_
                                                           , &spTop          // expected node in head
                                                           , spTop->spNext_) // desired new head node
            ) {
            std::call_once(flag, [] {
                    std::this_thread::yield();
                }
            );
        }
#endif
        if (spTop) {
            return std::optional<T>(spTop->value_);
        }
        return std::nullopt;
    }
#endif // ABA__
};

int main() {

    int i = -1;
    lockfree_stack<int> lfs;

    ThreadPool pool(4);
    std::vector<std::future<std::optional<int>>> results;
    std::map<std::string, int> map_result;

    int L = 100'000;
    int N = 2;

    for (int l = 0; l < L; ++l) {

        lfs.clear();
        assert(!lfs.top());

#ifdef ABA__
        std::once_flag flag;
        lfs.push_front(i);

        results.emplace_back(
            pool.enqueue(&lockfree_stack<int>::pop_front_ABA, std::ref(lfs), std::ref(flag))
        );
        results.emplace_back(
            pool.enqueue(&lockfree_stack<int>::pop_front_ABA, std::ref(lfs), std::ref(flag))
        );
#endif // ABA__

        for (int k = 0; k < N; ++k) {
            results.emplace_back(
                pool.enqueue(&lockfree_stack<int>::push_front_test, std::ref(lfs), i)
            );
        }

        int success_sum = 1;

        for (auto&& res : results) {
            try {
                auto opt = res.get();
                if (opt) success_sum += opt.value();
            }
            catch (std::runtime_error& ex) {
                cout_dump_msg(ex.what());
            }
        }
        results.clear();

        std::stringstream ss;
        ss << '[' << success_sum << ',' << lfs.size() << ']';
        ++map_result[ss.str()];
    }

    for (const auto& [key, val] : map_result) {
        std::cout << key << ' ' << 100 * (double)val / L << '%' << '\n';
    }
    std::cout << "states = " << map_result.size() << " cycles = " << L << '\n';
}
