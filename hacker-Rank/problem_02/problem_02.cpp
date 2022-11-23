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

#define TAG_CLOSE "</"
#define TAG_CLOSE_SIZE 2

enum class Switch { name, value, delim };

int main()
{

#ifdef INPUT_FROM_FILE
    ifstream input_stream("test2.txt");
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
    string line, tagfull;

    for (int j = 0; j < N && getline(input_stream, line); ++j)
    {
        if (0 == strcmp(TAG_CLOSE, line.substr(0, TAG_CLOSE_SIZE).c_str())) {

            string tag_close = line.substr(2, line.size() - 3);
            std::size_t pos = tagfull.find(tag_close);
            if (pos != string::npos) {
                if (pos != 0) {
                    tagfull.erase(pos - 1, tag_close.length() + 1);
                }
                else {
                    tagfull.erase(pos, max(tag_close.length() + 1, tagfull.size()));
                }
            }
            continue;
        }

        stringstream ss(line);
        string tag;

        if (!getline(ss, tag, ' ')) break;


        tag = tag.substr(1, '>' == *(tag.end()-1) ? tag.size() - 2: tag.size() - 1);


        if (tagfull.empty()) tagfull = tag;
        else tagfull.append('.' + tag);

        mapHRML[tagfull] = {};

        string pattern, name;
        Switch sw = Switch::name;

        while (getline(ss, pattern, ' '))
        {
            if (pattern.empty()) continue;
            if (pattern == "\"") {
                if (!getline(ss, pattern, '\"')) continue;
                pattern = "\" " + pattern + '\"';
            }

            switch (sw) {
            case Switch::name:
                if ('=' == *(pattern.end()-1)) {
                    name = pattern.substr(0, pattern.size()-1);
                    sw = Switch::value;
                    continue;
                }
                name = pattern;
                sw = Switch::delim;
                break;
            case Switch::delim:
                if ('=' == *pattern.begin()) {
                    sw = Switch::value;
                }
                else {
                    mapHRML[tagfull][name] = {};
                    sw = Switch::name;
                }
                break;
            case Switch::value:
                mapHRML[tagfull][name] = pattern.substr(1, pattern.size() - ('>' == *(pattern.end() - 1) ? 3 : 2));
                sw = Switch::name;
            }
        }
    }

    for (int j = 0; j < Q && getline(input_stream, line); ++j) {

        size_t nPosDelim = line.rfind('~');
        string tag = line.substr(0, nPosDelim);
        string name = line.substr(nPosDelim + 1, line.size());
        string value = mapHRML[tag][name];

        cout << (value.empty() ? "Not Found!" : value.c_str()) << '\n';
    }

#ifdef INPUT_FROM_FILE
    input_stream.close();
#endif
}
