/*问题描述：请实现一个铁路购票系统的简单座位分配算法，来处理一节车厢的座位分配。
假设一节车厢有20排、每一排5个座位。
为方便起见，我们用1到100来给所有的座位编号，第一排是1到5号，第二排是6到10号，依次类推，第20排是96到100号。
购票时，一个人可能购一张或多张票，最多不超过5张。
如果这几张票可以安排在同一排编号相邻的座位，则应该安排在编号最小的相邻座位。
否则应该安排在编号最小的几个空座位中（不考虑是否相邻）。
假设初始时车票全部未被购买，现在给了一些购票指令，请你处理这些指令。
输入格式：
对于所有评测用例，1 ≤ n ≤ 100，所有购票数量之和不超过100。
输入的第一行包含一个整数n，表示购票指令的数量。
第二行包含n个整数，每个整数p在1到5之间，表示要购入的票数，相邻的两个数之间使用一个空格分隔。
输出格式：
输出n行，每行对应一条指令的处理结果。
对于购票指令p，输出p张车票的编号，按从小到大排序。
问题分析：
这个问题可以用顺序结构或链式结构实现。
样例输入：
4
2 5 4 2
样例输出：
1 2
6 7 8 9 10
11 12 13 14
3 4
*/
#include <cstdio>

int main()
{
    bool seat[20][5];
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 5; j++)
            seat[i][j] = 0;
    }

    int n = 0, num[101] = {0}; //多一个位置用作结束标记
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
        scanf("%d", &num[i]);

    int k = 0;
    while (num[k] != 0)
    {
        bool(*pRow)[5] = seat;
        bool *pCol = pRow[0];
        while (num[k] != 0 && num[k] != 1 && pRow - seat < 20 && pCol - pRow[0] < 5)
        {
            if (pCol - pRow[0] + num[k] <= 5 && *pCol == 0)
            {
                int i = 0;
                for (i = 0; i < num[k]; i++)
                {
                    if (*(pCol + i) == 1)
                        break;
                } //遍历，看本行是否有满足条件的座位
                if (i == num[k])
                {
                    for (i = 0; i < num[k]; i++)
                    {
                        *(pCol + i) = 1;
                        printf("%d ", (pRow - seat) * 5 + pCol - pRow[0] + i + 1);
                    }
                    if (num[k + 1] != 0)
                        printf("\n");
                    k++;
                    pRow = seat;
                    pCol = pRow[0];
                } //若有则将数组对应位置置为1，进行下一次买票指令
            }
            else if (pCol - pRow[0] + num[k] > 5)
            {
                pRow++;
                pCol = pRow[0];
            } //若本行没有合适座位，则查找下一行
            else
                pCol++;
            //若当前位置已买，则查找本行下一位置
        }

        //若没有相邻的合适位置
        if (pRow - seat == 20 || num[k] == 1)
        {
            int m = 1, count = 0;
            for (int row = 0; row < 20; row++)
            {
                for (int col = 0; col < 5; col++)
                {
                    if (seat[row][col] == 0)
                    {
                        seat[row][col] = 1;
                        printf("%d ", row * 5 + col + 1);
                        count++;
                    }
                    if (count == num[k])
                        break;
                }

                if (count == num[k])
                {
                    if (num[k + 1] != 0)
                        printf("\n");
                    k++;
                    break;
                }
            }
        }
    }
    return 0;
}