#include <cstdlib>
#include <cstdio>

#define INITSIZE 256
#define INCRECEMENT 128

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

void createSeqList(SeqList &L, int num, int elem[])
{
    if (num <= 0)
    {
        printf("Erorr:Illegal parameter!");
        exit(-1);
    }

    while (num > L.size)
    {
        realloc(L.pData, sizeof(int) * (L.size + INCRECEMENT));
        L.size += INCRECEMENT;
    }

    for (int i = 0; i < num; i++)
        L.pData[i] = elem[i];
    L.length = num;
}

void destroySeqList(SeqList &L)
{
    if (L.pData)
    {
        free(L.pData);
        L.pData = NULL;
    }

    L.length = 0;
    L.size = 0;
}

void clearSeqList(SeqList &L)
{
    L.length = 0;
}

bool isEmpty(SeqList &L)
{
    return L.length == 0 ? true : false;
}

int length(SeqList &L)
{
    return L.length;
}

int locateElem(SeqList &L, int elem)
{
    for (int i = 0; i < L.length; i++)
    {
        if (L.pData[i] == elem)
            return i + 1;
    }

    printf("Error:Cannot find the element!");
    exit(-1);
}

int getPriorElem(SeqList &L, int elem)
{
    int pre = locateElem(L, elem) - 1;

    if (pre > 0)
        return L.pData[pre - 1];
    else
    {
        printf("Error:Current element is the first element!");
        exit(-1);
    }
}

int getNextElem(SeqList &L, int elem)
{
    int next = locateElem(L, elem) + 1;

    if (next <= L.length)
        return L.pData[next - 1];
    else
    {
        printf("Error:Current element is the last element!");
        exit(-1);
    }
}

int traverseSeqList(SeqList &L)
{
    if (L.length == 0)
    {
        printf("Empty\n");
        return 0;
    }

    for (int i = 0; i < L.length; i++)
        printf("%d ", L.pData[i]);
    printf("\n");

    return 0;
}

//3.加工型操作
int SetElem(SeqList &L, int loc, int elem)
{
    if (loc < 1 || loc > length(L))
    {
        printf("Error:Illegal parameter!");
        exit(-1);
    }

    int oldVal = L.pData[loc - 1];
    L.pData[loc - 1] = elem;
    return oldVal;
}

void insertElem(SeqList &L, int loc, int elem)
{
    if (loc < 1 || loc > length(L) + 1)
    {
        printf("Error:Illegal parameter!");
        exit(-1);
    }

    if (L.length >= L.size)
    {
        L.pData = (int *)realloc(L.pData, sizeof(int) * (L.size + INCRECEMENT));
        L.size += INCRECEMENT;
    }

    for (int i = L.length - 1; i >= loc - 1; i--)
        L.pData[i + 1] = L.pData[i];
    L.pData[loc - 1] = elem;
    L.length++;
}

int deleteElem(SeqList &L, int loc)
{
    if (loc < 1 || loc > length(L))
    {
        printf("Error:Illegal parameter!");
        exit(-1);
    }

    int oldVal = L.pData[loc - 1];

    for (int i = loc - 1; i < length(L) - 1; i++)
        L.pData[i] = L.pData[i + 1];
    L.length--;
    return oldVal;
}

//4.附加操作
int reverseSeqList(SeqList &L)
{
    if (L.length == 0 || L.length == 1)
        return 0;

    for (int i = 0; i <= (L.length - 1) / 2; i++)
    {
        int temp;
        temp = L.pData[i];
        L.pData[i] = L.pData[L.length - 1 - i];
        L.pData[L.length - 1 - i] = temp;
    }
    return 0;
}

int deleteDuplicateElem(SeqList &L)
{
    if (L.length == 0 || L.length == 1)
        return 0;
    int i = 0, j = 1, len = 1; //len指不重复元素的索引
    for (j = 1; j < L.length; j++)
    {
        for (i = 0; i < len; i++)
        {
            //若该元素与前面的元素重复，则退出该循环，跳过该元素，继续判断下一个元素
            if (L.pData[j] == L.pData[i])
                break;
        }
        //若循环进行到了最后，将不重复元素放到前面
        if (i == len)
            L.pData[len++] = L.pData[j];
    }

    //去除重复元素
    L.length = len;

    return 0;
}
int bubbleSort(SeqList &L)
{
    if (L.length == 0 || L.length == 1)
        return 0;

    for (int i = 0; i < L.length - 1; i++)
    {
        //错误代码for(int j = i; j < L.length-1; j++)
        //原因是冒泡排序的一次循环将最值放到最右边，不是最左边，无论升序还是逆序
        for (int j = 0; j < L.length - 1 - i; j++)
        {
            if (L.pData[j] > L.pData[j + 1])
            {
                int temp = L.pData[j];
                L.pData[j] = L.pData[j + 1];
                L.pData[j + 1] = temp;
            }
        }
    }
    return 0;
}

int selectiveSort(SeqList &L)
{
    if (L.length == 0 || L.length == 1)
        return 0;

    for (int i = 0; i < L.length - 1; i++)
    {
        int k = i;
        for (int j = i + 1; j < L.length /*不要写成L.length - 1*/; j++)
        {
            if (L.pData[k] > L.pData[j])
                k = j;
        }
        if (k == i)
            continue; //不要写成break

        int temp = L.pData[i]; //不要写成L.pData[j]
        L.pData[i] = L.pData[k];
        L.pData[k] = temp;
    }
}

int combineSeqListInDescOrder(SeqList &L1, SeqList &L2)
{
    if (L1.length == 0)
    {
        L1 = L2;
        return 0;
    }
    int i = 0, j = 0;
    while (i < L1.length && j < L2.length)
    {
        if (L2.pData[j] > L1.pData[i])
            insertElem(L1, i + 1, L2.pData[j++]);
        else if (L2.pData[j] == L1.pData[i])
        {
            i++;
            j++;
        }
        else
            i++;
    }

    //将L2中所有比L1最小的数还小的数插入到L1尾后
    if (j <= L2.length)
    {
        for (int k = j; k < L2.length; k++)
            insertElem(L1, ++i, L2.pData[k]);
    }
    return 0;
}

int JosephusBySeqlist(SeqList &L, int n, int m)
{
    if (L.length == 0)
        return 0;

    int num = 1;
    for (int i = 0; L.length; i++)
    {
        if (L.pData[i % n] == 0)
            continue;
        else if ((num++ % m) == 0)
        {
            printf("%d ", L.pData[i % n]); //不要写成i%(n-1)
            L.pData[i % n] = 0;
            L.length--;
        }
    }
    return 0;
}

int main()
{
    SeqList l1, l2;
    int elem1[4] = {7, 5, 2, 1};
    int elem2[7] = {9, 8, 7, 4, 3, 0, -1};
    initSeqList(l1);
    initSeqList(l2);
    createSeqList(l1, 4, elem1);
    createSeqList(l2, 7, elem2);
    combineSeqListInDescOrder(l1, l2);
    traverseSeqList(l1);
}