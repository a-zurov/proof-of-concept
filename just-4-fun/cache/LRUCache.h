#pragma once

#include <list>
#include <unordered_map>

#include <cassert>

class LRUCache
{
    using ListItem_t = std::pair<int, int>; // { key, value }
    using List_t = std::list<ListItem_t>;
    using Map_t = std::unordered_map<int, List_t::iterator>;

    List_t m_list;
    Map_t m_map;
    int m_size;

    void MoveNodeToFront(const List_t::iterator& it);

public:
    LRUCache(int capacity) : m_size(capacity) {}

    int Get(int nKey);
    void Put(int nKey, int nValue);
};


class LRUCache_Fast
{
    std::unordered_map<int, std::pair<int, std::list<int>::iterator>> m_map; // will keep only existing
    std::list<int> m_list;
    int m_size;

    void erase(int key);
    void push(int key, int value);

public:
    LRUCache_Fast(int capacity) : m_size(capacity) {}

    int Get(int key);
    void Put(int key, int value);
};
