/*问题描述：
待处理的灰度图像长宽皆为n个像素，可以表示为一个n×n大小的矩阵A，
其中每个元素是一个[0,L)范围内的整数，表示对应位置像素的灰度值。
对于矩阵中任意一个元素Aij（0≤i,j<n），其邻域定义为附近若干元素的集和：

这里使用了一个额外的参数r来指明Aij附近元素的具体范围。根据定义，
易知Neighbor(i,j,r)最多有(2r+1)^2个元素。
如果元素Aij邻域中所有元素的平均值小于或等于一个给定的阈值t，
我们就认为该元素对应位置的像素处于较暗区域。
下图给出了两个例子，左侧图像的较暗区域在右侧图像中展示为黑色，其余区域展示为白色。
输出一个整数，表示输入灰度图像中处于较暗区域的像素总数。
输入格式：
输入共 n+1 行。
输入的第一行包含四个用空格分隔的正整数 n、L、r 和 t，含义如前文所述。
第二到第 n+1 行输入矩阵 A。
第 i+2（0≤i<n）行包含用空格分隔的 n 个整数，依次为 Ai0,Ai1,⋯,Ai(n−1)。
输出格式：
输出一个整数，表示输入灰度图像中处于较暗区域的像素总数。
样例输入：
4 16 1 6
0 1 2 3
4 5 6 7
8 9 10 11
12 13 14 15
样例输出：
7
样例输入：
11 8 2 2
0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0
0 7 0 0 0 7 0 0 7 7 0
7 0 7 0 7 0 7 0 7 0 7
7 0 0 0 7 0 0 0 7 0 7
7 0 0 0 0 7 0 0 7 7 0
7 0 0 0 0 0 7 0 7 0 0
7 0 7 0 7 0 7 0 7 0 0
0 7 0 0 0 7 0 0 7 0 0
0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0
样例输出：
83
评测用例规模与约定：
70% 的测试数据满足 n≤100、r≤10。
全部的测试数据满足 0<n≤600、0<r≤100 且 2≤t<L≤256。
*/
#include <iostream>
#include <math.h>
using namespace std;
int main()
{
    int n, L, r, t;
    cin >> n >> L >> r >> t;
    int matrix[100][100];

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> matrix[i][j];

    int sum[100][100];
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
            sum[i][j] = 0;
    }

    for (int i = 0; i < n; i++) 
    {
        for (int j = 0; j < n; j++)
        {
            if (i == 0 && j == 0)
                sum[i][j] = matrix[0][0];
            else if (i == 0 && j >= 1)
                sum[i][j] = sum[0][j - 1] + matrix[0][j];
            else if (j == 0 && i >= 1)
                sum[i][j] = sum[i - 1][0] + matrix[i][0];
            else
                sum[i][j] = sum[i - 1][j] + sum[i][j - 1] + matrix[i][j] - sum[i - 1][j - 1];
        }
    }
    //二维数组前缀和

    int result = 0;
    float avg = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int row1 = i - r - 1,
                col1 = j - r - 1,
                row2 = min(i + r, n - 1),
                col2 = min(j + r, n - 1);

            if (row1 < -1 && col1 < -1)
                avg = sum[row2][col2] * 1.0 / ((row2 + 1) * (col2 + 1));
            else if (row1 < -1)
                avg = (sum[row2][col2] - sum[row2][col1] * 1.0) / ((row2 + 1) * (col2 - col1));
            else if (col1 < -1)
                avg = (sum[row2][col2] - sum[row1][col2] * 1.0) / ((row2 - row1) * (col2 + 1));
            else
                avg = (sum[row2][col2] - sum[row1][col2] - sum[row2][col1] + sum[row1][col1]) * 1.0 / ((row2 - row1) * (col2 - col1));

            if (avg <= t)
                result++;
        }
    }

    cout << result;
}