// message_passing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>

static const int g_nTries = 1, g_nSignals = 100;

int main()
{
    int a = 0;

    std::atomic<int> x;
    std::atomic<int> y;

    for (int k = 0; k < g_nTries; ++k) {

        std::thread producer{
            [&a, &x, &y]() {
                for (int j = 1; j < g_nSignals; ++j) {
                    // TODO: Store
                }
            }
        };
        std::thread consumer{
            [&a, &x, &y]() {
                for (;;) {
                    // TODO: Load
                }
            }
        };

        producer.join();
        consumer.join();
    }

}
