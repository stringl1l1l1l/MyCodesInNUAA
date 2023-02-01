// The API isBadVersion is defined for you.
bool isBadVersion(int version);

class Solution
{
public:
    int firstBadVersion(int n)
    {
        long long low = 1, high = n;
        while (low < high)
        {
            long long mid = low + ((high - low) >> 1); // 注意加括号
            if (isBadVersion(mid))
                high = mid;
            else
                low = mid + 1;
        }
        return (int)low;
    }
};