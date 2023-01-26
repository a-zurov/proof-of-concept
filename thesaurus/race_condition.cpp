// race_condition.cpp : This file contains the 'main' function. Program execution begins and ends there.
// g++ -fsanitize=address race_condition.cpp
// ------------------------------------------

#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

//#define RACE_CONDITION

class ForwardList {

    std::mutex m_mutex;

    struct Node {
        Node* next;
    };

public:
    void Push() {

        Node* new_node = new Node{};

#ifndef RACE_CONDITION
        std::lock_guard<std::mutex> lock(m_mutex);
#endif
        new_node->next = head_;
        head_ = new_node;
    }

    void Pop() {

#ifndef RACE_CONDITION
        std::lock_guard<std::mutex> lock(m_mutex);
#endif

        Node* head = head_;
        head_ = head->next;
        delete head;
    }

private:
    Node* head_{ nullptr };
};

int main() {
    ForwardList list;

    // Just works

    list.Push();
    list.Push();
    list.Pop();
    list.Pop();
    list.Push();
    list.Pop();

    // Concurrent access

    std::vector<std::thread> threads;

    for (size_t i = 0; i < 5; ++i)
    {
        threads.emplace_back(
            [&]() {
                for (size_t k = 0; k < 100500; ++k) {
                    list.Push();
                    list.Pop();
                }
            }
        );
    }

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
