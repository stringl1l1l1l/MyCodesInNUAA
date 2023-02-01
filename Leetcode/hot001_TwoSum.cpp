#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;
bool cmp(pair<int, int> p1, pair<int, int> p2)
{
    return p1.second < p2.second;
}
class Solution
{
public:
    vector<int> twoSum(vector<int> &nums, int target)
    {
        int len = nums.size();
        vector<pair<int, int>> map;
        vector<int> answer;
        for (int i = 0; i < len; i++)
        {
            map.push_back(make_pair(i, nums[i]));
        }
        sort(map.begin(), map.end(), [&](pair<int,int>p1, pair<int,int>p2 )->bool{return p1.second<p2.second;});
        for (int i = 0; i < len - 1; i++)
        {
            int another = this->binarySearch(map, i + 1, len - 1, target - map[i].second);
            if (another != -1)
            {
                answer.push_back(map[i].first);
                answer.push_back(map[another].first);
                return answer;
            }
        }
        return answer;
    }

    int binarySearch(vector<pair<int, int>> &map, int low, int high, int target)
    {
        int mid = ((high - low) >> 1) + low;
        while (low <= high)
        {
            mid = ((high - low) >> 1) + low;
            if (map[mid].second < target)
                low = mid + 1;
            else if (map[mid].second > target)
                high = mid - 1;
            else
                return mid;
        }
        return -1;
    }
};

// 请记住二分查找只能应用于有序数组