#include <cstdio>
#include <cstdlib>


#define MAXSIZE 256
typedef int ElemType;
typedef struct Triple
{
    int row;
    int col;
    ElemType val;
} Triple;

typedef struct SqSMatrix
{
    Triple arr[MAXSIZE];
    int Rows, Cols, Nums;
} SqSMatrix;

void createSqSMatrix(SqSMatrix &M)
{
    M.Rows = 0;
    M.Cols = 0;
    scanf("%d", &M.Nums);

    for (int i = 0; i < M.Nums; i++)
    {
        scanf("%d %d %d", &M.arr[i].row, &M.arr[i].col, &M.arr[i].val);
        M.Rows = M.arr[i].row > M.Rows ? M.arr[i].row + 1 : M.Rows + 1;
        M.Cols = M.arr[i].col > M.Cols ? M.arr[i].col + 1 : M.Cols + 1;
    }
}

void destroyMatrix(SqSMatrix &M)
{
    if (M.Nums == 0)
        return;
    else
    {
        M.Rows = 0;
        M.Cols = 0;
        M.Nums = 0;
    }
}

void printMatrix(SqSMatrix M)
{
    if (M.Nums != 0)
    {
        for (int i = 0; i < M.Nums; i++)
            printf("%d %d %d\n", M.arr[i].row, M.arr[i].col, M.arr[i].val);
    }
    else
    {
        printf("Empty");
        return;
    }
}

void transposeSqSMatrix(SqSMatrix &A, SqSMatrix &B)
{
    if (A.Nums == 0)
        return;

    B.Rows = A.Rows;
    B.Nums = A.Nums;
    B.Cols = A.Cols;
    int cur = 0;
    for (int i = 0; i < A.Cols; i++)
    {
        for (int j = 0; j < A.Nums; j++)
        {
            if (A.arr[j].col == i)
            {
                B.arr[cur].row = A.arr[j].col;
                B.arr[cur].col = A.arr[j].row;
                B.arr[cur].val = A.arr[j].val;
                cur++;
            }
        }
    }
}

void rapidTransposeSqSMatrix(SqSMatrix &A, SqSMatrix &B)
{
    if (A.Nums == 0)
        return;

    B.Rows = A.Rows;
    B.Nums = A.Nums;
    B.Cols = A.Cols;
    int rowNum[MAXSIZE] = {0}, rowStart[MAXSIZE] = {0};

    for (int i = 0; i < A.Nums; i++)
        rowNum[A.arr[i].col]++;
    for (int i = 1; i < A.Cols; i++)
        rowStart[i] = rowNum[i - 1] + rowStart[i - 1];

    for (int i = 0; i < A.Nums; i++)
    {
        B.arr[rowStart[A.arr[i].col]].val = A.arr[i].val;
        B.arr[rowStart[A.arr[i].col]].row = A.arr[i].col;
        B.arr[rowStart[A.arr[i].col]++].col = A.arr[i].row;
    }
}

int main()
{
    SqSMatrix M, M2;
    createSqSMatrix(M);
    rapidTransposeSqSMatrix(M, M2);
    printMatrix(M2);
}