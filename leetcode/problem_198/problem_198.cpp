// problem_198.cpp : This file contains the 'main' function. Program execution begins and ends there.
// -------------------------------------------------------------------------------------------------
// Leetcode 198. House Robber
// https://leetcode.com/problems/house-robber/description/
// https://leetcode.com/problems/house-robber-ii/description/

#include <iostream>
#include <vector>

using namespace std;

int rob(vector<int>& nums) {

    int N = nums.size();
    if (!N) return 0;
    if (1 == N) return nums[0];
    if (2 == N) return max(nums[0], nums[1]);

    vector<int> vecMoneyMaxSolution;
    vecMoneyMaxSolution.resize(N);

    auto getMoneyMax = [&](int n) -> int {
        for (int i = 2; i < n; ++i) {
            vecMoneyMaxSolution[i] =
                max(vecMoneyMaxSolution[i - 2] + nums[i],
                    vecMoneyMaxSolution[i - 1]);
        }
        return vecMoneyMaxSolution[n - 1];
    };

    vecMoneyMaxSolution[0] = nums[0];
    vecMoneyMaxSolution[1] = nums[0];

    int begin_first_house_amount = getMoneyMax(N);

    std::fill(vecMoneyMaxSolution.begin(), vecMoneyMaxSolution.end(), 0);

    vecMoneyMaxSolution[0] = 0;
    vecMoneyMaxSolution[1] = nums[1];

    int begin_second_house_amount = getMoneyMax(N);

    return max(begin_first_house_amount, begin_second_house_amount);
}

int cycle_rob(vector<int>& nums) {

    int N = nums.size();
    if (!N) return 0;
    if (1 == N) return nums[0];
    if (2 == N) return max(nums[0], nums[1]);

    vector<int> vecMoneyMaxSolution;
    vecMoneyMaxSolution.resize(N);

    auto getMoneyMax = [&](int n) -> int {
        for (int i = 2; i < n; ++i) {
            vecMoneyMaxSolution[i] =
                max(vecMoneyMaxSolution[i - 2] + nums[i],
                    vecMoneyMaxSolution[i - 1]);
        }
        return vecMoneyMaxSolution[n - 1];
    };

    vecMoneyMaxSolution[0] = nums[0];
    vecMoneyMaxSolution[1] = nums[0];

    int begin_first_house_amount = getMoneyMax(N - 1);

    std::fill(vecMoneyMaxSolution.begin(), vecMoneyMaxSolution.end(), 0);

    vecMoneyMaxSolution[0] = 0;
    vecMoneyMaxSolution[1] = nums[1];

    int begin_second_house_amount = getMoneyMax(N);

    return max(begin_first_house_amount, begin_second_house_amount);
}

int main()
{
    vector<int> nums = { 2,1,1,2 }; // 4
    std::cout << rob(nums);
}
