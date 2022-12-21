
#include "LRUCache.h"

#include <cassert>

void LRUCache::MoveNodeToFront(const List_t::iterator& itSrc)
{
    // move a cache list node corresponding itSrc to m_list.begin()
    m_list.splice(  m_list.begin(), // destination position
                    m_list,         // source list
                    itSrc );        // source position
}

int LRUCache::Get(int nKey)
{
    if (0 == m_size) return -1;

    auto itMap = m_map.find(nKey);
    if (m_map.end() == itMap) return -1;

    auto itList = itMap->second;
    MoveNodeToFront(itList);
    return itList->second;
}

void LRUCache::Put(int nKey, int nValue)
{
    if (0 == m_size) return;

    auto itMap = m_map.find(nKey);
    if (m_map.end() != itMap) {
        auto itList = itMap->second;
        itList->second = nValue;
        MoveNodeToFront(itList);
        return;
    }

    if (m_list.size() == m_size) {
        m_map.erase(m_list.back().first);
        m_list.pop_back();
    }

    m_list.push_front(std::make_pair(nKey, nValue));

    m_map[nKey] = m_list.begin();
}

// ------------------------------------------------------------------------

void LRUCache_Fast::erase(int key)
{
    auto it = m_map.find(key);
    assert(it != m_map.end());
    m_list.erase(it->second.second);
    m_map.erase(it);
}

void LRUCache_Fast::push(int key, int value)
{
    m_list.push_back(key);
    m_map[key] = { value, prev(m_list.end()) };
}

int LRUCache_Fast::Get(int key)
{
    if (0 == m_size) return -1;

    auto it = m_map.find(key);
    if (it == m_map.end()) {
        return -1;
    }
    int old_value = it->second.first;
    erase(key);
    push(key, old_value);
    return old_value;
}

void LRUCache_Fast::Put(int key, int value)
{
    if (0 == m_size) return;

    if (m_map.find(key) != m_map.end()) {
        erase(key);
    }
    if ((int)m_map.size() == m_size) {
        assert(!m_list.empty());
        erase(m_list.front());
    }
    push(key, value);
}
