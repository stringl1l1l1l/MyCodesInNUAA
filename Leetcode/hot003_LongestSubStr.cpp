#include <map>
#include <iostream>
#include <string>
#include <string.h>
#include <algorithm>
using namespace std;

// 动态规划超时
class Solution
{
public:
    int lengthOfLongestSubstring(string s)
    {
        // 哈希集合，记录每个字符是否出现过
        unordered_set<char> occ;
        int n = s.size();
        // 右指针，初始值为 -1，相当于我们在字符串的左边界的左侧，还没有开始移动
        int rk = -1, ans = 0;
        // 枚举左指针的位置，初始值隐性地表示为 -1
        for (int i = 0; i < n; ++i)
        {
            if (i != 0)
            {
                // 左指针向右移动一格，移除一个字符
                occ.erase(s[i - 1]);
            }
            while (rk + 1 < n && !occ.count(s[rk + 1]))
            {
                // 不断地移动右指针
                occ.insert(s[rk + 1]);
                ++rk;
            }
            // 第 i 到 rk 个字符是一个极长的无重复字符子串
            ans = max(ans, rk - i + 1);
        }
        return ans;
    }

    // int lengthOfLongestSubstring(string s)
    // {
    //     if (s.empty())
    //         return 0;
    //     int len = s.size();
    //     int max = 1;
    //     int **d = new int *[len];
    //     for (int i = 0; i < len; i++)
    //     {
    //         d[i] = new int[len];
    //         for (int j = 0; j < len; j++)
    //         {
    //             d[i][j] = 0;
    //         }
    //     }
    //     for (int i = 0; i < len; i++)
    //     {
    //         d[i][i] = 1;
    //     }
    //     for (int i = 0; i < len - 1; i++)
    //     {
    //         for (int j = i + 1; j < len; j++)
    //         {
    //             int hashMap[256] = {0};
    //             string substring = s.substr(i, j - i);
    //             for (int k = 0; k < substring.size(); k++)
    //             {
    //                 hashMap[substring[k]]++;
    //             }
    //             int ch = hashMap[s[j]];
    //             if (ch == 0)
    //             {
    //                 d[i][j] = d[i][j - 1] + 1;
    //                 if (max < d[i][j])
    //                     max = d[i][j];
    //             }
    //         }
    //     }
    //     return max;
    // }
};