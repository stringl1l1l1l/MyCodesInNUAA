#include <bits/stdc++.h>
using namespace std;

map<int, set<int>> area_node;
map<int, set<int>> app_node;
int main()
{
    int n, m, g;
    cin >> n >> m;
    for (int i = 1; i <= n; i++)
    {
        int num;
        cin >> num;
        area_node[num].insert(i);
    }
    cin >> g;
    int fi, ai, nai, pai, paai, paari;
    for (int i = 0; i < g; i++)
    {
        cin >> fi >> ai >> nai >> pai >> paai >> paari;
    }
}