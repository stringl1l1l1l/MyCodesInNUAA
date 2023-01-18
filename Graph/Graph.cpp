#include <iostream>
#include <stack>
#include <cstdlib>
#include <queue>
using namespace std;

#define INFINITY 2000000000
#define MAX_V 20

enum GraphType
{
    EMPTY = -1,
    UNDIGRAPH = 0,
    DIGRAPH = 1,
    UNDINET = 2,
    DINET = 3
};
typedef char ElemType;

typedef struct Vertix
{
    int code;
    ElemType info;
} Vertix;

typedef struct Graph
{
    int adjMatrix[MAX_V][MAX_V];
    int vexNum, arcNum;
    ElemType vexs[MAX_V];
    GraphType type;
} Graph;

void createGraph(Graph &G, GraphType type)
{
    G.type = type;

    if (G.type != EMPTY)
    {
        //cout << "输入结点数与度数：" << endl;
        cin >> G.vexNum >> G.arcNum;

        //cout << "输入结点内容：" << endl;
        for (int i = 0; i < G.vexNum; i++)
            cin >> G.vexs[i];

        if (G.type == DINET || G.type == UNDINET)
        {
            for (int i = 0; i < G.vexNum; i++)
                for (int j = 0; j < G.vexNum; j++)
                    G.adjMatrix[i][j] = INFINITY;
        }
        else
        {
            for (int i = 0; i < G.vexNum; i++)
                for (int j = 0; j < G.vexNum; j++)
                    G.adjMatrix[i][j] = 0;
        }

        //cout << "输入邻接矩阵：" << endl;
        if (G.type == UNDIGRAPH || G.type == DIGRAPH)
        {
            for (int k = 0; k < G.arcNum; k++)
            {
                int i, j;
                cin >> i >> j;
                G.adjMatrix[i][j] = 1;
                if (G.type == UNDIGRAPH)
                    G.adjMatrix[j][i] = 1;
            }
        }
        else
        {
            for (int k = 0; k < G.arcNum; k++)
            {
                int i, j, weight;
                cin >> i >> j >> weight;
                G.adjMatrix[i][j] = weight;
                if (G.type == UNDINET)
                    G.adjMatrix[j][i] = weight;
            }
        }
    }
}

void destroyGraph(Graph &G)
{
    G.arcNum = 0;
    G.vexNum = 0;
    G.type = EMPTY;
    free(G.vexs);
    free(G.adjMatrix);
}

ElemType getVertix(Graph &G, int code)
{
    return G.vexs[code];
}

int getFirstAdjVex(Graph &G, int vex)
{
    if (G.type != EMPTY)
    {
        for (int j = 0; j < G.vexNum; j++)
        {
            if (G.adjMatrix[vex][j] > 0 && G.adjMatrix[vex][j] < INFINITY)
                return j;
        }
    }
    return -1;
}

int getNextAdjVex(Graph &G, int vex, int adjVex)
{
    if (G.type != EMPTY)
    {
        for (int j = 0; j < G.vexNum; j++)
        {
            if (j != adjVex && G.adjMatrix[vex][j] > 0 && G.adjMatrix[vex][j] < INFINITY)
                return j;
        }
    }
    return -1;
}

void DFSTraverse(Graph G)
{
    if (G.type != EMPTY)
    {
        stack<int> stk;
        bool visit[MAX_V];

        //初始化标记数组为未访问
        for (int i = 0; i < G.vexNum; i++)
            visit[i] = false;

        int i = 0;
        while (i != G.vexNum)
        {
            for (i = 0; i < G.vexNum; i++)
            {
                if (visit[i] == false)
                {
                    stk.push(i);
                    visit[i] = true;
                    break;
                }
            }
            while (!stk.empty())
            {
                int curTop = stk.top();
                cout << G.vexs[curTop] << " ";
                stk.pop();
                visit[curTop] = true;

                int j = 0;
                for (j = 0; j < G.vexNum; j++)
                {
                    if (visit[j] == false)
                    {
                        if (G.adjMatrix[curTop][j] > 0 && G.adjMatrix[curTop][j] < INFINITY)
                        {
                            stk.push(j);
                            break;
                        }
                    }
                }
            }
        }
        cout << endl;
    }
}

void BFSTraverse(Graph G)
{
    if (G.type != EMPTY)
    {
        queue<int> Q;
        bool visit[MAX_V];
        //初始化标记数组为未访问
        for (int i = 0; i < G.vexNum; i++)
            visit[i] = false;

        int i = 0;
        while (i != G.vexNum)
        {
            for (i = 0; i < G.vexNum; i++)
            {
                if (visit[i] == false)
                {
                    Q.push(i);
                    visit[i] = true;
                    break;
                }
            }

            while (!Q.empty())
            {
                int curFront = Q.front();
                cout << G.vexs[curFront] << " ";
                Q.pop();

                for (int j = 0; j < G.vexNum; j++)
                {
                    if (G.adjMatrix[curFront][j] > 0 && G.adjMatrix[curFront][j] < INFINITY)
                    {
                        if (visit[j] == false)
                        {
                            visit[j] = true;
                            Q.push(j);
                        }
                    }
                }
            }
        }
        cout << endl;
    }
}

