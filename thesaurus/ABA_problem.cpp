// ABA_problem.cpp : This file contains the 'main' function. Program execution begins and ends there.
// -------------------------------------
// g++ -std=c++17 ABA_problem.cpp

#include <atomic>
#include <optional>
#include <cassert>

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
};

int main() {

    int i = 1;
    lockfree_stack<int> lfs(i);

    if (lfs.top()) {
        assert(lfs.top().value() == i);
        assert(lfs.size() == 1);
        assert(lfs.pop_front());
        assert(lfs.size() == 0);
    }
}
