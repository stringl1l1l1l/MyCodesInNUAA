#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <cstdio>
#include <string.h>
using namespace std;

const int MAXSIZE = 1000;
struct ListNode
{
    int data;
    int next;
};

struct StaticLinkList
{
    ListNode *List;
    int size;
    int end;       // 链表最后一个元素的位置，逻辑上的末尾
    int allocator; // 开辟新元素的位置，真正的数组尾
};

void initLinkList(StaticLinkList &L)
{
    // 创建头结点
    L.List = new ListNode[MAXSIZE];
    L.List[0].next = -1;
    L.size = 0;
    L.allocator = 1;
    L.end = 0;
}

void createLinkList(StaticLinkList &L, int size)
{
    L.size = size;
    L.end = size;
    L.allocator = size + 1;
    L.List[0].next = 1;
    printf("input numbers: ");
    while (size)
    {
        int index = L.size - size + 1;
        int elm;
        scanf("%d", &elm);
        L.List[index].data = elm;
        L.List[index].next = index + 1;
        size--;
    }
    L.List[L.size].next = -1;
}

void traveseLinkList(StaticLinkList &L)
{
    for (int i = 1; i != -1; i = L.List[i].next)
    {
        printf("%d", L.List[i].data);
        if (L.List[i].next != -1)
            printf(" --> ");
    }
}

int allocate(StaticLinkList &L)
{
    L.List[L.allocator].data = 0;
    L.size++;
    return L.allocator++;
}
void insert(StaticLinkList &L, int pos, int val)
{
    if (L.size == MAXSIZE)
    {
        printf("overflow!");
        return;
    }
    int insert = allocate(L);
    L.List[insert].data = val;
    // 找到待插入位置前一个结点
    int preInsert = 0;
    for (int i = 1; i < pos; i++)
    {
        preInsert = L.List[preInsert].next;
    }
    int afterInsert = L.List[preInsert].next;

    L.List[preInsert].next = insert;
    L.List[insert].next = afterInsert;
}

int main()
{
    StaticLinkList list;
    initLinkList(list);
    createLinkList(list, 5);
    insert(list, 2, 8);
    traveseLinkList(list);
}