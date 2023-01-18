#include <iostream>
#include <algorithm>
using namespace std;

typedef struct Node
{
    int y;
    int result;
} Node;

int m = 0;
int dif[100000000] = {0};
Node arr[100001] = {{0, 0}};
bool cmp(Node n1, Node n2)
{
    return n1.y < n2.y;
}

int main()
{
    cin >> m;
    for (int i = 0; i < m; i++)
    {
        cin >> arr[i].y >> arr[i].result;
        if (arr[i].result == 1)
            dif[0]++;
    }

    sort(arr, arr + m, cmp);
    int count = 0;
    for (int i = 1; i < m; i++)
    {
        if (arr[i - 1].result == 0)
            count++;
        else
            count--;

        if (arr[i - 1].y != arr[i].y)
        {
            dif[arr[i].y] += count;
            count = 0;
        }
    }
    int pre = 0;
    pair<int, int> max(0, 0);
    for (int i = 0; i < m; i++)
    {
        pre += dif[arr[i].y];
        if (max.second <= pre)
        {
            max.first = arr[i].y;
            max.second = pre;
        }
    }
    cout << max.first;
}
