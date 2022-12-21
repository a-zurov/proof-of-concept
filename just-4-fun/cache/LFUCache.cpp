#include "LFUCache.h"

void LFUCache::MoveNodeToFront(const List_t::iterator& itSrc)
{
    int counter = ++(itSrc->first);
    auto itDest = itSrc;
    auto itNext = itSrc;
    while (itNext != m_list.begin() && (--itNext)->first <= counter) {
        itDest = itNext;
    };

    // move a cache list node corresponding itSrc to itDest
    m_list.splice(  itDest,     // destination position
                    m_list,     // source list
                    itSrc );    // source position
}

int LFUCache::Get(int nKey)
{
    if (0 == m_size) return -1;

    auto itMap = m_map.find(nKey);
    if (m_map.end() == itMap) return -1;

    auto itList = itMap->second;
    MoveNodeToFront(itList);
    return (itList->second).second;
}

void LFUCache::Put(int nKey, int nValue)
{
    if (0 == m_size) return;

    auto itMap = m_map.find(nKey);
    if (m_map.end() != itMap) {
        auto itList = itMap->second;
        (itList->second).second = nValue;
        MoveNodeToFront(itList);
        return;
    }

    if (m_list.size() == m_size) {
        m_map.erase(m_list.back().second.first);
        m_list.pop_back();
    }

    m_list.push_back(std::make_pair(0, std::make_pair(nKey, nValue)));

    auto it = m_list.end();
    MoveNodeToFront(--it);
    m_map[nKey] = it;
}
