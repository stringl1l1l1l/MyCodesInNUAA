#include <bits/stdc++.h>

using namespace std;
const int MAXVAL = 2e5;

int hashTable[MAXVAL + 1] = {0};
int q[MAXVAL + 1] = {0};
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int n, m, k;
    cin >> n >> m >> k;
    for (int i = 0; i < n; i++)
    {
        int ti, ci;
        cin >> ti >> ci;
        if (ti < k + 1)
            continue;
        int max = ti - k,
            min = (ti - ci + 1 - k) > 0 ? (ti - ci + 1 - k) : 1;
        for (int j = min; j <= max; j++)
            hashTable[j]++;
    }

    for (int i = 0; i < m; i++)
    {
        cin >> q[i];
    }

    for (int i = 0; i < m; i++)
    {
        cout << hashTable[q[i]];
        if (i < m - 1)
            cout << endl;
    }
}