/*4. CSP题目
题目描述：
顿顿评估了m位同学上学期的安全指数，其中第i（1≤i≤m）位同学的安全指数为yi，
是一个[0, 108] 范围内的整数；同时，该同学上学期的挂科情况记作resulti∈0,1，
其中0表示挂科、1表示未挂科。 
相应地，顿顿用predictθ(y) 表示根据阈值θ将安全指数y转化为的具体预测结果。 
如果predictθ(yj) 与resultj相同，则说明阈值为θ时顿顿对第j位同学是否挂科预测正确；
不同则说明预测错误。
最后，顿顿设计了如下公式来计算最佳阈值θ∗：

该公式亦可等价地表述为如下规则：
1. 最佳阈值仅在yi中选取，即与某位同学的安全指数相同；
2. 按照该阈值对这m位同学上学期的挂科情况进行预测，预测正确的次数最多（即准确率最高）；
3. 多个阈值均可以达到最高准确率时，选取其中最大的。 输入格式：
从标准输入读入数据。 输入的第一行包含一个正整数m。 
接下来输入m行，其中第i（1≤i≤m）行包括用空格分隔的两个整数yi和resulti，含义如上文所述。 输出格式：
输出到标准输出。 输出一个整数，表示最佳阈值θ∗。
*/

#include <iostream>
#include <algorithm>
using namespace std;

typedef struct Node
{
    int y;
    int result;
    int count;
} Node;

bool cmp(Node n1, Node n2)
{
    return n1.y < n2.y;
}

int main()
{
    int m = 0;
    int sum[100001] = {0};
    int count[100001] = {0};
    Node arr[100001] = {{0, 0, 0}};

    cin >> m;
    for (int i = 1; i <= m; i++)
        cin >> arr[i].y >> arr[i].result;

    sort(arr + 1, arr + m + 1, cmp);
    int pre_y = arr[1].y;

    sum[1] = arr[1].result;

    for (int i = 1; i < m; i++)
        sum[i + 1] = sum[i] + arr[i + 1].result;
    //统计及格人数前缀和

    arr[1].count = sum[m];
    Node max = arr[1];

    for (int i = 2; i <= m; i++)
    {
        if (arr[i].y == pre_y)
            continue;
        else
        {
            arr[i].count = (i - 1 - sum[i - 1]) /*当前阈值前不及格的个数*/
                           + (sum[m] - sum[i - 1]) /*当前阈值后及格的个数*/;

            if (arr[i].count >= max.count && arr[i].y > max.y)
                max = arr[i];

            pre_y = arr[i].y;
        }
    }

    cout << max.y << endl;

    return 0;
}