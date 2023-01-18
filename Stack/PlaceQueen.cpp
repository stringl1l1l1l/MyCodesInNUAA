#include "SeqStack.cpp"
#include <cstdio>
#include <cstdlib>
#include <math.h>

int outPutResult(SeqStack StkQueen)
{
    if (!StkQueen.pBase)
    {
        printf("Error:Initialize the stack first!");
        exit(-1);
    }

    int len = length(StkQueen);

    if (len == 0)
    {
        printf("Empty");
        return 0;
    }
    else
    {
        Position *p = StkQueen.pBase;

        while (p != StkQueen.pTop)
        {
            printf("(%d,%d) ", p->row, p->col);
            p++;
        }
        printf("\n");
        return 0;
    }
}

bool isPosConflicting(Position newQueen, SeqStack StkQueen)
{
    //判断已经在栈中的皇后是否会和新皇后冲突
    Elemtype *pCurQueen = StkQueen.pBase;
    int x1 = newQueen.row;
    int y1 = newQueen.col;

    while (pCurQueen != StkQueen.pTop)
    {
        int x2 = pCurQueen->row;
        int y2 = pCurQueen->col;
        if (x1 == x2 || y1 == y2)
            return true;
        //同一列或同一行

        //千万不要写成 abs((y1 - y2) / (x1 - x2)) == 1，因为整型数相除会舍去小数点，如5/4==1
        if (abs(y1 - y2) == abs(x1 - x2))
            return true;
        //同一斜线(斜率为1or-1)
        pCurQueen++;
    }
    return false;
}

int placeQueen(int N)
{
    SeqStack StkQueen;
    initSeqStack(StkQueen);
    int resultCount = 0, count = 1;

    Position curQueen;
    curQueen.row = 1;
    curQueen.col = 1;
    push(StkQueen, curQueen);

    curQueen.row = 2;
    curQueen.col = 1;

    while (1) //输出循环
    {
        if (curQueen.row == 1 && curQueen.col > N)
        {
            curQueen.col = ++count;
            push(StkQueen, curQueen);
            curQueen.row = 2;
            curQueen.col = 1;
            break;
        }

        while (curQueen.row <= N && curQueen.col <= N) //一个解的循环，退出该循环的条件为 空栈或栈长为N
        {
            while (curQueen.col <= N) //每行判断循环
            {
                if (isPosConflicting(curQueen, StkQueen)) //冲突则进行下一轮判断循环
                {
                    curQueen.col++;
                    continue;
                }
                else //存在不冲突的位置则入栈
                {
                    push(StkQueen, curQueen);
                    curQueen.row++;
                    curQueen.col = 1;
                    break;
                }
            }

            if (!isEmpty(StkQueen) && length(StkQueen) == N) //入栈后栈长为N则做一次输出，并弹栈，继续遍历下一位置
            {
                outPutResult(StkQueen);
                resultCount++;
                curQueen.row = N;
                curQueen.col = getTop(StkQueen).col + 1;
                pop(StkQueen);
            }

            while (curQueen.col > N && !isEmpty(StkQueen)) //若该行没有不冲突的位置，则一直回退到不会越界的位置
            {
                curQueen.row = getTop(StkQueen).row;
                curQueen.col = getTop(StkQueen).col + 1;
                pop(StkQueen);
            }
        }
    }

    destroySeqStack(StkQueen);
    return resultCount;
}

int main()
{
    printf("%d", placeQueen(8));
}