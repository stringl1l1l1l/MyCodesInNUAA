#include <vector>
#include <math.h>
#include <algorithm>
using namespace std;

class Solution
{
public:
    vector<int> sortedSquares(vector<int> &nums)
    {
        vector<int> res;
        int i = 0, j = nums.size() - 1;
        while (i <= j)
        {
            if (i <= j && abs(nums[i]) >= abs(nums[j]))
            {
                res.push_back(nums[i] * nums[i]);
                i++;
            }
            if (i <= j && abs(nums[j]) >= abs(nums[i]))
            {
                res.push_back(nums[j] * nums[j]);
                j--;
            }
        }
        reverse(res.begin(), res.end());
        return res;
    }
};