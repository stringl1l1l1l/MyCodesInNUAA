#include <bits/stdc++.h>
using namespace std;
const int MAXN = 1e3;

struct Position
{
    int x, y;
    Position() {}
    Position(int x, int y)
        : x(x), y(y) {}
    bool operator==(const Position &p)
    {
        return p.x == this->x && p.y == this->y;
    }
};

struct hash_functor
{
    bool operator()(const Position &p1, const Position &p2)
    {
        if (p1.x != p2.x)
            return p1.x < p2.x;
        else
            return p1.y < p2.y;
    }
};
vector<Position> positions;
map<Position, int, hash_functor> posMap;

int main()
{
    ios::sync_with_stdio(false), cin.tie(0);
    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        int x, y;
        cin >> x >> y;
        Position pos(x, y);
        positions.push_back(pos);
        posMap[pos] = -1;
    }
    for (auto &pos : positions)
    {
        int x = pos.x, y = pos.y;
        Position p1(x - 1, y), p2(x + 1, y), p3(x, y - 1), p4(x, y + 1),
            p5(x - 1, y - 1), p6(x - 1, y + 1), p7(x + 1, y + 1), p8(x + 1, y - 1);
        if (posMap.find(p1) == posMap.end())
            continue;
        if (posMap.find(p2) == posMap.end())
            continue;
        if (posMap.find(p3) == posMap.end())
            continue;
        if (posMap.find(p4) == posMap.end())
            continue;
        posMap[pos] = 0;
        if (posMap.find(p5) != posMap.end())
            posMap[pos]++;
        if (posMap.find(p6) != posMap.end())
            posMap[pos]++;
        if (posMap.find(p7) != posMap.end())
            posMap[pos]++;
        if (posMap.find(p8) != posMap.end())
            posMap[pos]++;
    }
    int ans[5] = {0};
    for (auto &mem : posMap)
    {
        if (mem.second >= 0)
            ans[mem.second]++;
    }

    for (int i = 0; i < 5; i++)
    {
        cout << ans[i];
        if (i != 4)
            cout << endl;
    }
}