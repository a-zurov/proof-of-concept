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

        x.store(0);

        std::thread producer{
            [&a, &x, &y]() {
                for (int j = 1; j < g_nSignals; ++j) {
                    // TODO: Store
                    a = j;
                    x.store(j);
                    while (y.load() != j); {
                        std::this_thread::yield();
                    }
                }
            }
        };
        std::thread consumer{
            [&a, &x, &y]() {
                int p = 0;
                for (;;) {
                    // TODO: Load
                    int j = x.load();
                    if (!j) continue;
                    if (p == j) {
                        std::this_thread::yield();
                        continue;
                    }
                    p = j;
                    int b = a;
                    y.store(j);
                    if (j != b) {
                        std::cout << j << ' ' << b << '\n';
                    }
                }
            }
        };

        producer.join();
        consumer.join();
    }

}
