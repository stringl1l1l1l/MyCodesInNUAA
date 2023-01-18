#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string.h>
#include <queue>
#include <stack>
using namespace std;
#define FILE_NAME "D:\\Programming\\GitHub\\repository\\DataStruct\\CourseDesign\\.txt\\JSON.txt"

#define INITSIZE 256
#define INCRECEMENT 128

typedef int Elemtype;
typedef struct SeqList
{
    int *pData; //动态存储空间的基地址
    int length; //已有元素的数目
    int size;   //当前表的大小
} SeqList;
void initSeqList(SeqList &L)
{
    L.pData = (int *)malloc(sizeof(int) * INITSIZE);
    L.length = 0;
    L.size = INITSIZE;
}
void push(SeqList &L, Elemtype elem)
{
    if (L.length >= L.size - 1)
    {
        L.pData = (Elemtype *)realloc(L.pData, sizeof(Elemtype) * (L.size + INCRECEMENT));
        L.size += INCRECEMENT;
    }
    L.pData[L.length] = elem;
    L.length++;
}
int deleteElem(SeqList &L, int index)
{
    if (index < 0 || index >= L.length)
    {
        printf("Error:Illegal parameter!");
        exit(-1);
    }

    int oldVal = L.pData[index];

    for (int i = index; i < L.length - 1; i++)
        L.pData[i] = L.pData[i + 1];
    L.length--;
    return oldVal;
}

typedef struct TreeNode
{
    pair<char *, char *> data;
    TreeNode *firstChild, *nextBro;
} TreeNode, *Tree;

void initTree(Tree &T)
{
    T = (TreeNode *)malloc(sizeof(TreeNode));
    T->firstChild = NULL;
    T->nextBro = NULL;
}

void deleteFromStr(char *str, int beg, int end)
{
    if (str[end + 1] == 0)
    {
        for (int m = 0; str[m + 1]; m++)
            str[m] = 0;
        return;
    }
    int j, k;
    if (str[end + 1] == ',')
        end++;
    for (k = beg, j = end + 1; str[j]; j++, k++)
        str[k] = str[j];
    for (int m = k; str[m + 1]; m++)
        str[m] = 0;
}

//该函数将只含有一组引号(未转义)的子串转换为一个键或值
char *transToKey(char *JSON, int beg, int end)
{
    int i = beg;
    int flag = 0, num = 0;
    char *content = new char[100];
    content[0] = 0;
    while (JSON[i] && (flag == 0 || JSON[i] != '"'))
    {
        char sub[2] = "";
        if (JSON[i] == ',')
            i++;
        if (JSON[i] == ':')
            i++;
        if (JSON[i] == ' ')
            i++;
        if (JSON[i] == '\\')
            i++;
        if (JSON[i] == '"')
        {
            if (flag == 0)
            {
                flag = 1;
                i++;
                continue;
            }
            else
            {
                if (JSON[i - 1] == '\\')
                {
                    sub[0] = JSON[i];
                    strcat(content, sub);
                    i++;
                    continue;
                }
            }
        }
        sub[0] = JSON[i];
        strcat(content, sub);
        i++;
    }
    return content;
}
char type1[] = "OBJECT";
char type2[] = "STRING";
char type3[] = "value";
//该函数用于读取最内层对象的键值对，读取完毕后将其删去
TreeNode *oneObject(Tree &T, char *JSON, TreeNode *objectVal, int beg, int end)
{
    int i = beg + 1;

    TreeNode *head = (TreeNode *)malloc(sizeof(TreeNode)), *curPtr = head;
    int num = 0;
    char *res[100];

    while (JSON[i] && JSON[i] != '}')
    {
        int flag = 0;
        //char content[100]=""; 在函数内初始化不会改变地址,对同一块地址操作会导致存储的值同时改变
        //改变地址
        char *content = new char[100];
        for (int k = 0; k < 100; k++)
            content[k] = 0;
        while (JSON[i] && (flag == 0 || JSON[i] != '"'))
        {
            char sub[2] = "";
            if (JSON[i] == ',')
                i++;
            if (JSON[i] == ':')
                i++;
            if (JSON[i] == ' ')
                i++;
            if (JSON[i] == '\\')
                i++;
            if (JSON[i] == '"')
            {
                if (flag == 0)
                {
                    flag = 1;
                    i++;
                    continue;
                }
                else
                {
                    if (JSON[i - 1] == '\\')
                    {
                        sub[0] = JSON[i];
                        strcat(content, sub);
                        i++;
                        continue;
                    }
                }
            }
            sub[0] = JSON[i];
            strcat(content, sub);

            i++;
        }

        //cout << content << endl;
        res[num++] = content;
        i++;
    }

    deleteFromStr(JSON, beg, end);
    char *str = new char[100];
    str[0] = 0;
    int m;
    for (m = beg - 1;; m--)
    {
        if (JSON[m] == ',' || JSON[m] == '{')
            break;
    }
    //找到该对象的名字，向前遍历到逗号或左花括号
    strcpy(str, transToKey(JSON, m + 1, beg - 1));
    deleteFromStr(JSON, m + 1, beg - 1);
    head->data.second = str;

    head->data.first = type1;
    head->nextBro = NULL;
    for (int k = 0, v = 1; k < num && v < num; k += 2, v += 2)
    {
        TreeNode *key = (TreeNode *)malloc(sizeof(TreeNode));
        TreeNode *val = (TreeNode *)malloc(sizeof(TreeNode));
        key->firstChild = val;
        key->data.first = type2;
        val->nextBro = NULL;
        val->firstChild = NULL;
        if (k == 0)
            curPtr->firstChild = key;
        else
            curPtr->nextBro = key;

        curPtr = key;
        curPtr->data.second = res[k];
        curPtr->firstChild->data.first = type3;
        curPtr->firstChild->data.second = res[v];
    }
    curPtr->nextBro = objectVal;
    return head;
}

