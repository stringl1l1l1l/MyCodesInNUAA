#include <bits/stdc++.h>
using namespace std;

const int MAXCITA = 1e8;
int diff[MAXCITA + 1] = {0};
vector<pair<int, int>> pairs;

bool cmp(pair<int, int> p1, pair<int, int> p2)
{
    return p1.first < p2.first;
}

int main()
{
    int m, max = 0;
    cin >> m;
    for (int i = 0; i < m; i++)
    {
        int yi, resulti;
        cin >> yi >> resulti;
        pairs.push_back(make_pair(yi, resulti));
    }

    sort(pairs.begin(), pairs.end(), cmp);
    int maxYi = pairs[pairs.size() - 1].first;
    for (auto p : pairs)
    {
        // 当res为1，对小于等于他的阈值有贡献
        if (p.second)
        {
            diff[0]++;
            diff[p.first + 1]--;
        }
        // 当res为0, 对比他大的阈值都有贡献
        else
        {
            diff[p.first + 1]++;
        }
    }
    int count = 0, ans = 0;
    for (int i = 0; i <= maxYi; i++)
    {
        count += diff[i];
        if (count >= max)
        {
            max = count;
            ans = i;
        }
    }
    cout << ans;
}