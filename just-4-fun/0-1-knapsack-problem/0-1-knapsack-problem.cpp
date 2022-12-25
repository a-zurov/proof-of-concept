// 0-1-knapsack-problem.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>

using namespace std;

int main()
{
    int nWeightMax = 11;
    int nItemMax = 5;

    vector<int> weight = { 0,2,5,3,7,4 };
    vector<int> profit = { 0,4,3,5,1,2 };

    int arrProfitMaxSolution[31][31] = {};

    for (int i = 1; i <= nItemMax; ++i) {

        cout << arrProfitMaxSolution[i][0] << ' ';

        for (int w = 1; w <= nWeightMax; ++w) {

            arrProfitMaxSolution[i][w] = w < weight[i] ? 0 : profit[i];

            cout << arrProfitMaxSolution[i][w] << ' ';
        }
        cout << '\n';
    };

    cout << '\n';


    for (int i = 1; i <= nItemMax; ++i) {

        cout << arrProfitMaxSolution[i][0] << ' ';

        for (int w = 1; w <= nWeightMax; ++w) {
            if (w < weight[i])
                arrProfitMaxSolution[i][w] = arrProfitMaxSolution[i-1][w];
            else
                arrProfitMaxSolution[i][w] =
                    max( arrProfitMaxSolution[i-1][w], arrProfitMaxSolution[i-1][w - weight[i]] + profit[i] );

            cout << arrProfitMaxSolution[i][w] << ' ';
        }
        cout << '\n';
    };

    return 0;

}
