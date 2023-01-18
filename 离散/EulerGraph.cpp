#include <iostream>
#include <stack>
#include <cstdlib>
#include <queue>
using namespace std;

#define MAX_V 30

enum GraphType
{
    EMPTY = -1,
    UNDIGRAPH = 0,
    DIGRAPH = 1,
};
typedef int ElemType;

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
        cout << "输入结点数与边数：" << endl;
        cin >> G.vexNum >> G.arcNum;

        cout << "输入结点内容：" << endl;
        for (int i = 0; i < G.vexNum; i++)
            cin >> G.vexs[i];

        for (int i = 0; i < G.vexNum; i++)
            for (int j = 0; j < G.vexNum; j++)
                G.adjMatrix[i][j] = 0;

        cout << "输入邻接矩阵：" << endl;
        for (int k = 0; k < G.arcNum; k++)
        {
            int i, j;
            cin >> i >> j;
            G.adjMatrix[i][j] = 1;
            if (G.type == UNDIGRAPH)
                G.adjMatrix[j][i] = 1;
        }
    }
}

bool judgeLinkedGraph(Graph G)
{
    if (G.type != EMPTY)
    {
        queue<int> Q;
        bool visit[MAX_V];
        //初始化标记数组为未访问
        for (int i = 0; i < G.vexNum; i++)
            visit[i] = false;

        Q.push(0);
        visit[0] = true;
        while (!Q.empty())
        {
            int curFront = Q.front();
            Q.pop();

            for (int j = 0; j < G.vexNum; j++)
            {
                if (G.adjMatrix[curFront][j] > 0)
                {
                    if (visit[j] == false)
                    {
                        visit[j] = true;
                        Q.push(j);
                    }
                }
            }
        }

        for (int i = 0; i < G.vexNum; i++)
        {
            if (visit[i] == false)
                return false;
        }
        return true;
    }
}

bool judgeEulerGraph(Graph G)
{
    if (!judgeLinkedGraph(G))
        return false;
    if (G.type != EMPTY)
    {
        if (G.type == UNDIGRAPH)
        {
            int degree[MAX_V] = {0};
            int num = 0;
            for (int i = 0; i < G.vexNum; i++)
            {
                for (int j = num++; j < G.vexNum; j++)
                {
                    if (G.adjMatrix[i][j] == 1)
                    {
                        degree[i]++;
                        degree[j]++;
                    }
                }
            }
            for (int i = 0; i < G.vexNum; i++)
            {
                if (degree[i] % 2 != 0)
                    return false;
            }
            return true;
        }
        else
        {
            int inDegree[MAX_V] = {0}, outDegree[MAX_V] = {0};
            for (int i = 0; i < G.vexNum; i++)
            {
                for (int j = 0; j < G.vexNum; j++)
                {
                    if (G.adjMatrix[i][j] == 1)
                    {
                        inDegree[i]++;
                        outDegree[j]++;
                    }
                }
            }
            for (int i = 0; i < G.vexNum; i++)
            {
                if (inDegree[i] != outDegree[i])
                    return false;
            }
            return true;
        }
    }
}

bool judgeEulerRoad(Graph G)
{
    if (!judgeLinkedGraph(G))
        return false;
    if (G.type != EMPTY)
    {
        if (G.type == UNDIGRAPH)
        {
            int degree[MAX_V] = {0};
            int num = 0;
            for (int i = 0; i < G.vexNum; i++)
            {
                for (int j = num++; j < G.vexNum; j++)
                {
                    if (G.adjMatrix[i][j] == 1)
                    {
                        degree[i]++;
                        degree[j]++;
                    }
                }
            }
            int count = 0;
            for (int i = 0; i < G.vexNum; i++)
            {
                if (degree[i] % 2 != 0)
                    count++;
            }
            if (count == 0 || count == 2)
                return true;
            else
                return false;
        }
        else
        {
            int inDegree[MAX_V] = {0}, outDegree[MAX_V] = {0};
            for (int i = 0; i < G.vexNum; i++)
            {
                for (int j = 0; j < G.vexNum; j++)
                {
                    if (G.adjMatrix[i][j] == 1)
                    {
                        inDegree[i]++;
                        outDegree[j]++;
                    }
                }
            }
            int s = -1, t = -1, count = 0;
            for (int i = 0; i < G.vexNum; i++)
            {
                if (inDegree[i] != outDegree[i])
                {
                    if (s == -1)
                        s = i;
                    if (s != -1)
                        t = i;
                    count++;
                }
            }
            if (count == 0)
                return true;
            else if (count > 2 || count == 1)
                return false;
            else if ((inDegree[s] - outDegree[s] == 1 && inDegree[t] - outDegree[t] == -1) || (inDegree[s] - outDegree[s] == -1 && inDegree[t] - outDegree[t] == 1))
                return true;
        }
    }
}

