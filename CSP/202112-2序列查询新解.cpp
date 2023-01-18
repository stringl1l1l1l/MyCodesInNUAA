#include <bits/stdc++.h>
using namespace std;

unordered_map<int, int> diffF, diffG, diff;

int main()
{
    ios::sync_with_stdio(false), cin.tie(NULL);
    int n, N, r;
    long long err = 0;
    cin >> n >> N;
    for (int i = 1; i <= n; i++)
    {
        int ai;
        cin >> ai;
        diffF[ai] = 1;
    }

    r = N / (n + 1);
    for (int i = r; i < N; i += r)
        diffG[i] = -1;

    vector<int> keys;

    for (auto &f : diffF)
    {
        diff[f.first] += f.second;
    }
    for (auto &g : diffG)
    {
        diff[g.first] += g.second;
    }

    for (auto &d : diff)
    {
        if (d.second != 0)
            keys.push_back(d.first);
    }
    diff[N] = 0;
    keys.push_back(N);
    sort(keys.begin(), keys.end());

    int lastD = 0;
    for (int i = 0; i < keys.size() - 1; i++)
    {
        int curVal = diff[keys[i]] + lastD;
        int count = keys[i + 1] - keys[i];
        err += abs(count * curVal);
        lastD = curVal;
    }
    cout << err;
}