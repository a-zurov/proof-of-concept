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
};

int main() {

    int i = 1;
    lockfree_stack<int> lfs(i);

    if (lfs.top()) {
        assert(lfs.top().value() == i);
    }
}