void insertVex(Graph &G, Vertix v)
{
    G.vexs[G.vexNum] = v.info;
    G.vexNum++;
}

void insertArc(Graph &G, Vertix v, Vertix w)
{
    if (G.type != EMPTY)
    {
        if (G.type == DIGRAPH)
            G.adjMatrix[v.code][w.code] = 1;
        else if (G.type == UNDIGRAPH)
        {
            G.adjMatrix[v.code][w.code] = 1;
            G.adjMatrix[w.code][v.code] = 1;
        }
    }
}

void deleteVex(Graph &G, Vertix v)
{
    if (G.type != EMPTY)
    {
        int count = 0;
        //删除所有边

        for (int i = 0; i < G.vexNum; i++)
        {
            if (G.adjMatrix[v.code][i] > 0 && G.adjMatrix[v.code][i] < INFINITY)
            {
                if (G.type == UNDIGRAPH || G.type == DIGRAPH)
                    G.adjMatrix[v.code][i] = 0;
                else if (G.type == UNDINET || G.type == DINET)
                    G.adjMatrix[v.code][i] = INFINITY;
                count++;
            }
        }

        for (int i = 0; i < G.vexNum; i++)
        {
            if (G.adjMatrix[i][v.code] > 0 && G.adjMatrix[i][v.code] < INFINITY)
            {
                if (G.type == UNDIGRAPH || G.type == DIGRAPH)
                    G.adjMatrix[v.code][i] = 0;
                else if (G.type == UNDINET || G.type == DINET)
                    G.adjMatrix[v.code][i] = INFINITY;
                if (G.type == DIGRAPH || G.type == DINET)
                    count++;
            }
        }
        G.arcNum -= count;

        //删除顶点数组中的被删除顶点
        for (int i = v.code; i < G.vexNum - 1; i++)
            G.vexs[i] = G.vexs[i + 1];
        //删除邻接矩阵中所有空行
        for (int i = v.code; i < G.vexNum - 1; i++)
            for (int j = 0; j < G.vexNum; j++)
                G.adjMatrix[i][j] = G.adjMatrix[i + 1][j];

        for (int i = v.code; i < G.vexNum - 1; i++)
            for (int j = 0; j < G.vexNum; j++)
                G.adjMatrix[j][i] = G.adjMatrix[j][i + 1];
        G.vexNum--;
    }
}

void deleteArc(Graph &G, Vertix v, Vertix w)
{
    if (G.type != EMPTY)
    {
        if (G.type == UNDIGRAPH || G.type == DIGRAPH)
            G.adjMatrix[v.code][w.code] = 0;
        else if (G.type == UNDINET || G.type == DINET)
            G.adjMatrix[v.code][w.code] = INFINITY;

        if (G.type == UNDIGRAPH)
            G.adjMatrix[w.code][v.code] = 0;
        if (G.type == UNDIGRAPH)
            G.adjMatrix[w.code][v.code] = INFINITY;
        G.arcNum--;
    }
}

stack<int> findEarly(Graph &G, int early[])
{
    queue<int> eQue;
    stack<int> lStk;
    bool visited[MAX_V] = {false};
    int indegree[MAX_V] = {0};
    for (int i = 0; i < G.vexNum; i++)
        for (int j = 0; j < G.vexNum; j++)
            if (G.adjMatrix[j][i] > 0 && G.adjMatrix[j][i] < INFINITY)
                indegree[i]++;
    //按拓扑排序顺序
    //所有入度为0的点入队
    while (true)
    {
        int flag = 0;
        for (int i = 0; i < G.vexNum; i++)
        {
            if (indegree[i] == 0 && visited[i] == false)
            {
                eQue.push(i);
                visited[i] = true;
                flag = 1;
            }
        }
        //计算队中所有点最早时间
        if (flag == 0)
            return lStk;
        //所有结点都访问过则结束循环
        //删除入队点的出度
        while (!eQue.empty())
        {
            int vMax = 0;
            int curFront = eQue.front();
            for (int i = 0; i < G.vexNum; i++)
            {
                //找出到达队头最长的路径
                if (G.adjMatrix[i][curFront] > 0 && G.adjMatrix[i][curFront] < INFINITY)
                {
                    if (vMax < early[i] + G.adjMatrix[i][curFront])
                        vMax = early[i] + G.adjMatrix[i][curFront];
                }
            }
            early[curFront] = vMax;
            eQue.pop();
            lStk.push(curFront);
            //
            for (int i = 0; i < G.vexNum; i++)
                if (G.adjMatrix[curFront][i] > 0 && G.adjMatrix[curFront][i] < INFINITY)
                    indegree[i]--;
            //弹栈后删除对应结点提供的入度
        }
    }
}

