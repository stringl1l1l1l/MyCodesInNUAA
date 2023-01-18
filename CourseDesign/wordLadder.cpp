#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
using namespace std;
#define SOURCE_FILE "D:\\Programming\\GitHub\\repository\\DataStruct\\CourseDesign\\.txt\\words.txt"
#define SORTED_WORD "D:\\Programming\\GitHub\\repository\\DataStruct\\CourseDesign\\.txt\\sortedWords.txt"
typedef struct Vetrix
{
    int num;
    string name;
} Vetrix;

typedef struct ArcNode
{
    int adjVex;
    ArcNode *nextarc;
} ArcNode;

typedef struct VexNode
{
    string data;
    ArcNode *firstarc;
} VexNode;

typedef struct Graph
{
    VexNode *adjTable;
    int vexNum, arcNum;
    string *vexs;
} Graph;

bool cmp(string str1, string str2)
{
    return str1.length() < str2.length();
}

void sortByLenth(vector<string> &wordVec)
{
    fstream inFile(SOURCE_FILE, ios::in);
    if (inFile.fail())
    {
        cout << "Cannot open file!" << endl;
        exit(-1);
    }
    string word;
    while (inFile >> word)
        wordVec.push_back(word);
    // sort(wordVec.begin(), wordVec.end(), cmp);
    inFile.close();
}

void readFromFile(set<string> &wordSet)
{
    fstream inFile(SOURCE_FILE, ios::in);
    if (inFile.fail())
    {
        cout << "Cannot open file!" << endl;
        exit(-1);
    }
    string word;
    while (inFile >> word)
        wordSet.insert(word);
    inFile.close();
}

int culculateDiff(string str1, string str2)
{
    if (str1.length() == str2.length())
    {
        int diff = 0;
        for (size_t i = 0; i < str1.length(); i++)
        {
            if (str1[i] != str2[i])
                diff++;
        }
        return diff;
    }
    else
    {
        cout << "长度不一致" << endl;
        return -1;
    }
}

bool judgeDiff(string str1, string str2)
{
    if (str1.length() == str2.length())
    {
        int diff = 0;
        for (size_t i = 0; i < str1.length(); i++)
        {
            if (str1[i] != str2[i])
                diff++;
            if (diff == 2)
                break;
        }
        if (diff == 2)
            return false;
        else
            return true;
    }
}
void buildGraph(Graph &G, int num)
{
    fstream inFile(SOURCE_FILE, ios::in);
    if (inFile.fail())
    {
        cout << "Cannot open file!" << endl;
        exit(-1);
    }
    int count = 0, flag = 0;
    string word;
    string *words = new string[40000];
    for (int i = 0; inFile >> word; i++)
    {
        if (word.length() == num && flag == 0)
            flag = 1;
        if (flag == 1 && word.length() == num)
            words[count++] = word;
        else if (word.length() > num)
            break;
    }
    G.vexNum = count;
    G.vexs = words;
    G.adjTable = new VexNode[G.vexNum];
    for (size_t i = 0; i < count; i++)
    {
        G.adjTable[i].data = G.vexs[i];
        G.adjTable[i].firstarc = NULL;
    }
    //选择长度相等的字符串序列
    for (int j = 0; j < count; j++)
    {
        for (int k = j + 1; k < count; k++)
        {
            if (judgeDiff(words[j], words[k]))
            {
                ArcNode *node1 = new ArcNode, *node2 = new ArcNode;
                node1->adjVex = k;
                node1->nextarc = G.adjTable[j].firstarc;
                G.adjTable[j].firstarc = node1;
                node2->adjVex = j;
                node2->nextarc = G.adjTable[k].firstarc;
                G.adjTable[k].firstarc = node2;
                G.arcNum++;
            }
        }
    }
    inFile.close();
}

void findRoadByBFS(Graph G, string beg, string end)
{
    queue<int> Q;
    int *visit = new int[G.vexNum];
    int b = 0, e = 0;
    for (int i = 0; i < G.vexNum; i++)
    {
        if (G.vexs[i] == beg)
            b = i;
        if (G.vexs[i] == end)
            e = i;
    }
    if (b == 0)
    {
        cout << "输入的首单词不存在!" << endl;
        return;
    }
    if (e == 0)
    {
        cout << "输入的尾单词不存在!" << endl;
        return;
    }
    //初始化标记数组为未访问
    for (int i = 0; i < G.vexNum; i++)
        visit[i] = -1;

    Q.push(b);
    visit[b] = b;
    int last = b;
    int num = 0;
    while (!Q.empty())
    {
        int curFront = Q.front();
        if (curFront == e)
            break;
        Q.pop();
        //队头所有邻接点入队
        ArcNode *p = G.adjTable[curFront].firstarc;
        while (p)
        {
            if (visit[p->adjVex] == -1)
            {
                visit[p->adjVex] = curFront;
                Q.push(p->adjVex);
            }
            p = p->nextarc;
        }
    }

    vector<int> res;
    res.push_back(e);
    int cur = visit[e];
    while (cur != b)
    {
        if (cur == -1)
            cout << "不存在词梯" << endl;
        res.push_back(cur);
        cur = visit[cur];
    }
    res.push_back(b);

    for (int i = res.size() - 1; i >= 0; i--)
    {
        cout << G.vexs[res[i]];
        if (i != 0)
            cout << "->";
    }
}
int main()
{
    Graph G;
    int num = 0;
    cout << "请输入单词长度: ";
    cin >> num;
    string beg, end;
    cout << "请输入首单词: " << endl;
    cin >> beg;
    cout << "请输入尾单词: " << endl;
    cin >> end;
    buildGraph(G, num);
    findRoadByBFS(G, beg, end);
}
//void buildGraph2(Graph& G, int num)
//{
//    set<string> wordSet;
//    readFromFile(wordSet);
//    map<string, int> wordMap;
//    set<string>::iterator beg = wordSet.begin(), end = wordSet.end();
//    for (set<string>::iterator it = wordSet.begin(); it != wordSet.end(); ++it)
//    {
//        if (it->length() == num)
//        {
//            beg = it;
//            break;
//        }
//    }
//    set<string>::iterator it = beg, pre = beg;
//    int code = 0;
//    for (it = beg; it != wordSet.end(); pre = it, ++it)
//    {
//        G.vexs.push_back(*it);
//        wordMap.insert(make_pair(*it, code++));
//        if (pre->length() == num && it->length() > num)
//        {
//            end = it;
//            break;
//        }
//    }
//    if (it == wordSet.end())
//        end = it;
//    set<string> curSet(beg, end);
//    G.vexNum = G.vexs.size();
//    G.adjTable = new VexNode[G.vexNum];
//    for (int i = 0; i < G.vexNum; i++)
//    {
//        string curWord = G.vexs[i], temp = curWord;
//        for (size_t j = 0; j < num; j++)
//        {
//            char letter = 'A';
//            while (letter <= 'z')
//                curWord[j] = letter++;
//            if (curSet.find(curWord) != end)
//            {
//                ArcNode* node1 = new ArcNode, * node2 = new ArcNode;
//                node1->adjVex = wordMap.at(curWord);
//                node1->nextarc = G.adjTable[wordMap.at(temp)].firstarc;
//                G.adjTable[wordMap.at(temp)].firstarc = node1;
//                node2->adjVex = wordMap.at(temp);
//                node2->nextarc = G.adjTable[wordMap.at(curWord)].firstarc;
//                G.adjTable[wordMap.at(curWord)].firstarc = node2;
//                G.arcNum++;
//            }
//            curWord = temp;
//        }
//    }
//}