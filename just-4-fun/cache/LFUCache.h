#pragma once

#include <list>
#include <unordered_map>

class LFUCache
{
    using ListItem_t = std::pair<int, std::pair<int, int>>; // { counter, {key,value} }
    using List_t = std::list<ListItem_t>;
    using Map_t = std::unordered_map<int, List_t::iterator>;

    List_t m_list;
    Map_t m_map;
    int m_size;

    void MoveNodeToFront(List_t::iterator& it);

public:
    LFUCache(int capacity) : m_size(capacity) {}

    int Get(int nKey);
    void Put(int nKey, int nValue);
};

