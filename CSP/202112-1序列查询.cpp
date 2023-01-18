#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <cstdio>
#include <string.h>
using namespace std;

int main()
{
    int n, N, ans = 0;
    int arr[210] = {0};
    cin >> n >> N;
    for (int i = 1; i <= n; i++)
    {
        int temp;
        cin >> temp;
        arr[i] = temp;
    }
    for (int i = 0; i < N; i++)
    {
        int f = 0;
        f = upper_bound(arr, arr + n + 1, i) - arr - 1;
        ans += f;
    }
    cout << ans;
}
