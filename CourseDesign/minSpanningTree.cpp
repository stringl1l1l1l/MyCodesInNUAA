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
    //��ʼ��
    //�ӱ��Ϊ0�Ķ��㿪ʼ������С������
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
        //�ҳ����ﶥ�㼯������̵Ķ���
        //
        for (int j = 0; j < G.vexNum; j++)
        {
            if (finish[j] == false && lowCost[j] > G.adjMatrix[j][index])
            {
                lowCost[j] = G.adjMatrix[j][index];
                vexCode[j] = index;
            }
        }
        //�������ж��㵽�µĶ��㼯�ľ���
    }
    float sum = 0;
    for (int i = 0; i < G.vexNum; i++)
    {
        if (i == vexCode[i])
            continue;
        sum += lowCost[i];
        cout << G.vexs[i] << " ���� " << G.vexs[vexCode[i]] << " " << G.adjMatrix[i][vexCode[i]] << endl;
    }
    cout << "Ȩֵ֮��: " << sum << endl;
}

void heapAdjust(Edge *edge, int low /*��ǰ�Ѹ��ڵ�*/, int high /*��ǰ��ĩβ*/)
{
    //ע���Դ󶥶�Ϊ��
    //ʹ�øú�����Ҫ��֤�����ӶѾ�����󶥶ѵĶ���
    Edge temp = edge[low]; //ժ�¶Ѷ�
    int i = low, j = 2 * i;
    //��ѭ����Ŀ���ǽ���ǰ�ĶѶ�lowɸѡ�����ʵ�λ��
    while (j <= high)
    {
        //�����ӶѶ������Ҵ������ӶѶ���ѡ��֮
        //ע�⣬�����ӶѶ�֮��Ĵ�С��ϵ����Ҫ��
        if (j + 1 <= high && edge[j].weigh < edge[j + 1].weigh)
            j++;
        //����ǰ�Ѷ����������ӶѶ����ֵ�����Ѿ��Ǵ󶥶ѣ��˳�ѭ��
        if (edge[j].weigh < temp.weigh)
            break;
        //����ǰ�Ѳ��Ǵ󶥶ѣ������ֵ����
        edge[i] = edge[j];
        //��������ɸѡ
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

    int num = 1; //��1��ʼ����
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
    //�������鼯
    //
    int *pre = new int[G.arcNum];
    for (int i = 0; i < G.arcNum; i++)
        pre[i] = -1;
    pre[edge[1].rightEnd] = edge[1].leftEnd;
    cout << edge[1].leftEnd << " ���� " << edge[1].rightEnd << " " << edge[1].weigh << endl;
    //��ʼ��
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
            cout << G.vexs[edge[i].leftEnd] << " ���� " << G.vexs[edge[i].rightEnd] << " " << edge[i].weigh << endl;
        }
    }
    cout << "Ȩֵ֮��Ϊ: " << sum << endl;
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