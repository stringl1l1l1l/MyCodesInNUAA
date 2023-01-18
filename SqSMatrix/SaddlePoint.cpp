#include <cstdio>
#include <cstdlib>
int main()
{
    int m, n;
    scanf("%d%d", &m, &n);

    int **matrix = (int **)malloc(sizeof(*matrix) * n);
    for (int i = 0; i < n; i++)
        matrix[i] = (int *)malloc(sizeof(int) * m);

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            scanf("%d", &matrix[i][j]);
        }
        getchar();
    }

    int rowMin[100] = {0}, colMax[100] = {0};
    int max = 0, min = 0;

    for (int i = 0; i < m; i++)
    {
        min = matrix[i][0];
        for (int j = 0; j < n; j++)
            min = min > matrix[i][j] ? matrix[i][j] : min;
        rowMin[i] = min;
    }

    for (int j = 0; j < n; j++)
    {
        max = matrix[0][j];
        for (int i = 0; i < m; i++)
            max = max < matrix[i][j] ? matrix[i][j] : max;
        colMax[j] = max;
    }

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (colMax[j] == rowMin[i])
            {
                printf("(%d,%d,%d)", i, j, matrix[i][j]);
                break;
            }
        }
    }
}
