// problem_494.cpp : This file contains the 'main' function. Program execution begins and ends there.
// -------------------------------------------------------------------------------------------------
// Leetcode 494. Target Sum
// https://leetcode.com/problems/target-sum/description/

#include <iostream>
#include <vector>

using namespace std;

int GetSumsCount(int N, vector<int>& nums) {

    vector<vector<int>> arrSumsCountSolution;
    arrSumsCountSolution.resize(nums.size());
    for (auto& v : arrSumsCountSolution) {
        v.resize(N + 1);
    }

    for (int w = 1; w <= N; ++w) {
        arrSumsCountSolution[0][w] = (w == nums[0] ? 1 : 0);
    }

    for (int i = 1; i < nums.size(); ++i) {
        for (int w = 1; w <= N; ++w) {
            if (w < nums[i])
                arrSumsCountSolution[i][w] = arrSumsCountSolution[i - 1][w];
            else if (w == nums[i])
                arrSumsCountSolution[i][w] = arrSumsCountSolution[i - 1][w] + 1;
            else
                arrSumsCountSolution[i][w] =
                    arrSumsCountSolution[i - 1][w] + arrSumsCountSolution[i - 1][w - nums[i]];
        }
    }
    return arrSumsCountSolution[nums.size() - 1][N];
}

int findTargetSumWays(vector<int>& nums, int target) {

    vector<int> nonzero_nums;
    int sum = 0;
    for (int n : nums) {
        sum += n;
        if (n) nonzero_nums.push_back(n);
    }

    /*
        sum(P) - sum(N) = target
        sum(P) + sum(N) + sum(P) - sum(N) = target + sum(P) + sum(N)
        2 * sum(P) = target + sum(nums)
    */
    int positive_sum = sum + target;

    if (positive_sum < 0) return 0;

    int z, zero_factor = ( z = nums.size() - nonzero_nums.size() ) ? pow(2,z) : 0;

    if (0 == positive_sum) return zero_factor ? zero_factor : 1;

    if (positive_sum % 2) return 0;

    int nonzero_count = GetSumsCount( positive_sum / 2, nonzero_nums );

    return zero_factor ? zero_factor * nonzero_count : nonzero_count;
}

int main()
{
    vector<int> nums = { 9, 7, 0, 3, 9, 8, 6, 5, 7, 6 };
    cout << findTargetSumWays(nums, 2);
}
