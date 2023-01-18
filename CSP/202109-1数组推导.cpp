#include <iostream>
#include <set>
#include <math.h>
#include <cstdio>
#include <string.h>
using namespace std;

int main()
{
    int arr[101] = {0};
    int n = 0;
    int maxSum = 0, minSum = 0;
    set<int> setArr;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        cin >> arr[i];
        maxSum += arr[i];
        setArr.insert(arr[i]);
    }
    int minCount = n - setArr.size();
    for (auto it = setArr.begin(); it != setArr.end(); it++)
        minSum += *it;
    cout << maxSum << "\n"
         << minSum;
}