
#include <cstdio>
#include <cstdlib>

#define INITSTACKSIZE 256
#define INCRECEMENT 128

typedef struct Position
{
    int row;
    int col;
} Position;

typedef Position Elemtype;

typedef struct SeqStack
{
    Elemtype *pBase;
    Elemtype *pTop;
    int stackSize;
} SeqStack;

void initSeqStack(SeqStack &S)
{
    S.pBase = NULL;
    S.pTop = NULL;
    S.pBase = (Elemtype *)malloc(sizeof(Elemtype) * INITSTACKSIZE);
    S.pTop = S.pBase;
    S.stackSize = INITSTACKSIZE;
}

void createSeqStack(SeqStack &S, int len, Elemtype elem[])
{
    if (len <= 0)
    {
        printf("Erorr:Illegal parameter!");
        exit(-1);
    }

    while (len > S.stackSize)
    {
        S.pBase = (Elemtype *)realloc(S.pBase, sizeof(Elemtype) * (S.stackSize + INCRECEMENT));
        S.stackSize += INCRECEMENT;
    }

    for (int i = 0; i < len; i++)
        S.pBase[i] = elem[i];
    S.pTop = S.pBase + len;
}

void destroySeqStack(SeqStack &S)
{
    if (S.pBase)
    {
        free(S.pBase);
        S.pBase = NULL;
    }

    S.pTop = S.pBase;
    S.stackSize = 0;
}

void clearSeqStack(SeqStack &S)
{
    S.pTop = S.pBase;
}

bool isEmpty(SeqStack &S)
{
    return S.pTop == S.pBase ? true : false;
}

int length(SeqStack &S)
{
    return S.pTop - S.pBase;
}

Elemtype getTop(SeqStack &S)
{
    if (!S.pBase || isEmpty(S))
        exit(-1);

    return *(S.pTop - 1);
}

int traverseSeqStack(SeqStack &S)
{
    if (!S.pBase)
    {
        printf("Error:Initialize the stack first!");
        exit(-1);
    }

    int len = length(S);
    if (len == 0)
    {
        printf("Empty\n");
        return 0;
    }

    for (Elemtype *p = S.pBase; p != S.pTop; p++)
        printf("%d ", *p);
    printf("\n");

    return 0;
}

int push(SeqStack &S, Elemtype elem)
{
    if (!S.pBase)
    {
        printf("Error:Initialize the stack first!");
        exit(0);
    }

    int len = length(S);
    if (len == S.stackSize)
    {
        S.pBase = (Elemtype *)realloc(S.pBase, sizeof(Elemtype) * (S.stackSize + INCRECEMENT));
        if (!S.pBase)
            exit(-1);
        S.pTop = S.pBase + S.stackSize;
        S.stackSize += INCRECEMENT;
    }

    *S.pTop = elem;
    S.pTop++;
    return 0;
}

Elemtype pop(SeqStack &S)
{
    if (!S.pBase)
    {
        printf("Error:Initialize the stack first!");
        exit(-1);
    }

    if (isEmpty(S))
    {
        printf("Error:Empty stack!");
        exit(-1);
    }

    return *(--S.pTop);
}