void buildTree(Tree &T)
{
    fstream file(FILE_NAME, ios::in);
    if (file.fail())
        exit(-1);
    TreeNode *curPtr = T, *nextPtr = NULL;
    int rowNum = 0, i;
    file >> rowNum >> i;
    char JSON[1000] = "", str[200] = "";
    for (i = 0; i <= rowNum; i++)
    {
        file.getline(str, 200);
        strcat(JSON, str);
    }

    SeqList L;
    initSeqList(L);
    for (int i = 0, num = 1; JSON[i]; i++)
    {
        if (JSON[i] == '{')
            push(L, i);
        else if (JSON[i] == '}')
            push(L, -i);
    }

    TreeNode *objectVal = NULL;
    while (L.length != 0)
    {
        int beg = 0, end = 0;
        for (int i = L.length - 1; i >= 0; i--)
        {
            if (L.pData[i] >= 0)
            {
                beg = deleteElem(L, i);
                end = -deleteElem(L, i);
                break;
            }
        }
        objectVal = oneObject(T, JSON, objectVal, beg, end);
    }
    T = objectVal;
    file.close();
}

TreeNode *JSONTreeQuery(Tree &T, char *str)
{
    if (T)
    {
        stack<TreeNode *> stk;
        stk.push(T);
        while (!stk.empty())
        {
            TreeNode *curNode = stk.top();
            stk.pop();
            if (strcmp(str, curNode->data.second) == 0)
                return curNode;

            if (curNode->firstChild)
                stk.push(curNode->firstChild);

            if (curNode->nextBro)
                stk.push(curNode->nextBro);
        }
    }
}

void JSONQuery(Tree &T)
{
    fstream file(FILE_NAME, ios::in);
    file.seekg(0L, ios::beg);
    if (file.fail())
        exit(-1);

    int rowNum1 = 0, rowNum2 = 0;
    char *query = new char[200];
    file >> rowNum1 >> rowNum2;
    for (int i = 0; i <= rowNum1; i++)
        file.getline(query, 200);

    while (!file.eof())
    {
        TreeNode *curPtr = NULL, *lastPtr = NULL;
        query = new char[200]; //当前查询
        for (int k = 0; k < 200; k++)
            query[k] = 0;
        file.getline(query, 200);
        //
        char *res[100];
        char *content = new char[100];
        for (int k = 0; k < 100; k++)
            content[k] = 0;
        //初始化字符串
        //
        int i = 0, beg = 0, count = 0;
        for (i = 0, beg = 0, count = 0; query[i]; i++)
        {
            if (query[i] == '.')
            {
                int j = 0;
                for (int k = beg; k < i; k++)
                    content[j++] = query[k];
                for (int k = j; content[k + 1]; k++)
                    content[k] = 0;
                res[count++] = content;
                content = new char[100];
                for (int k = 0; k < 100; k++)
                    content[k] = 0;
                beg = i + 1;
            }
        }
        int j = 0;
        for (int k = beg; k < i; k++)
            content[j++] = query[k];
        for (int k = j; content[k + 1]; k++)
            content[k] = 0;
        res[count] = content;
        count++;
        //读取按"."分隔的字符串
        //
        lastPtr = JSONTreeQuery(T, res[0]);
        curPtr = lastPtr;
        for (int k = 1; k < count; k++)
        {
            curPtr = lastPtr->firstChild;
            //
            while (curPtr)
            {
                if (strcmp(curPtr->data.second, res[k]) == 0)
                    break;
                curPtr = curPtr->nextBro;
            }
            // 查询同一代所有兄弟，检查待查询对象是否存在
            lastPtr = curPtr;
        }

        if (curPtr)
        {
            if (strcmp(curPtr->data.first, "OBJECT") != 0)
                cout << curPtr->data.first << " " << curPtr->firstChild->data.second << endl;
            else
                cout << curPtr->data.first << " " << curPtr->data.second << endl;
        }
        else
            cout << "NO EXIST" << endl;
    }
    file.close();
}

int main()
{
    Tree T;
    initTree(T);
    buildTree(T);
    JSONQuery(T);
}