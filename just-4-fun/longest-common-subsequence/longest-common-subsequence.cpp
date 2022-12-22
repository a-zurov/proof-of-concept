// longest-common-subsequence.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>

using namespace std;

int main()
{
    string s = "gamadzhoba";
    cout << s << ' ' << s.size() << '\n';

    auto lcs = [](const string& a, const string& b, const string& c)
    {
        short m[31][31][31] = {};

        auto cube_exclude_vertex_max = [&](int i, int j, int k) {

            short res = 0;
            for (int p = 0; p < 2; ++p) {
                for (int q = 0; q < 2; ++q) {
                    for (int l = 0; l < 2; ++l) {
                        if (p + q + l != 3) res = max(m[i + p][j + q][k + l], res);
                    }
                }
            }
            return res;
        };

        for (int k = 0; k < c.size(); ++k) {
            for (int i = 0; i < a.size(); ++i) {
                for (int j = 0; j < b.size(); ++j) m[i + 1][j + 1][k + 1] =
                    (a[i] == b[j] && b[j] == c[k] ? m[i][j][k] + 1 : cube_exclude_vertex_max(i, j, k));
            }
        }

        return m[a.size()][b.size()][c.size()];
    };

    string a, b, c;
    a = "xsxsgxsaxsmadxsxszhoxsxsbxsaxs";
    b = "gamawqdwqzwqwqwqwqhwqowqwqbwqa";
    c = "gamadzhobagamadzhobagamadzhoba";

    cout << a << ' ' << a.size() << '\n';
    cout << b << ' ' << b.size() << '\n';
    cout << c << ' ' << c.size() << '\n';

    cout << lcs(a, b, c) << '\n';
}
