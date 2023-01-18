#include <vector>
using namespace std;

class Solution
{
public:
    int searchInsert(vector<int> &nums, int target)
    {
        int n = nums.size();
        int low = 0, high = n - 1, mid;
        while (low <= high)
        {
            mid = ((high - low) >> 1) + low;
            if (nums[mid] > target)
                high = mid - 1;
            else if (nums[mid] < target)
                low = mid + 1;
            else
                return mid;
        }
        // if(nums[mid]>target)
        // {
        //     if(mid > 0)
        //         return mid-1;
        //     else
        //         return 0;
        // }
        // else
        // {
        //     if(mid<nums.size()-1)
        //         return mid + 1;
        //     else
        //         return nums.size();
        // }
        return low;
    }
};