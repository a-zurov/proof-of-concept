// problem_14.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ------------------------------------------------------------------------------------------------
// https://www.hackerrank.com/challenges/abstract-classes-polymorphism/problem

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <set>
#include <cassert>

// comment this macro and check includes before copying to HackerRank editor
#define INPUT_FROM_FILE

#ifdef INPUT_FROM_FILE
#include <fstream>
#endif

using namespace std;

struct Node {
    Node* next;
    Node* prev;
    int value;
    int key;
    Node(Node* p, Node* n, int k, int val) :prev(p), next(n), key(k), value(val) {};
    Node(int k, int val) :prev(NULL), next(NULL), key(k), value(val) {};
};

class Cache {

protected:
    map<int, Node*> mp; //map the key to the node in the linked list
    int cp;  //capacity
    Node* tail; // double linked list tail pointer
    Node* head; // double linked list head pointer
    virtual void set(int, int) = 0; //set function
    virtual int get(int) = 0; //get function

};

#include <list>

struct Node2 : public Node {
    std::list<Node2*>::iterator m_it;
    Node2(int k, int val) : Node(k, val) {};
};

class LRUCache : public Cache {

    using CacheList_t = std::list<Node2*>;

    CacheList_t m_listCache;
    int m_nSize;
public:
    LRUCache(int capacity) : m_nSize(capacity) {}

    void MoveCacheNodeToFront(CacheList_t::iterator& it)
    {
        // move a cache list node corresponding to the iterator to front
        m_listCache.splice(m_listCache.begin(), // destination position
            m_listCache, // source list
            it); // source position
        // std::list<T,Allocator>::splice transfers elements from one list to another.
        // Complexity: constant (for this signature)
    }

    int get(int nKey) override
    {
        auto itMap = mp.find(nKey);
        if (mp.end() == itMap) return -1;
        Node* pNode = itMap->second;
        Node2* pNode2 = static_cast<Node2*>(pNode);
        MoveCacheNodeToFront(pNode2->m_it);
        return pNode->value;
    }

    void set(int nKey, int nValue) override
    {
        auto itMap = mp.find(nKey);
        if (mp.end() != itMap)
        {
            Node* pNode = itMap->second;
            Node2* pNode2 = static_cast<Node2*>(pNode);
            MoveCacheNodeToFront(pNode2->m_it);
            pNode->value = nValue;
            return;
        }

        if (m_listCache.size() == m_nSize)
        {
            Node2* pNode2 = m_listCache.back();
            mp.erase(static_cast<Node*>(m_listCache.back())->key);
            m_listCache.pop_back();
            delete pNode2;
        }

        Node2* p = new Node2(nKey, nValue);
        m_listCache.push_front(p);
        p->m_it = m_listCache.begin();
        mp[nKey] = static_cast<Node*>(p);
    }
};

int main() {

#ifdef INPUT_FROM_FILE
    ifstream input_stream("test.txt"); // 2, -1
    if (!input_stream.is_open()) {
        std::cout << "File 'test.txt' is not found : test stopped..\n";
        return 0;
    }
#else
#define input_stream std::cin
#endif

    int n, capacity, i;
    input_stream >> n >> capacity;
    LRUCache l(capacity);
    for (i = 0; i < n; i++) {
        string command;
        input_stream >> command;
        if (command == "get") {
            int key;
            input_stream >> key;
            cout << l.get(key) << endl;
        }
        else if (command == "set") {
            int key, value;
            input_stream >> key >> value;
            l.set(key, value);
        }
    }

#ifdef INPUT_FROM_FILE
    input_stream.close();
#endif

    return 0;
}
