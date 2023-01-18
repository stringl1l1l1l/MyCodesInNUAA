
#include <iostream>
#include <cstdio>
using namespace std;
typedef struct Location
{
    int row;
    int col;
} Location;

int main()
{
    bool chart[16][10];
    for (int i = 0; i < 15; i++)
        for (int j = 0; j < 10; j++)
            scanf("%d", &chart[i][j]);

    for (int j = 0; j < 10; j++)
        chart[15][j] = 1;

    bool pattern[4][4];
    Location loc[4];
    int num = 0, minRow = 4;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            scanf("%d", &pattern[i][j]);
            if (pattern[i][j] == 1)
            {
                loc[num].row = i;
                loc[num++].col = j;
            }
        }
    }

    int m = 0;
    scanf("%d", &m);


    int i = 0, j = 0, flag = 0;
    for (i = 0; i < 16; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (chart[loc[j].row + i][loc[j].col + m - 1] == 1)
            {
                flag = 1;
                break;
            }
        }
        if (flag == 1)
            break;
    }

    if (flag == 1)
    {
        for (int k = 0; k < 4; k++)
            chart[loc[k].row + i - 1][loc[k].col + m - 1] = 1;
    }

    for (int l = 0; l < 15; l++)
    {
        for (int n = 0; n < 10; n++)
            printf("%d ", chart[l][n]);
        printf("\n");
    }
}