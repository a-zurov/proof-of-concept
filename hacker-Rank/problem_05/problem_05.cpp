// problem_05.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ------------------------------------------------------------------------------------------------
// https://www.hackerrank.com/challenges/exceptional-server/problem

#include <iostream>
#include <exception>
#include <string>
#include <stdexcept>
#include <vector>
#include <cmath>

using namespace std;


class Server {
private:
    static int load;
public:
    static int compute(long long A, long long B) {
        load += 1;
        if (A < 0) {
            throw std::invalid_argument("A is negative");
        }
        vector<int> v(A, 0);
        int real = -1, cmplx = sqrt(-1);
        if (B == 0) throw 0;
        real = (A / B) * real;
        int ans = v.at(B);
        return real + A - B * ans;
    }
    static int getLoad() {
        return load;
    }
};
int Server::load = 0;

// comment this macro and check includes before copying to HackerRank editor
#define INPUT_FROM_FILE

#ifdef INPUT_FROM_FILE
#include <fstream>
std::ifstream input_stream("test.txt");
#else
#define input_stream std::cin
#endif //INPUT_FROM_FILE

int main() {

#ifdef INPUT_FROM_FILE
    if (!input_stream.is_open()) {
        std::cout << "File 'test.txt' is not found : test stopped..\n";
        return 0;
    }
#endif //INPUT_FROM_FILE

    int T; cin >> T;
    while (T--) {
        long long A, B;
        cin >> A >> B;

        /* Enter your code here. */
        Server s;
        try {
            cout << s.compute(A, B) << '\n';
        }
        catch (const std::invalid_argument ex) {
            cout << "Exception: " << ex.what() << '\n';
        }
        catch (const std::bad_alloc& ex) {
            cout << "Not enough memory\n";
        }
        catch (const exception& ex) {
            cout << "Exception: " << ex.what() << '\n';
        }
        catch (...) {
            cout << "Other Exception\n";
        }

    }
    cout << Server::getLoad() << endl;

#ifdef INPUT_FROM_FILE
    input_stream.close();
#endif
    return 0;
}
