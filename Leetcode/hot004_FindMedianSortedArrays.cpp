#include <map>
#include <iostream>
#include <string>
#include <string.h>
#include <algorithm>
#include <vector>
using namespace std;

class Solution
{
public:
    double findMedianSortedArrays(vector<int> &nums1, vector<int> &nums2)
    {
        int len1 = nums1.size(), len2 = nums2.size();
        int amount = len1 + len2;
        if (amount & 0b1 == 1) // 奇数
        {
            return getKthElement(nums1, nums2, amount / 2 + 1);
        }
        else
            return (getKthElement(nums1, nums2, amount / 2) + getKthElement(nums1, nums2, amount / 2 + 1)) / 2;
    }

    int getKthElement(const vector<int> &nums1, const vector<int> &nums2, int k)
    {
        int len1 = nums1.size(), len2 = nums2.size();
        int beg1 = 0, beg2 = 0;
        while (true)
        {
            // 若第一个数组为空
            if (beg1 == len1)
            {
                return nums2[beg2 + k - 1];
            }
            else if (beg2 == len2)
            {
                return nums1[beg1 + k - 1];
            }
            else if (k == 1)
            {
                return min(nums1[beg1], nums2[beg2]);
            }
            // 含有删除操作，引入beg变量
            // min()防止越界
            int privotIndex1 = min(beg1 + k / 2, len1) - 1,
                privotIndex2 = min(beg2 + k / 2, len2) - 1;
            int privot1 = nums1[privotIndex1],
                privot2 = nums2[privotIndex2];
            if (privot1 <= privot2)
            {
                k -= privotIndex1 - beg1 + 1;
                beg1 = privotIndex1 + 1;
            }
            else
            {
                k -= privotIndex2 - beg2 + 1;
                beg2 = privotIndex2 + 1;
            }
        }
    }
};