#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <map>
#include <queue>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

#define FILE_NAME "D:\\Programming\\GitHub\\repository\\DataStruct\\CourseDesign\\.txt\\busLine.txt"

typedef struct Vetrix
{
    int num;
    string name;
} Vetrix;

struct AdjNode
{
    int flag;
    int lineNum;
};

typedef struct Graph
{
    AdjNode *adjMatrix[6000];
    int vexNum, arcNum;
    Vetrix vexs[6000];
} Graph;

bool cmp(Vetrix *v1, Vetrix *v2)
{
    return v1->name < v2->name;
}

multimap<int, Vetrix> readFromFile()
{
    fstream file(FILE_NAME, ios::in);
    if (file.fail())
    {
        cout << "Cannot open file!" << endl;
        exit(-1);
    }

    string line;

    multimap<int, Vetrix> content;
    while (getline(file, line))
    {
        istringstream isstream(line);
        string station, lineNum;
        isstream >> lineNum;
        //
        stringstream sstream(lineNum);
        int num;
        sstream >> num;
        //字符串转为int
        while (isstream >> station)
        {
            Vetrix v;
            v.num = 0;
            v.name = station;
            content.insert(make_pair(num, v));
        }
    }
    file.close();
    return content;
}

void createMinTransGraph(Graph &G1)
{
    for (int i = 0; i < 6000; i++)
    {
        G1.adjMatrix[i] = (AdjNode *)malloc(sizeof(AdjNode) * 6000);
        for (int j = 0; j < 6000; j++)
            G1.adjMatrix[i][j].flag = 0;
    }
    multimap<int, Vetrix> content = readFromFile();
    //
    vector<Vetrix *> vec;
    for (multimap<int, Vetrix>::iterator beg = content.begin(); beg != content.end(); beg++)
        vec.push_back(&(beg->second));
    sort(vec.begin(), vec.end(), cmp);

    int num = 1;
    vector<Vetrix *>::iterator it = vec.begin();
    (*it)->num = num;
    G1.vexs[num] = **it;
    vector<Vetrix *>::iterator dupli_beg = it + 1;
    while (it != vec.end() && dupli_beg != vec.end())
    {
        if ((*dupli_beg)->name == (*it)->name)
        {
            (*dupli_beg)->num = num;
            dupli_beg++;
        }
        else
        {
            it = dupli_beg;
            (*it)->num = ++num;
            G1.vexs[num] = **it;
            dupli_beg = it + 1;
        }
    }
    //给重复元素编相同的号,并按编号建立顶点数组
    unique(vec.begin(), vec.end());
    //去重
    G1.vexNum = num;
    G1.arcNum = 0;

    for (int i = 1; i <= 990; i++)
    {
        for (multimap<int, Vetrix>::iterator it = content.lower_bound(i),
                                             end = content.upper_bound(i);
             it != end; ++it)
        {
            for (multimap<int, Vetrix>::iterator it2 = content.lower_bound(i),
                                                 end2 = content.upper_bound(i);
                 it2 != end2; ++it2)
            {
                if (it2->second.num == it->second.num)
                    continue;
                int row = it->second.num, col = it2->second.num;
                G1.adjMatrix[row][col].flag = 1;
                G1.adjMatrix[row][col].flag = 1;
                G1.adjMatrix[row][col].lineNum = i;
                G1.adjMatrix[col][row].lineNum = i;
            }
        }
    }

    for (int i = 1; i <= num; i++)
    {
        for (int j = i + 1; j <= num; j++)
        {
            if (G1.adjMatrix[i][j].flag == 1)
                G1.arcNum++;
        }
    }
}

