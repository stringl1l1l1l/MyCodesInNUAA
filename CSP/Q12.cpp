/*问题描述：小刘承包了很多片麦田，为了灌溉这些麦田，
小刘在第一个麦田挖了一口很深的水井，所有的麦田都从这口井来引水灌溉。
为了灌溉，小刘需要建立一些水渠，以连接水井和麦田，小刘也可以利用部分麦田作为“中转站”，
利用水渠连接不同的麦田，这样只要一片麦田能被灌溉，则与其连接的麦田也能被灌溉。
现在小刘知道哪些麦田之间可以建设水渠和建设每个水渠所需要的费用（注意不是所有麦田之间都可以建立水渠）。
请问灌溉所有麦田最少需要多少费用来修建水渠。

输入格式：输入的第一行包含两个正整数n, m，分别表示麦田的片数和小刘可以建立的水渠的数量。
麦田使用1, 2, 3, ……依次标号。
接下来m行，每行包含三个整数ai, bi, ci，表示第ai片麦田与第bi片麦田之间可以建立一条水渠，所需要的费用为ci。
输出格式：输出一个整数，表示灌溉所有麦田所需要的最小费用，及水渠连接说明。
问题分析：这个问题可以用最小生成树算法实现。
输入样例:
4 4
1 2 1
2 3 4
2 4 2
3 4 3
输出样例:
6
建立以下3条水渠：麦田1与麦田2、麦田2与麦田4、麦田4与麦田3。
*/
#include <iostream>
using namespace std;
#define INFINITY 2000000000
#define MAX_V 1005

typedef struct Vertix
{
    int code;
    int info;
} Vertix;

typedef struct Graph
{
    int adjMatrix[MAX_V][MAX_V];
    int vexNum, arcNum;
    int vexs[MAX_V];
} Graph;

int prim(Graph &G)
{
    int vexCode[MAX_V] = {0}, lowCost[MAX_V] = {0};
    bool finish[MAX_V] = {false};
    for (int i = 0; i < MAX_V; i++)
    {
        vexCode[i] = 1;
        lowCost[i] = INFINITY;
    }
    for (int i = 1; i <= G.vexNum; i++)
    {
        vexCode[i] = 1;
        lowCost[i] = G.adjMatrix[1][i];
    }
    finish[1] = true;

    for (int i = 1; i <= G.vexNum; i++)
    {
        int min = INFINITY, index = MAX_V;
        for (int k = 1; k <= G.vexNum; k++)
        {
            if (finish[k] == false && min > lowCost[k])
            {
                min = lowCost[k];
                index = k;
            }
        }
        finish[index] = true;

        for (int j = 1; j <= G.vexNum; j++)
        {
            if (finish[j] == false && lowCost[j] > G.adjMatrix[j][index])
            {
                lowCost[j] = G.adjMatrix[j][index];
                vexCode[j] = index;
            }
        }
    }
    int sum = 0;
    for (int i = 1; i <= G.vexNum; i++)
        sum += lowCost[i];
    return sum;
}

int a[100005][3];
Graph G;
int main()
{
    int n, m; //麦田片数和水渠数
    cin >> n >> m;

    for (int i = 0; i < MAX_V; i++)
    {
        for (int j = 0; j < MAX_V; j++)
        {
            if (i == j)
                G.adjMatrix[i][j] = 0;
            else
                G.adjMatrix[i][j] = INFINITY;
        }
    }
    G.vexNum = n;
    G.arcNum = m;

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < 3; j++)
            cin >> a[i][j];
    }

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            G.adjMatrix[a[i][0]][a[i][1]] = a[i][2];
            G.adjMatrix[a[i][1]][a[i][0]] = a[i][2];
        }
    }
    cout << prim(G) << endl;
}