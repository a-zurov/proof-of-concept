#include "LRUCache.h"

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
