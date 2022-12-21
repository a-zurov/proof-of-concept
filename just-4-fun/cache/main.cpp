// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "LRUCache.h"
#include "LFUCache.h"

int main()
{
    std::unique_ptr<LRUCache> spCache(new LRUCache(2));

    spCache->Put(1, 1);
    spCache->Put(2, 2);
    std::cout << spCache->Get(1) << "\n"; // 1
    spCache->Put(3, 3);
    std::cout << spCache->Get(2) << "\n"; // -1

    std::unique_ptr<LRUCache_Fast> spCache_Fast(new LRUCache_Fast(2));

    spCache_Fast->Put(1, 1);
    spCache_Fast->Put(2, 2);
    std::cout << spCache_Fast->Get(1) << "\n"; // 1
    spCache_Fast->Put(3, 3);
    std::cout << spCache_Fast->Get(2) << "\n"; // -1

    std::unique_ptr<LFUCache> spCache1(new LFUCache(2));
    spCache1->Put(1, 1);
    spCache1->Put(2, 2);
    std::cout << spCache1->Get(1) << "\n"; // 1
    spCache1->Put(3, 3);
    std::cout << spCache1->Get(2) << "\n"; // -1
    std::cout << spCache1->Get(3) << "\n"; // 3
    spCache1->Put(4, 4);
    std::cout << spCache1->Get(1) << "\n"; // -1
    std::cout << spCache1->Get(3) << "\n"; // 3
    std::cout << spCache1->Get(4) << "\n"; // 4


    std::unique_ptr<LFUCache> spCache2(new LFUCache(3));
    spCache2->Put(2, 2);
    spCache2->Put(1, 1);
    std::cout << spCache2->Get(2) << "\n"; // 2
    std::cout << spCache2->Get(1) << "\n"; // 1
    std::cout << spCache2->Get(2) << "\n"; // 2
    spCache2->Put(3, 3);
    spCache2->Put(4, 4);
    std::cout << spCache2->Get(3) << "\n"; // -1
    std::cout << spCache2->Get(2) << "\n"; // 2
    std::cout << spCache2->Get(1) << "\n"; // 1
    std::cout << spCache2->Get(4) << "\n"; // 4


    std::unique_ptr<LFUCache> spCache3(new LFUCache(2));
    spCache3->Put(3, 1);
    spCache3->Put(2, 1);
    spCache3->Put(2, 2);
    spCache3->Put(4, 4);
    std::cout << spCache3->Get(2) << "\n"; // 2

}
