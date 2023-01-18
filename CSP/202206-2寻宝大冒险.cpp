#include <bits/stdc++.h>

using namespace std;

const int MAXL = 1e9;
const int MAXS = 50;
const int MAXN = 1e3;

int A[MAXN][2];
int B[MAXS + 1][MAXS + 1]; // 数组记得开大一点！
int main()
{
    ios::sync_with_stdio(false);
    cout.tie(NULL);
    cin.tie(NULL);

    int n, L, S;
    int ans = 0;
    int mapCount = 0;
    cin >> n >> L >> S;
    for (int i = 0; i < n; i++)
        cin >> A[i][0] >> A[i][1];
    for (int i = S; i >= 0; i--)
    {
        for (int j = 0; j <= S; j++)
        {
            cin >> B[i][j];
            if (B[i][j])
                mapCount++;
        }
    }

    for (int i = 0; i < n; i++)
    {
        // 以当前树作为藏宝图起始点
        int begX = A[i][0], begY = A[i][1];
        int flag = 0;

        // 计算藏宝图范围内树木的个数,个数不匹配直接退出
        int treasureCount = 0;
        for (int j = 0; j < n; j++)
        {
            if (A[j][0] - begX >= 0 && A[j][0] - begX <= S && A[j][1] - begY >= 0 && A[j][1] - begY <= S)
                treasureCount++;
        }
        if (treasureCount != mapCount)
            continue;

        //遍历藏宝图每个点，看其偏移后的坐标是否能与地图对应
        for (int j = 0; j <= S; j++)
        {
            for (int k = 0; k <= S; k++)
            {
                //偏移后在地图中对应的坐标
                int curX = begX + j,
                    curY = begY + k;

                if (curX > L || curY > L)
                {
                    flag = 1;
                    break;
                }
                if (B[j][k] == 1)
                {
                    int m = 0;
                    for (m = 0; m < n; m++)
                    {
                        //查找到符合条件的点，直接进行下轮判断
                        if (curX == A[m][0] && curY == A[m][1])
                            break;
                    }
                    if (m == n)
                        flag = 1;
                }
                else
                {
                    for (int m = 0; m < n; m++)
                    {
                        //查找到不符合条件的点，该藏宝图位置错误，退出整个循环
                        if (curX == A[m][0] && curY == A[m][1])
                        {
                            flag = 1;
                            break;
                        }
                    }
                }
            }
            if (flag == 1)
                break;
        }
        if (flag == 0)
            ans++;
    }
    cout << ans;
}