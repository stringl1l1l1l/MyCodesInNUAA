#include <bits/stdc++.h>
using namespace std;

class Solution
{
public:
    vector<int> searchRange(vector<int> &nums, int target)
    {
        int n = nums.size();
        int left = binarySearch2(nums, target),
            right = binarySearch2(nums, target + 1);

        if (left == n || nums[left] != target)
            return vector<int>({-1, -1});
        else
            return vector<int>({left, right - 1});
    }
    int binarySearch(vector<int> &nums, int target)
    {
        // 如果没有搜索到target,会返回第一个大于target值的索引，
        // 若target超过所有数的范围，返回索引就会变为n
        int n = nums.size();
        int beg = 0, end = n;
        while (beg < end)
        {
            int mid = ((end - beg) >> 1) + beg;
            if (nums[mid] >= target)
                end = mid;
            else
                beg = mid + 1;
        }
        return beg;
    }
    int binarySearch2(vector<int> &nums, int target)
    {
        int n = nums.size();
        int beg = 0, end = n - 1;
        while (beg <= end)
        {
            int mid = ((end - beg) >> 1) + beg;
            if (nums[mid] >= target)
                end = mid - 1;
            // 等于的情况下为什么可以取mid-1?
            // 假设nums[mid] == target时，此时的mid处正好是第一个等于target的数
            // end=mid-1后，便不可能再改变了，因为[beg,end]区间的值都比target小
            // 直到最后beg == end, beg=mid+1退出循环，此时beg的值就是最后结果
            else
                beg = mid + 1;
        }
        return beg;
    }
};

int main()
{
    Solution s;
    vector<int> nums({4, 5, 7, 7, 9});
    auto v = s.searchRange(nums, 7);
    for (auto &mem : v)
        cout << mem << " ";
}