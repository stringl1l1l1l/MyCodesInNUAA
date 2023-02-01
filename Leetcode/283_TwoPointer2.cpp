#include <vector>
using namespace std;

class Solution
{
public:
    // void moveZeroes(vector<int> &nums)
    // {
    //     int n = nums.size();
    //     int i = 0, j = n - 1;
    //     while (i < j)
    //     {
    //         if (nums[i] == 0)
    //         {
    //             for (int k = i + 1; k <= j; k++)
    //                 nums[k - 1] = nums[k];
    //             nums[j] = 0;
    //             j--;
    //         }
    //         else
    //             i++;
    //     }
    // }

    void moveZeroes(vector<int> &nums)
    {
        int n = nums.size();
        int i = 0, j = 0; // i指向第一个0，j指向i后面第一个非0数
        while (i < n)
        {
            if (nums[i])
                i++;
            else
                break;
        }
        j = i + 1;
        while (j < n)
        {
            if (!nums[j])
                j++;
            else
                break;
        }
        while (j < n)
        {
            if (nums[j])
            {
                int temp = nums[i];
                nums[i] = nums[j];
                nums[j] = temp;
                while (nums[i])
                    i++;
            }
            else
                j++;
        }
    }
    void moveZeroes2(vector<int> &nums)
    {
        int n = nums.size(), left = 0, right = 0;
        while (right < n)
        {
            if (nums[right])
            {
                swap(nums[left], nums[right]);
                left++;
            }
            right++;
        }
    }
};