void findLate(Graph G, stack<int> lStk, int early[], int late[])
{
    int lenth = 0;
    for (int i = 0; i < G.vexNum; i++)
        if (lenth < early[i])
            lenth = early[i];

    while (!lStk.empty())
    {
        int max = 0;
        int curTop = lStk.top();
        for (int i = 0; i < G.vexNum; i++)
        {
            //计算当前结点到达终点的最长路径
            if (G.adjMatrix[curTop][i] > 0 && G.adjMatrix[curTop][i] < INFINITY)
            {
                if (max < G.adjMatrix[curTop][i] + lenth - late[i])
                    max = G.adjMatrix[curTop][i] + lenth - late[i];
            }
        }
        late[curTop] = lenth - max; //路径长度减去当前结点到终点的最长路径
        lStk.pop();
    }
}

void showCriticalRoad(Graph G)
{
    if (G.type == DINET)
    {
        stack<int> lStk;
        int early[MAX_V] = {0}, late[MAX_V] = {0};

        lStk = findEarly(G, early);
        findLate(G, lStk, early, late);
        for (int i = 0; i < G.vexNum; i++)
        {
            if (early[i] == late[i])
                cout << G.vexs[i] << " ";
        }
    }
}

void Dijkstra(Graph G, int beg)
{
    if (G.type != EMPTY)
    {
        int dist[MAX_V] = {0}, path[MAX_V] = {0};
        bool flag[MAX_V] = {false};
        for (int i = 0; i < G.vexNum; i++)
        {
            flag[i] = false;
            dist[i] = G.adjMatrix[beg][i];
            if (dist[i] < INFINITY)
                path[i] = beg;
            else
                path[i] = -1;
        }

        dist[beg] = 0;
        flag[beg] = true; //起始点标记为已访问
        for (int i = 0; i < G.vexNum; i++)
        {
            int minDist = INFINITY, index = MAX_V; //最短距离与对应的索引
            for (int k = 0; k < G.vexNum; k++)
            {
                if (flag[k] == false && minDist > dist[k])
                {
                    minDist = dist[k];
                    index = k;
                }
            }
            flag[index] = true; //标记最短距离对应点为访问
            if (minDist < INFINITY)
            {
                for (int j = 0; j < G.vexNum; j++)
                {
                    //判断未访问的每个顶点经过新的中转点后距起点是否更短
                    //INFINITY太大时这里会溢出
                    if (flag[j] == false && dist[j] > G.adjMatrix[index][j] + dist[index])
                    {
                        dist[j] = G.adjMatrix[index][j] + dist[index];
                        path[j] = index;
                    }
                }
            }
        }

        //输出最短路径
        for (int i = 0; i < G.vexNum; i++)
        {
            if (i != beg)
            {
                int count = 1, k = i;
                int out[MAX_V] = {0};
                out[0] = i;
                while (path[k] != beg)
                {
                    k = path[k];
                    out[count++] = k;
                }
                out[count] = beg;
                cout << beg << "~" << i << "  distance: " << dist[i] << ", road: ";
                for (int j = count; j >= 0; j--)
                {
                    cout << out[j];
                    if (j != 0)
                        cout << "->";
                }
                cout << endl;
            }
        }
    }
}

int prim(Graph &G)
{
    int vexCode[MAX_V] = {0}, lowCost[MAX_V] = {0};
    bool finish[MAX_V] = {false};
    for (int i = 0; i < MAX_V; i++)
    {
        vexCode[i] = 0;
        lowCost[i] = INFINITY;
    }
    for (int i = 0; i < G.vexNum; i++)
    {
        vexCode[i] = 0;
        lowCost[i] = G.adjMatrix[0][i];
    }
    finish[0] = true;

    for (int i = 0; i < G.vexNum; i++)
    {
        int min = INFINITY, index = MAX_V;
        for (int k = 0; k < G.vexNum; k++)
        {
            if (finish[k] == false && min > lowCost[k])
            {
                min = lowCost[k];
                index = k;
            }
        }
        finish[index] = true;

        for (int j = 0; j < G.vexNum; j++)
        {
            if (finish[j] == false && lowCost[j] > G.adjMatrix[j][index])
            {
                lowCost[j] = G.adjMatrix[j][index];
                vexCode[j] = index;
            }
        }
    }
    int sum = 0;
    for (int i = 0; i < G.vexNum; i++)
        sum += lowCost[i];
    return sum;
}

int main()
{
    Graph G;
    createGraph(G, DINET);
    Dijkstra(G, 0);
}