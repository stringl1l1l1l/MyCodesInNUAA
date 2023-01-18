#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include <cstdlib>
#include <queue>
using namespace std;

#define INFINITY 2000000000
#define FILE_NAME "D:\\Programming\\GitHub\\repository\\DataStruct\\CourseDesign\\.txt\\minSpanningTree.txt"

enum GraphType
{
    UNDINET = 0,
    DINET = 1
};

struct Vertix
{
    int code;
    string info;
};

struct Edge
{
    int leftEnd, rightEnd;
    float weigh;
};

struct Graph
{
    float **adjMatrix;
    int vexNum, arcNum;
    string *vexs;
};

void createGraph(Graph &G, GraphType type)
{
    fstream file(FILE_NAME, ios::in);
    if (file.fail())
    {
        cout << "Cannot open file!" << endl;
        exit(-1);
    }

    string line;
    for (int i = 0; !file.eof(); i++)
    {
        getline(file, line);
        stringstream sstream(line);
        if (i > 1)
        {
            int row, col;
            sstream >> row >> col;
            sstream >> G.adjMatrix[row][col];
            G.adjMatrix[col][row] = G.adjMatrix[row][col];
            G.arcNum++;
        }
        else if (i == 0)
        {
            G.arcNum = 0;
            sstream >> G.vexNum;
            G.adjMatrix = new float *[G.vexNum];
            for (int j = 0; j < G.vexNum; j++)
            {
                G.adjMatrix[j] = new float[G.vexNum];
                for (int k = 0; k < G.vexNum; k++)
                    G.adjMatrix[j][k] = INFINITY;
            }
        }
        else if (i == 1)
        {
            G.vexs = new string[G.vexNum];
            int j = 0;
            while (sstream >> G.vexs[j])
                j++;
        }
    }
    file.close();
}

void prim(Graph G)
{
    int *vexCode = new int[G.vexNum];
    float *lowCost = new float[G.vexNum];
    bool *finish = new bool[G.vexNum];

    for (int i = 0; i < G.vexNum; i++)
    {
        vexCode[i] = 0;
        lowCost[i] = G.adjMatrix[0][i];
        finish[i] = false;
    }
    finish[0] = true;
    //初始化
    //从编号为0的顶点开始查找最小生成树
    for (int i = 0; i < G.vexNum; i++)
    {
        //
        float min = INFINITY;
        int index = G.vexNum;
        for (int k = 0; k < G.vexNum; k++)
        {
            if (finish[k] == false && min > lowCost[k])
            {
                min = lowCost[k];
                index = k;
            }
        }
        finish[index] = true;
        //找出到达顶点集距离最短的顶点
        //
        for (int j = 0; j < G.vexNum; j++)
        {
            if (finish[j] == false && lowCost[j] > G.adjMatrix[j][index])
            {
                lowCost[j] = G.adjMatrix[j][index];
                vexCode[j] = index;
            }
        }
        //更新所有顶点到新的顶点集的距离
    }
    float sum = 0;
    for (int i = 0; i < G.vexNum; i++)
    {
        if (i == vexCode[i])
            continue;
        sum += lowCost[i];
        cout << G.vexs[i] << " —— " << G.vexs[vexCode[i]] << " " << G.adjMatrix[i][vexCode[i]] << endl;
    }
    cout << "权值之和: " << sum << endl;
}

void heapAdjust(Edge *edge, int low /*当前堆根节点*/, int high /*当前堆末尾*/)
{
    //注释以大顶堆为例
    //使用该函数需要保证左右子堆均满足大顶堆的定义
    Edge temp = edge[low]; //摘下堆顶
    int i = low, j = 2 * i;
    //本循环的目的是将当前的堆顶low筛选到合适的位置
    while (j <= high)
    {
        //若右子堆顶存在且大于左子堆顶，选择之
        //注意，左右子堆顶之间的大小关系并无要求
        if (j + 1 <= high && edge[j].weigh < edge[j + 1].weigh)
            j++;
        //若当前堆顶大于左右子堆顶最大值，则已经是大顶堆，退出循环
        if (edge[j].weigh < temp.weigh)
            break;
        //若当前堆不是大顶堆，则将最大值上移
        edge[i] = edge[j];
        //继续向下筛选
        i = j;
        j = 2 * i;
    }
    edge[i] = temp;
}

void heapSort(Edge *edge, int len)
{
    for (int i = len / 2; i > 0; i--)
        heapAdjust(edge, i, len);
    for (int i = len; i > 1; i--)
    {
        Edge temp = edge[i];
        edge[i] = edge[1];
        edge[1] = temp;
        heapAdjust(edge, 1, i - 1);
    }
}

int findRoot(int *pre, int beg)
{
    int k = beg;
    while (pre[k] != -1)
        k = pre[k];
    return k;
}

void Kruskal(Graph G)
{
    Edge *edge = new Edge[G.arcNum + 1];

    int num = 1; //从1开始计数
    for (int i = 0; i < G.vexNum; i++)
    {
        for (int j = i + 1; j < G.vexNum; j++)
        {
            if (G.adjMatrix[i][j] < INFINITY && G.adjMatrix[i][j] > 0)
            {
                edge[num].leftEnd = i;
                edge[num].rightEnd = j;
                edge[num].weigh = G.adjMatrix[i][j];
                num++;
            }
        }
    }
    heapSort(edge, G.arcNum);
    //构建并查集
    //
    int *pre = new int[G.arcNum];
    for (int i = 0; i < G.arcNum; i++)
        pre[i] = -1;
    pre[edge[1].rightEnd] = edge[1].leftEnd;
    cout << edge[1].leftEnd << " —— " << edge[1].rightEnd << " " << edge[1].weigh << endl;
    //初始化
    float sum = edge[1].weigh;
    for (int i = 2; i < G.arcNum; i++)
    {
        int r1 = findRoot(pre, edge[i].leftEnd),
            r2 = findRoot(pre, edge[i].rightEnd);
        if (r1 == r2)
            continue;
        else
        {
            int from = edge[i].rightEnd, to = edge[i].leftEnd;
            if (pre[from] != -1)
            {
                from = edge[i].leftEnd;
                to = edge[i].rightEnd;
            }
            pre[from] = to;
            sum += edge[i].weigh;
            cout << G.vexs[edge[i].leftEnd] << " —— " << G.vexs[edge[i].rightEnd] << " " << edge[i].weigh << endl;
        }
    }
    cout << "权值之和为: " << sum << endl;
}

int main()
{
    Graph G;
    createGraph(G, UNDINET);
    cout << "prim" << endl;
    prim(G);
    cout << "Kruskal" << endl;
    Kruskal(G);
}