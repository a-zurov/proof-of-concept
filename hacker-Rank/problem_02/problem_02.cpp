// problem_02.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ------------------------------------------------------------------------------------------------
//

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

#include <cstring>
#include <map>
#include <sstream>

using namespace std;

// comment this macro and check includes before copying to HackerRank editor
#define INPUT_FROM_FILE

#ifdef INPUT_FROM_FILE
#include <fstream>
#endif

#define TAG_OPEN "<tag"
#define TAG_OPEN_SIZE 4

enum class Switch { name, value, delim };

int main()
{

#ifdef INPUT_FROM_FILE
    ifstream input_stream("test.txt");
    if (!input_stream.is_open()) {
        std::cout << "File 'test.txt' is not found : test stopped..\n";
        return 0;
    }
#else
#define input_stream std::cin
#endif //INPUT_FROM_FILE

    int N, Q;
    input_stream >> N >> Q;

    char next;
    while (input_stream.get(next))
    {
        if (next == '\n') {
            break;
        }
    }

    map<string, map <string, string>> mapHRML;
    string line;

    for (int j = 0; j < N && getline(input_stream, line); ++j)
    {
        if (0 == strcmp(TAG_OPEN, line.substr(0, TAG_OPEN_SIZE).c_str()))
        {
            stringstream ss(line);
            string tag;

            if (getline(ss, tag, ' ')) {

                mapHRML[tag = tag.substr(1, tag.size() - 1)] = {};

                string pattern, name;
                Switch sw = Switch::name;

                while (getline(ss, pattern, ' '))
                {
                    switch (sw) {
                    case Switch::name:
                        name = pattern;
                        sw = Switch::delim;
                        break;
                    case Switch::delim:
                        if ('=' == *pattern.begin()) {
                            sw = Switch::value;
                        }
                        else {
                            mapHRML[tag][name] = {};
                            name.clear();
                            sw = Switch::name;
                        }
                        break;
                    case Switch::value:
                        mapHRML[tag][name] = pattern.substr(1, pattern.size() - ('>' == *(pattern.end() - 1) ? 3 : 2));
                        name.clear();
                        sw = Switch::name;
                    }
                }
            }
        }
    }

    for (int j = 0; j < Q && getline(input_stream, line); ++j) {

        size_t nPosDelim = line.rfind('~');
        size_t nPosTagBegin = line.rfind('.', nPosDelim);
        nPosTagBegin = string::npos == nPosTagBegin ? 0 : nPosTagBegin + 1;

        string tag = line.substr(nPosTagBegin, nPosDelim - nPosTagBegin);
        string name = line.substr(nPosDelim + 1, line.size());
        string value = mapHRML[tag][name];

        cout << (value.empty() ? "Not Found!" : value.c_str()) << '\n';
    }

#ifdef INPUT_FROM_FILE
    input_stream.close();
#endif
}