void EulerCircle(Graph G, stack<int> &stk, bool visit[][MAX_V], int beg)
{
    if (!judgeEulerGraph(G))
    {
        cout << "Don't have Euler Circle!" << endl;
        return;
    }

    if (G.type != EMPTY)
    {
        stk.push(beg);
        //初始化标记数组为访问
        int flag = 0;
        int curTop = 0;
        int j = 0;
        while (j != G.vexNum)
        {
            curTop = stk.top();

            for (j = 0; j < G.vexNum; j++)
            {
                if (visit[curTop][j] == false)
                {
                    if (G.adjMatrix[curTop][j] > 0)
                    {
                        stk.push(j);
                        visit[curTop][j] = true;
                        if (G.type == UNDIGRAPH)
                            visit[j][curTop] = true;
                        break;
                    }
                }
            }
        }

        int i = 0, count = 0;
        for (i = 0; i < G.vexNum; i++)
        {
            for (int k = 0; k < G.vexNum; k++)
            {
                if (visit[i][k] == false)
                    count++;
            }
            if (count == G.vexNum)
            {
                flag = 1;
                break;
            }
            count = 0;
        }

        if (flag == 1)
        {
            if (G.type == DIGRAPH)
            {
                int flag3 = 0;
                while (flag3 != 1)
                {
                    for (int k = 0; k < i; k++)
                    {
                        if (G.adjMatrix[k][i] > 0)
                        {
                            flag3 = 1;
                            break;
                        }
                        else
                            flag3 = 2;
                    }
                    if (flag3 != 1)
                        i++;
                }
            }

            int flag2 = 0, last = 0;
            while (!flag2)
            {
                curTop = stk.top();
                visit[curTop][last] = false;
                if (G.type == UNDIGRAPH)
                    visit[last][curTop] = false;
                stk.pop();
                last = curTop;

                for (int k = 0; k < G.vexNum; k++)
                {
                    if (G.adjMatrix[curTop][i] > 0)
                    {
                        flag2 = 1;
                        stk.push(curTop);
                        visit[curTop][i] = true;
                        if (G.type == UNDIGRAPH)
                            visit[i][curTop] = true;
                        j = 0;
                        break;
                    }
                }
            }
            EulerCircle(G, stk, visit, i);
        }
    }
}

void showEulerCircle(Graph G)
{
    stack<int> stk;
    bool visit[MAX_V][MAX_V];
    for (int i = 0; i < G.vexNum; i++)
        for (int j = 0; j < G.vexNum; j++)
            visit[i][j] = false;
    EulerCircle(G, stk, visit, 0);
    stack<int> out;
    while (!stk.empty())
    {
        int cur = stk.top();
        out.push(cur);
        stk.pop();
    }
    while (!out.empty())
    {
        int cur = out.top();
        cout << G.vexs[cur] << " ";
        out.pop();
    }
}

int main()
{
    Graph G;
    createGraph(G, UNDIGRAPH);
    cout << judgeLinkedGraph(G) << endl;
    cout << judgeEulerRoad(G) << endl;
    cout << judgeEulerGraph(G) << endl;
    showEulerCircle(G);
}