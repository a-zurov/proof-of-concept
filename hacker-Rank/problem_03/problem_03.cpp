// problem_03.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ------------------------------------------------------------------------------------------------
//

#include "./../bits/bits-stdc++.h"

using namespace std;

#define INPUT_FROM_FILE

string ltrim(const string&);
string rtrim(const string&);

// Complete the maxTickets function below.
int maxTickets(vector<int> tickets) {

    return 0;
}

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

    ofstream fout(getenv("OUTPUT_PATH"));

#endif //INPUT_FROM_FILE
    

    string tickets_count_temp;
    getline(input_stream, tickets_count_temp);

    int tickets_count = stoi(ltrim(rtrim(tickets_count_temp)));

    vector<int> tickets(tickets_count);

    for (int i = 0; i < tickets_count; i++) {
        string tickets_item_temp;
        getline(input_stream, tickets_item_temp);

        int tickets_item = stoi(ltrim(rtrim(tickets_item_temp)));

        tickets[i] = tickets_item;
    }

    int res = maxTickets(tickets);
    

#ifdef INPUT_FROM_FILE
    cout << res << "\n";
    input_stream.close();
#else
    fout << res << "\n";
    fout.close();
#endif //INPUT_FROM_FILE

    return 0;
}

string ltrim(const string& str) {
    string s(str);

    s.erase(
        s.begin(),
        find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace)))
    );

    return s;
}

string rtrim(const string& str) {
    string s(str);

    s.erase(
        find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
        s.end()
    );

    return s;
}
