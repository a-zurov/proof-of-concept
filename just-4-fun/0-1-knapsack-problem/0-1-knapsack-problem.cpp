// 0-1-knapsack-problem.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>

using namespace std;

/*
// Leetcode 416. Partition Equal Subset Sum
// https://leetcode.com/problems/partition-equal-subset-sum/description/

bool canPartition(vector<int>& nums) {

    int sum = 0;
    for (auto n : nums) {
        sum += n;
    }
    if (sum % 2) return false;
    return IsSum(sum/2, nums );
}
*/

bool IsSum(int N, vector<int>& nums) {

    vector<vector<int>> arrProfitMaxSolution;
    arrProfitMaxSolution.resize(nums.size());
    for (auto& v : arrProfitMaxSolution) {
        v.resize(N + 1);
    }

    for (int w = 1; w <= N; ++w) {
        arrProfitMaxSolution[0][w] = w < nums[0] ? 0 : nums[0];
    }

    for (int i = 1; i < nums.size(); ++i) {
        for (int w = 1; w <= N; ++w) {
            if (w < nums[i])
                arrProfitMaxSolution[i][w] = arrProfitMaxSolution[i - 1][w];
            else
                arrProfitMaxSolution[i][w] =
                    max( arrProfitMaxSolution[i - 1][w],
                         arrProfitMaxSolution[i - 1][w - nums[i]] + nums[i]);

            if (N == arrProfitMaxSolution[i][w]) {
                return true;
            }
        }
    }
    return false;
}

int main()
{
    vector<int> nums = { 7,3,11,21,1,10 }; // Answer: 16 false

    for (int j = 10; j < 21; ++j) {
        cout << j << " is sum : " << (IsSum(j, nums) ? "true" : "false") << '\n';
    }

    int N = 269; // Answer: 295
    vector<int> weight = { 95,  4, 60, 32, 23, 72, 80, 62, 65, 46 };
    vector<int> profit = { 55, 10, 47,  5,  4, 50,  8, 61, 85, 87 };

    int arrProfitMaxSolution[300][300] = {};

    for (int w = 1; w <= N; ++w) {
        arrProfitMaxSolution[0][w] = w < weight[0] ? 0 : profit[0];
    }

    for (int i = 1; i < profit.size(); ++i) {
        for (int w = 1; w <= N; ++w) {
            if (w < weight[i])
                arrProfitMaxSolution[i][w] = arrProfitMaxSolution[i-1][w];
            else
                arrProfitMaxSolution[i][w] =
                    max( arrProfitMaxSolution[i-1][w],
                         arrProfitMaxSolution[i-1][w - weight[i]] + profit[i] );

            //cout << arrProfitMaxSolution[i][w] << ' ';
        }
        //cout << '\n';
    };

    cout << "Napsack = " << N << ", Profit = " << arrProfitMaxSolution[profit.size()-1][N] << ' ';

    return 0;

}
