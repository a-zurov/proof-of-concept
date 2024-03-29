// message_passing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <atomic>
#include <thread>
#include <cassert>

static const int g_nTries = 10, g_nSignals = 100;

int main()
{
    int a = 0;

    std::atomic<int> x;
    std::atomic<int> y;

    for (int k = 0; k < g_nTries; ++k) {

        x.store(0);
        y.store(0);

        std::thread producer{
            [&a, &x, &y]() {
                for (int j = 1; j < g_nSignals; ++j) {
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
                    int j = x.load();
                    if (!j) continue;
                    if (p == j) {
                        std::this_thread::yield();
                        continue;
                    }
                    p = j;
                    int b = a;
                    y.store(j);
                    assert(j == b);
                    if (g_nSignals - 1 == j) break;
                }
            }
        };

        producer.join();
        consumer.join();
    }
}
