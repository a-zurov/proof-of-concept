// race_condition.cpp : This file contains the 'main' function. Program execution begins and ends there.
// g++ -fsanitize=address race_condition.cpp
// ------------------------------------------

#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

//#define RACE_CONDITION

class ForwardList {

    struct Node {
        Node* next_;
    };

    Node* head_{ nullptr };
    std::mutex mutex_;

public:
    void Push() {

        Node* new_head = new Node{};

#ifndef RACE_CONDITION
        std::lock_guard<std::mutex> lock(mutex_);
#endif
        new_head->next_ = head_;
        head_ = new_head;
    }

    void Pop() {

#ifndef RACE_CONDITION
        std::lock_guard<std::mutex> lock(mutex_);
#endif
        Node* p = head_;
        head_ = p->next_; // not safe (!) but works
        delete p;
    }
};

int main() {

    ForwardList list;

    // Work test

    list.Push();
    list.Push();
    list.Pop();
    list.Pop();
    list.Push();
    list.Pop();

    // Concurrent access

    std::vector<std::thread> threads;

    for (size_t i = 0; i < 5; ++i) {
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
