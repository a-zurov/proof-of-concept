// ABA_problem.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ----------------------------------------------------------
// g++ -std=c++17 -pthread -fsanitize=address ABA_problem.cpp

#include "thread_pool.h"

#include <atomic>
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
        Node<U>* pNext_;

        explicit Node(const U& value, Node<U>* p = nullptr)
            : value_(value)
            , pNext_(p) {}
    };

    std::atomic<Node<T>*> pHead_;

public:
    lockfree_stack() : pHead_(nullptr) {}
    lockfree_stack(const T& value) : pHead_{ new Node<T>{ value, nullptr } } {}

    std::optional<T> top() const noexcept {

        auto pTop = pHead_.load();
        return pTop != nullptr ? std::optional<T>(pTop->value_) : std::nullopt;
    }

    size_t size() const noexcept {

        size_t depth = 0u;
        auto pTop = pHead_.load();
        while (pTop) {
            ++depth;
            pTop = pTop->pNext_;
        }
        return depth;
    }

    std::optional<T> pop_front() noexcept {

        auto pTop = pHead_.load();
        while (pTop && !pHead_.compare_exchange_weak( pTop          // expected node in head
                                                    , pTop->pNext_) // desired new head node
            ) {}

        if (pTop) {
            T value = pTop->value_;
            delete pTop;
            return std::optional<T>(value);
        }
        return std::nullopt;
    }

    void clear() {
         while(pop_front()){}
    }

    void push_front(const T& value) noexcept {

        auto pNewNode = new Node<T>{ value, pHead_.load() };
        while (!pHead_.compare_exchange_weak( pNewNode->pNext_ // expected node in head
                                            , pNewNode)        // desired new head node
            ) {}
    }

    std::optional<T> push_front_test(const T& value) noexcept {

        push_front(value);
        return std::optional<T>(-value);
    }


#define ABA__
#ifdef ABA__
    std::optional<T> pop_front_ABA(std::once_flag& flag) noexcept {

        auto pTop = pHead_.load();
        while (pTop && !pHead_.compare_exchange_weak(pTop, pTop->pNext_)) {
            std::call_once(flag, [] {
                std::this_thread::yield();
                }
            );
        }

        if (pTop) {
            T value = pTop->value_;
            delete pTop;
            return std::optional<T>(value);
        }
        return std::nullopt;
    }
#endif // ABA__
};

int main() {

    int i = -1;
    lockfree_stack<int> lfs(i);

    if (lfs.top()) {
        assert(lfs.top().value() == i);
        assert(lfs.size() == 1);
        assert(lfs.pop_front());
        assert(lfs.size() == 0);
        lfs.push_front(i);
        assert(lfs.size() == 1);
        assert(lfs.pop_front());
        assert(!lfs.top());
    }

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
        lfs.push_front_test(i);

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
