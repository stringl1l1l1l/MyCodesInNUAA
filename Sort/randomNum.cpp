#include <iostream>
#include <cstdlib>
#include <fstream>
#include <time.h>
using namespace std;
typedef int Elemtype;

#define MAX_NUM 32000

#define INITSIZE 256
#define INCRECEMENT 128

//索引统一从1开始
typedef struct SeqList
{
    Elemtype *pData; //动态存储空间的基地址
    int length;      //已有元素的数目
    int size;        //当前表的大小
} SeqList;

void exchange(Elemtype *elem1, Elemtype *elem2)
{
    if (*elem1 != *elem2)
    {
        Elemtype temp;
        temp = *elem1;
        *elem1 = *elem2;
        *elem2 = temp;
    }
}
void initSeqList(SeqList &L)
{
    L.pData = (Elemtype *)malloc(sizeof(Elemtype) * INITSIZE);
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
    L.pData[L.length + 1] = elem;
    L.length++;
}
int partition(SeqList L1, int low, int high, bool cmp(Elemtype a, Elemtype b))
{
    //如果只有1个数或没有数，直接return
    if (high <= low)
        return low;
    //如果只有两个数的情况
    else if (high - low == 1)
    {
        if (!cmp(L1.pData[low], L1.pData[high]))
            exchange(&L1.pData[low], &L1.pData[high]);
        return low;
    }
    else
    {
        //三者取中 begin
        int mid = (low + high) / 2;
        if (!cmp(L1.pData[low], L1.pData[high]))
            exchange(&L1.pData[low], &L1.pData[high]);
        if (!cmp(L1.pData[mid], L1.pData[high]))
            exchange(&L1.pData[mid], &L1.pData[high]);
        if (!cmp(L1.pData[low], L1.pData[mid]))
            exchange(&L1.pData[low], &L1.pData[mid]);
        //三者取中 end

        L1.pData[0] = L1.pData[mid]; //哨兵
        int privotKey = L1.pData[mid];
        exchange(&L1.pData[low + 1], &L1.pData[mid]); //此时中间值作为枢值，low的顺序已经确定，从low下一个数开始遍历
        int i = low + 1, j = high - 1;

        while (i < j)
        {
            while (i < j && (cmp(privotKey, L1.pData[j]) || privotKey == L1.pData[j]))
                j--;
            //从后往前，找到第一个比枢轴小的数
            L1.pData[i] = L1.pData[j];
            while (i < j && (cmp(L1.pData[i], privotKey) || privotKey == L1.pData[j]))
                i++;
            L1.pData[j] = L1.pData[i];
            //从前往后，找到第一个比枢轴大的数
        }
        if (i == j)
            L1.pData[i] = L1.pData[0];
        return i;
    }
}
void QSort(SeqList &L1, int low, int high, bool cmp(Elemtype a, Elemtype b))
{
    if (low < high)
    {
        int pivotLoc = partition(L1, low, high, cmp);
        QSort(L1, low, pivotLoc - 1, cmp);
        QSort(L1, pivotLoc + 1, high, cmp);
    }
}
double quickSort(SeqList L, bool cmp(Elemtype a, Elemtype b))
{
    QSort(L, 1, L.length, cmp);
}
bool descCmp(Elemtype a, Elemtype b)
{
    return a > b;
}
bool seqCmp(Elemtype a, Elemtype b)
{
    return a < b;
}

void randomNum(int count, char fileName[])
{
    fstream file(fileName, ios::out);
    if (file.fail())
    {
        cout << "Cannot open file!" << endl;
        exit(-1);
    }
    srand((unsigned int)time(NULL));

    for (int i = 0; i < count; i++)
    {
        int num = rand() % MAX_NUM;
        file << num << " ";
    }
    file.close();
}

void orderedNum(int count, char fileName[], bool cmp(Elemtype a, Elemtype b))
{
    SeqList L;
    initSeqList(L);
    fstream file(fileName, ios::out);
    if (file.fail())
    {
        cout << "Cannot open file!" << endl;
        exit(-1);
    }
    srand((unsigned int)time(NULL));

    for (int i = 0; i < count; i++)
    {
        int num = rand() % MAX_NUM;
        push(L, num);
    }

    quickSort(L, cmp);
    for (int i = 1; i <= L.length; i++)
        file << L.pData[i] << " ";

    file.close();
}

int main()
{
    char fileName[] = "D:\\Programming\\GitHub\\repository\\DataStruct\\Sort\\Sort.txt";
    randomNum(50000, fileName);
    //orderedNum(50000, fileName, descCmp);
    //orderedNum(50000, fileName, seqCmp);
}