void createMinStationGraph(Graph &G2)
{
    for (int i = 0; i < 6000; i++)
    {
        G2.adjMatrix[i] = (AdjNode *)malloc(sizeof(AdjNode) * 6000);
        for (int j = 0; j < 6000; j++)
            G2.adjMatrix[i][j].flag = 0;
    }
    multimap<int, Vetrix> content = readFromFile();
    //
    vector<Vetrix *> vec;
    for (multimap<int, Vetrix>::iterator beg = content.begin(); beg != content.end(); beg++)
        vec.push_back(&(beg->second));
    sort(vec.begin(), vec.end(), cmp);

    int num = 1;
    vector<Vetrix *>::iterator it = vec.begin();
    (*it)->num = num;
    G2.vexs[num] = **it;
    vector<Vetrix *>::iterator dupli_beg = it + 1;
    while (it != vec.end() && dupli_beg != vec.end())
    {
        if ((*dupli_beg)->name == (*it)->name)
        {
            (*dupli_beg)->num = num;
            dupli_beg++;
        }
        else
        {
            it = dupli_beg;
            (*it)->num = ++num;
            G2.vexs[num] = **it;
            dupli_beg = it + 1;
        }
    }
    //给重复元素编相同的号,并按编号建立顶点数组
    unique(vec.begin(), vec.end());
    G2.vexNum = num;
    G2.arcNum = 0;

    for (int i = 1; i <= 990; i++)
    {
        for (multimap<int, Vetrix>::iterator it = content.lower_bound(i), end = content.upper_bound(i); it != end; ++it)
        {
            multimap<int, Vetrix>::iterator it2 = it;
            int row = it2->second.num;
            it2++;
            if (it2 == end)
                break;
            int col = it2->second.num;
            if (row == col)
                continue;
            G2.adjMatrix[row][col].flag = 1;
            G2.adjMatrix[col][row].flag = 1;
            G2.adjMatrix[row][col].lineNum = i;
            G2.adjMatrix[col][row].lineNum = i;
        }
    }

    for (int i = 1; i <= num; i++)
    {
        for (int j = i + 1; j <= num; j++)
        {
            if (G2.adjMatrix[i][j].flag == 1)
                G2.arcNum++;
        }
    }
}

void findRoadByBFS(Graph G, string b, string e)
{
    int beg = 0, end = 0;
    for (int i = 1; i <= G.vexNum; i++)
    {
        if (G.vexs[i].name == b)
            beg = i;
        if (G.vexs[i].name == e)
            end = i;
    }
    if (beg == 0 || end == 0)
    {
        cout << "输入的站名有误!" << endl;
        return;
    }
    queue<int> Q;
    bool *visit = new bool[G.vexNum + 1];
    int *pass = (int *)malloc(sizeof(int) * (G.vexNum + 1));
    for (int i = 0; i <= G.vexNum; i++)
        pass[i] = 0;

    //初始化标记数组为未访问
    for (int i = 1; i <= G.vexNum; i++)
        visit[i] = false;

    Q.push(beg);
    visit[beg] = true;

    while (!Q.empty())
    {
        int curFront = Q.front();
        if (curFront == end)
            break;

        Q.pop();
        //队头所有邻接点入队
        for (int j = 1; j <= G.vexNum; j++)
        {
            if (G.adjMatrix[curFront][j].flag == 1)
            {
                if (visit[j] == false)
                {
                    visit[j] = true;
                    Q.push(j);
                    pass[j] = curFront;
                }
            }
        }
    }

    int res[200], i = 1;
    res[0] = end;
    int curStation = end;
    while (curStation != beg)
    {
        if (pass[curStation] == 0)
        {
            cout << "没有可达的线路" << endl;
            return;
        }
        res[i++] = pass[curStation];
        curStation = pass[curStation];
    }

    int j = 0;
    for (j = i - 1; j > 0; j--)
    {
        int lineNum = G.adjMatrix[res[j]][res[j - 1]].lineNum;
        if (j == i - 1)
            cout << "\t起点: " << G.vexs[res[j]].name;
        else
            cout << setw(20) << G.vexs[res[j]].name;
        cout << endl;
        cout << "- " << setw(3) << lineNum;
        cout << " 号-> ";
    }
    cout << "\n"
         << "\t终点: " << G.vexs[res[j]].name << "\n"
         << endl;
}

int main()
{
    Graph G1;
    Graph G2;

    string beg, end;
    cout << "请输入起点与终点" << endl;
    cin >> beg >> end;
    cout << "最少转站: " << endl;
    createMinTransGraph(G1);
    findRoadByBFS(G1, beg, end);
    cout << "最少站数: " << endl;
    createMinStationGraph(G2);
    findRoadByBFS(G2, beg, end);
}