#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <cstdio>
#include <string.h>
using namespace std;

int lef[100010] = {1,0};

int main()
{
    int n, k, ans = 0;
    cin >> n >> k;
    while (k--)
    {
        int x, y;
        cin >> x >> y;
        if (!lef[y])
            ans++;
        lef[x] = 1;
    }
    cout << ans; 
}
