#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <algorithm>
using namespace std;

typedef int Elemtype;

#define INITSIZE 256
#define INCRECEMENT 128

//????????1???
typedef struct SeqList
{
    Elemtype *pData; //?????????????
    int length;      //???????????
    int size;        //?????????
} SeqList;

typedef struct LNode
{
    Elemtype data;
    LNode *next;
} LNode, *QueuePtr;

typedef struct LinkQueue
{
    QueuePtr head;
    QueuePtr rear;
} LinkQueue;

int initQueue(LinkQueue &Q)
{
    Q.rear = NULL;

    Q.head = (LNode *)malloc(sizeof(LNode));
    Q.rear = Q.head;
    if (!Q.head)
        exit(-1);
    //?????????????
    Q.head->next = NULL;
    return 0;
}

void initSeqList(SeqList &L)
{
    L.pData = (Elemtype *)malloc(sizeof(Elemtype) * INITSIZE);
    L.length = 0;
    L.size = INITSIZE;
}

void insertElem(SeqList &L, int loc, Elemtype elem)
{
    if (loc < 1 || loc > L.length + 1)
    {
        printf("Error:Illegal parameter!");
        exit(-1);
    }

    if (L.length >= L.size)
    {
        L.pData = (Elemtype *)realloc(L.pData, sizeof(Elemtype) * (L.size + INCRECEMENT));
        L.size += INCRECEMENT;
    }

    for (int i = L.length - 1; i >= loc - 1; i--)
        L.pData[i + 1] = L.pData[i];
    L.pData[loc - 1] = elem;
    L.length++;
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

int traverseSeqList(SeqList &L)
{
    if (L.length == 0)
    {
        printf("Empty\n");
        return 0;
    }

    for (int i = 1; i <= L.length; i++)
        printf("%d ", L.pData[i]);
    printf("\n");
    return 0;
}

void readFromFile(SeqList &L, char fileName[])
{
    fstream file(fileName, ios::in);
    if (file.fail())
    {
        cout << "Cannot open file!" << endl;
        exit(-1);
    }
    int count = 0;
    Elemtype val = 0;
    while (file >> val)
        push(L, val);

    file.close();
}

void seqListCpy(SeqList &target, SeqList &object)
{
    for (int i = 1; i <= object.length; i++)
        push(target, object.pData[i]);
}

bool descCmp(Elemtype a, Elemtype b)
{
    return a > b;
}

bool seqCmp(Elemtype a, Elemtype b)
{
    return a < b;
}

//??????

//??????
void insert(SeqList &L1, int i, bool cmp(Elemtype a, Elemtype b))
{
    //????????????i??????????????????
    Elemtype temp = L1.pData[i];
    // i???????????????????????
    int j;
    for (j = i; j > 1; j--)
    {
        //??????????????????????????????????????????????????????????
        if (cmp(temp, L1.pData[j - 1]))
            L1.pData[j] = L1.pData[j - 1];
        else
            break;
    }
    L1.pData[j] = temp;
}
//??????????
double insertSort(SeqList L, bool cmp(Elemtype a, Elemtype b))
{
    SeqList L1;
    initSeqList(L1);
    seqListCpy(L1, L);
    clock_t start, stop;
    double diff = 0;
    cout << "insertSort begin" << endl;
    start = clock();
    //
    for (int i = 2; i <= L.length; i++)
        insert(L1, i, cmp);
    //
    stop = clock();
    diff = (double)((stop - start));
    cout << "insertSort finished "
         << ", cost " << diff << "ms" << endl;
    return diff;
}
//????????????
void ShellInsert(SeqList &L1, int d, bool cmp(Elemtype a, Elemtype b))
{
    // i???????????????
    for (int i = d + 1; i <= L1.length; i++)
    {
        if (cmp(L1.pData[i], L1.pData[i - d])) //?????????????????????????????????????
        {
            Elemtype temp = L1.pData[i];
            int j;
            for (j = i; j > d; j -= d)
            {
                if (cmp(temp, L1.pData[j - d]))
                    L1.pData[j] = L1.pData[j - d];
                else
                    break;
            }
            L1.pData[j] = temp;
        }
    }
}
//???????
double ShellSort(SeqList &L, int d[], int t, bool cmp(Elemtype a, Elemtype b))
{
    SeqList L1;
    initSeqList(L1);
    seqListCpy(L1, L);

    clock_t start, stop;
    double diff = 0;
    cout << "ShellSort begin" << endl;
    start = clock();
    //
    for (int i = 0; i < t; i++)
        ShellInsert(L1, d[i], cmp);
    //
    stop = clock();
    diff = (double)((stop - start));
    // traverseSeqList(L1);
    cout << "ShellSort finished"
         << ", cost " << diff << "ms" << endl;
    return diff;
}
//?????????
double bubbleSort(SeqList L, bool cmp(Elemtype a, Elemtype b))
{
    SeqList L1;
    initSeqList(L1);
    seqListCpy(L1, L);

    clock_t start, stop;
    double diff = 0;
    cout << "bubbleSort begin" << endl;
    start = clock();

    int flag = 0;
    for (int i = 0; i < L1.length - 1; i++)
    {
        for (int j = 0; j < L1.length - i - 1; j++)
        {
            if (cmp(L1.pData[j + 1], L1.pData[j]))
            {
                flag = 1;
                Elemtype temp;
                temp = L1.pData[j];
                L1.pData[j] = L1.pData[j + 1];
                L1.pData[j + 1] = temp;
            }
        }
        if (flag == 0) //???????????????????????
            break;
    }
    stop = clock();
    diff = (double)((stop - start));
    // traverseSeqList(L1);
    cout << "bubbleSort finished"
         << ", cost " << diff << "ms" << endl;
    return diff;
}
//????????????
int partition(SeqList L1, int low, int high, bool cmp(Elemtype a, Elemtype b))
{
    //??????1????????????????return
    if (high <= low)
        return low;
    //?????????????????
    else if (high - low == 1)
    {
        if (!cmp(L1.pData[low], L1.pData[high]))
        {
            Elemtype temp;
            temp = L1.pData[low];
            L1.pData[low] = L1.pData[high];
            L1.pData[high] = temp;
        }
        return low;
    }
    else
    {
        //??????? begin
        int mid = (low + high) / 2;
        if (!cmp(L1.pData[low], L1.pData[high]))
        {
            Elemtype temp;
            temp = L1.pData[low];
            L1.pData[low] = L1.pData[high];
            L1.pData[high] = temp;
        }
        if (!cmp(L1.pData[mid], L1.pData[high]))
        {
            Elemtype temp;
            temp = L1.pData[mid];
            L1.pData[mid] = L1.pData[high];
            L1.pData[high] = temp;
        }
        if (!cmp(L1.pData[low], L1.pData[mid]))
        {
            Elemtype temp;
            temp = L1.pData[low];
            L1.pData[low] = L1.pData[mid];
            L1.pData[mid] = temp;
        }
        //??????? end

        L1.pData[0] = L1.pData[mid]; //???
        int privotKey = L1.pData[mid];
        Elemtype temp;
        temp = L1.pData[low + 1];
        L1.pData[low + 1] = L1.pData[mid];
        L1.pData[mid] = temp;
        //?????????low????????????????????????????????????????????????????????¡Ö??????????????
        //???low??????????????????????low??????????
        int i = low + 1, j = high - 1;

        while (i < j)
        {
            while (i < j && (cmp(privotKey, L1.pData[j]) || privotKey == L1.pData[j])) // i<j????????????????
                j--;
            //???????????????????????????
            L1.pData[i] = L1.pData[j];
            while (i < j && (cmp(L1.pData[i], privotKey) || privotKey == L1.pData[j]))
                i++;
            L1.pData[j] = L1.pData[i];
            //?????????????????????????
        }
        L1.pData[i] = L1.pData[0];
        return i;
    }
}
//?????????????
void QSort(SeqList &L1, int low, int high, bool cmp(Elemtype a, Elemtype b))
{
    if (low < high)
    {
        int pivotLoc = partition(L1, low, high, cmp);
        QSort(L1, low, pivotLoc - 1, cmp);
        QSort(L1, pivotLoc + 1, high, cmp);
    }
}
//????????
double quickSort(SeqList L, bool cmp(Elemtype a, Elemtype b))
{
    SeqList L1;
    initSeqList(L1);
    seqListCpy(L1, L);

    clock_t start, stop;
    double diff = 0;
    cout << "quickSort begin" << endl;
    start = clock();
    //
    QSort(L1, 1, L1.length, cmp);
    //
    stop = clock();
    diff = (double)((stop - start));
    // traverseSeqList(L1);
    cout << "quickSort finished"
         << ", cost " << diff << "ms" << endl;
    return diff;
}
//?????????
double selectSort(SeqList L, bool cmp(Elemtype a, Elemtype b))
{
    SeqList L1;
    initSeqList(L1);
    seqListCpy(L1, L);

    clock_t start, stop;
    double diff = 0;
    cout << "selectSort begin" << endl;
    start = clock();

    for (int i = 1; i <= L1.length - 1; i++)
    {
        int k = i;
        for (int j = i + 1; j <= L1.length; j++)
        {
            if (cmp(L1.pData[j], L1.pData[k]))
                k = j;
        }
        if (k != i)
        {
            Elemtype temp;
            temp = L1.pData[k];
            L1.pData[k] = L1.pData[i];
            L1.pData[i] = temp;
        }
    }

    stop = clock();
    diff = (double)((stop - start));
    // traverseSeqList(L1);
    cout << "selectSort finished"
         << ", cost " << diff << "ms" << endl;
    return diff;
}
//??????
void heapAdjust(SeqList L1, int low /*?????????*/, int high /*???????*/, bool cmp(Elemtype a, Elemtype b))
{
    //??????????
    //???¡¤???????????????????????????????????????????????0???
    Elemtype temp = L1.pData[low]; //?????
    int i = low, j = 2 * i;
    //????????????????????low????????????
    while (j <= high)
    {
        //?????????????????????????????
        //???????????????????????????
        if (j + 1 <= high && cmp(L1.pData[j], L1.pData[j + 1]))
            j++;
        //??????????????????????????????????????????
        if (cmp(L1.pData[j], temp))
            break;
        //???????????????????????
        L1.pData[i] = L1.pData[j];
        //??????????
        i = j;
        j = 2 * i;
    }
    L1.pData[i] = temp;
}
//??????
double heapSort(SeqList L, bool cmp(Elemtype a, Elemtype b))
{
    SeqList L1;
    initSeqList(L1);
    seqListCpy(L1, L);

    clock_t start, stop;
    double diff = 0;
    cout << "heapSort begin" << endl;
    start = clock();
    //
    //???????????????or?????
    for (int i = L1.length / 2; i > 0; i--) // i?????????????????????????????????????
        heapAdjust(L1, i, L1.length, cmp);
    for (int i = L1.length; i > 1; i--)
    {
        //?????????????????????????,??????????????????????
        Elemtype temp = L1.pData[1];
        L1.pData[1] = L1.pData[i];
        L1.pData[i] = temp;
        heapAdjust(L1, 1, i - 1, cmp);
    }
    //
    stop = clock();
    diff = (double)((stop - start));
    // traverseSeqList(L1);
    cout << "heapSort finished"
         << ", cost " << diff << "ms" << endl;
    return diff;
}
//????
void merge(SeqList &L1, int low /*??????????*/, int mid /*?????????*/, int high /*??????????*/, bool cmp(Elemtype a, Elemtype b))
{
    Elemtype *T = NULL;
    //????????????low??high??????????????
    T = (Elemtype *)malloc((high - low + 1) * sizeof(Elemtype));

    int i = low, j = mid + 1, k = 0;
    while (i <= mid && j <= high)
    {
        if (cmp(L1.pData[i], L1.pData[j]))
            T[k++] = L1.pData[i++];
        else
            T[k++] = L1.pData[j++];
    }
    //??????????????????????????????????????
    while (i <= mid)
        T[k++] = L1.pData[i++];
    while (j <= high)
        T[k++] = L1.pData[j++];

    for (k = 0, i = low; i <= high; k++, i++)
        L1.pData[i] = T[k];
}
//???????????
void MSort(SeqList &L1, int len, bool cmp(Elemtype a, Elemtype b))
{
    int i;
    //???????????len??????
    for (i = 1; i + 2 * len <= L1.length; i += 2 * len)
        merge(L1, i, i + len - 1, i + 2 * len - 1, cmp);

    //???????????????????????????????????????????????????????????????????????????????
    if (i + len <= L1.length)
        merge(L1, i, i + len - 1, L1.length, cmp);
}
//??????
double mergingSort(SeqList L, bool cmp(Elemtype a, Elemtype b))
{
    SeqList L1;
    initSeqList(L1);
    seqListCpy(L1, L);

    clock_t start, stop;
    double diff = 0;
    cout << "mergingSort begin" << endl;
    start = clock();
    //
    for (int len = 1; len < L.length; len *= 2)
        MSort(L1, len, cmp);
    //
    stop = clock();
    diff = (double)((stop - start));
    cout << "mergingSort finished"
         << ", cost " << diff << "ms" << endl;
    return diff;
}
//????????
double radixSort(SeqList L, int digit, bool cmp(Elemtype a, Elemtype b))
{
    LinkQueue Q[10];
    for (int i = 0; i < 10; i++)
        initQueue(Q[i]);

    LinkQueue q;
    initQueue(q);
    QueuePtr pQ = q.head;
    for (int i = 1; i <= L.length; i++)
    {
        LNode *node = (LNode *)malloc(sizeof(LNode));
        node->data = L.pData[i];
        pQ->next = node;
        pQ = pQ->next;
    }
    q.rear = pQ;
    pQ->next = NULL;

    clock_t start, stop;
    double diff = 0;
    cout << "radixSort begin" << endl;
    start = clock();

    int radix = 1;
    //???????????????????...
    for (int k = 1; k <= digit; k++)
    {
        radix *= 10;
        QueuePtr p = q.head->next;
        while (p)
        {
            int m = (p->data % radix) / (radix / 10);

            QueuePtr r = (LNode *)malloc(sizeof(LNode));
            r->data = p->data;
            r->next = NULL;
            Q[m].rear->next = r;
            Q[m].rear = r;

            p = p->next;
        }

        if (cmp(1, 2))
        {
            q.rear = q.head;
            for (int k = 0; k <= 9; k++)
            {
                if (Q[k].head->next)
                {
                    q.rear->next = Q[k].head->next;
                    q.rear = Q[k].rear;
                }
                Q[k].rear = Q[k].head;
                Q[k].rear->next = NULL;
            }
            q.rear->next = NULL;
        }
        else
        {
            q.rear = q.head;
            for (int k = 9; k >= 0; k--)
            {
                if (Q[k].head->next)
                {
                    q.rear->next = Q[k].head->next;
                    q.rear = Q[k].rear;
                }
                Q[k].rear = Q[k].head;
                Q[k].rear->next = NULL;
            }
            q.rear->next = NULL;
        }
    }
    //?????????????????,????????????????????
    stop = clock();
    diff = (double)((stop - start));
    LNode *p = q.head->next;
    // while (p)
    // {
    //     cout << p->data << " ";
    //     p = p->next;
    // }
    // cout << endl;
    cout
        << "radixSort finished"
        << ", cost " << diff << "ms" << endl;
    return diff;
}

bool cmp(pair<const char *, double> &p1, pair<const char *, double> &p2)
{
    return p1.second < p2.second;
}

int main()
{
    SeqList L;
    initSeqList(L);
    char fileName[] = "Sort1.txt";
    int d[4] = {7, 5, 3, 1}; //?????????????????????????1
    int digit = 5;

    int ope = 0;
    while (1)
    {
        cout << "?????????: 1.???? 2.??? " << endl;
        cin >> ope;
        if (ope == 1)
        {
            int num = 0;
            cout << "???????????: ";
            cin >> num;
            cout << "????" << num << endl;
            fileName[4] = num + '0';
            readFromFile(L, fileName);

            double t1 = insertSort(L, seqCmp),
                   t2 = ShellSort(L, d, 4, seqCmp),
                   t3 = bubbleSort(L, seqCmp),
                   t4 = quickSort(L, seqCmp),
                   t5 = selectSort(L, seqCmp),
                   t6 = heapSort(L, seqCmp),
                   t7 = mergingSort(L, seqCmp),
                   t8 = radixSort(L, digit, seqCmp);
            typedef pair<const char *, double> p;
            vector<p> vec;
            vec.push_back(make_pair("insertSort", t1));
            vec.push_back(make_pair("ShellSort", t2));
            vec.push_back(make_pair("bubbleSort", t3));
            vec.push_back(make_pair("quickSort", t4));
            vec.push_back(make_pair("selectSort", t5));
            vec.push_back(make_pair("heapSort", t6));
            vec.push_back(make_pair("mergingSort", t7));
            vec.push_back(make_pair("radixSort", t8));

            sort(vec.begin(), vec.end(), cmp);

            cout << "___________________________________________" << endl;
            cout << "Time Rank:" << endl;
            for (int i = 0; i < 8; i++)
                cout << vec[i].first << " : " << vec[i].second << "ms" << endl;
            cout << endl;
        }
        else
            break;
    }
}