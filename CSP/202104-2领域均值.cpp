#include <iostream>
#include <set>
#include <math.h>
#include <cstdio>
#include <string.h>
using namespace std;

int A[602][602];
int preSum[602][602];

int main()
{
    int n, L, r, t;
    int ans = 0;
    cin >> n >> L >> r >> t;
    for (int i = 0; i <= n; i++)
    {
        for (int j = 0; j <= n; j++)
        {
            A[i][j] = 0;
            preSum[i][j] = 0;
        }
    }

    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            cin >> A[i][j];
        }
    }

    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            preSum[i][j] = preSum[i - 1][j] + preSum[i][j - 1] - preSum[i - 1][j - 1] + A[i][j];
        }
    }
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            int x1 = (i - r) > 1 ? (i - r) : 1,
                y1 = (j - r) > 1 ? (j - r) : 1,
                x2 = (i + r) < n ? (i + r) : n,
                y2 = (j + r) < n ? (j + r) : n;
            float avg = (preSum[x2][y2] - preSum[x2][y1 - 1] - preSum[x1 - 1][y2] + preSum[x1 - 1][y1 - 1]) /
                        (1.0 * (y2 - y1 + 1) * (x2 - x1 + 1));
            // 平均值必须设为浮点数变量，两个整型数运算还需要在除数乘上1.0
            if (avg <= t)
                ans++;
        }
    }
    cout << ans;
}
