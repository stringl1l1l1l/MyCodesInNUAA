#include <cstdlib>
#include <cstdio>

typedef struct LNode
{
    int data;
    LNode *next;
} LNode, *QueuePtr;

typedef struct LinkQueue
{
    QueuePtr front;
    QueuePtr rear;

} LinkQueue;

//1.初始化、创建、销毁和清空
int initQueue(LinkQueue &Q)
{
    Q.front = NULL;
    Q.rear = NULL;

    Q.rear = Q.front;

    if (!Q.front)
        exit(-1);
    //防止内存申请失败

    Q.front->next = NULL;
    return 0;
}

void createQueue(LinkQueue &Q, int num, int elem[])
{
    if (!Q.front)
    {
        printf("Error:Initialize the queue first!");
        exit(-1);
    }

    if (Q.front->next)
    {
        printf("Error: The queue has existed!");
        exit(-1);
    }

    LNode *p = NULL, *pre_p = Q.front;
    for (int i = 0; i < num; i++)
    {
        p = (LNode *)malloc(sizeof(LNode));
        if (!p)
            exit(-1);
        //防止内存申请失败

        p->data = elem[i];
        pre_p->next = p;
        pre_p = p;
    }

    pre_p->next = NULL;
    Q.rear = pre_p;
}

void destoryQueue(LinkQueue &Q)
{
    if (!Q.front)
    {
        printf("Error:Initialize the queue first!");
        exit(-1);
    }

    LNode *p = NULL;
    while (Q.front->next)
    {
        p = Q.front->next;
        Q.front->next = p->next;
        free(p);
    }
    free(Q.front);
    Q.front = NULL;
    Q.rear = NULL;
    //free过后指针成为野指针，需要置为空指针
}

void clearQueue(LinkQueue &Q)
{
    QueuePtr p = Q.front;
    Q.front = Q.front->next;
    destoryQueue(Q);

    Q.front = p;
    Q.front->next = NULL;
    Q.rear = Q.front;
}

//2.访问型操作

bool isEmpty(LinkQueue &Q)
{
    return Q.front == Q.rear ? true : false;
}

int length(LinkQueue &Q)
{
    if (!Q.front)
    {
        printf("Error:Initialize the queue first!");
        exit(-1);
    }
    else if (isEmpty(Q))
        return 0;

    LNode *p = Q.front;
    int i = 0;
    while (p && p->next)
    {
        p = p->next;
        i++;
    }
    return i;
}

int traverseQueue(LinkQueue &Q)
{
    if (!Q.front)
    {
        printf("Error:Initialize the queue first!");
        return -1;
    }

    if (isEmpty(Q))
    {
        printf("Empty");
        return 0;
    }

    LNode *p = Q.front;
    int i = 0;
    while (p && p->next)
    {
        printf("%d ", p->next->data);
        p = p->next;
    }
    printf("\n");
    return 0;
}

int enQueue(LinkQueue &Q, int elem)
{
    if (!Q.front)
        return -1;

    QueuePtr p = (LNode *)malloc(sizeof(LNode));

    if (!p)
        exit(-1);
    //防止内存申请失败

    p->data = elem;
    p->next = NULL;
    Q.rear->next = p;
    Q.rear = p;
    return 0;
}

void deQueue(LinkQueue &Q)
{
    if (!Q.front)
    {
        printf("Error:Initialize the queue first!");
        exit(-1);
    }

    if (Q.front == Q.rear)
    {
        printf("Error:The queue is Empty!");
        return;
    }

    QueuePtr p = Q.front;
    Q.front = p->next;
    if (p == Q.rear)
        Q.rear = Q.front;
    //若将队尾删除，则为空表，队尾和队头重合
    free(p);
}

int getFront(LinkQueue &Q)
{
    if (!Q.front)
    {
        printf("Error:Initialize the queue first!");
        exit(-1);
    }

    if (Q.front == Q.rear)
    {
        printf("Empty");
        exit(0);
    }

    return Q.front->data;
}