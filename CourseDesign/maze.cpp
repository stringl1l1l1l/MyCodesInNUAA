#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <stack>
using namespace std;

#define MAX_ROW 100
#define MAX_COL 100
int rowNum = 0, colNum = 0;

typedef struct Position
{
    int row;
    int col;
    int flag;
} Position;

char **buildMaze(char *filename)
{
    fstream file(filename, ios::in);
    if (file.fail())
    {
        cout << "Cannot open file!" << endl;
        return NULL;
    }

    file >> rowNum >> colNum; //使用char变量读入数据，则会出现两位数作为两个字符出现的情况
    file.get();               //忽略一个回车
    char **matrix = new char *[rowNum];
    for (int i = 0; i < rowNum; i++)
    {
        matrix[i] = new char[colNum];
        for (int j = 0; j < colNum; j++)
            matrix[i][j] = 0;
    }

    char ch;
    int row = 0, col = 0;
    while (file.get(ch))
    {
        if (ch == ' ')
        {
            col++;
            continue;
        }
        else if (ch == '\n')
        {
            row++;
            col = 0;
            continue;
        }
        matrix[row][col] = ch;
    }
    file.close();
    return matrix;
}

Position judgeConflict(Position **maze, bool visited[][MAX_COL], Position last, Position cur)
{
    Position p;
    if (cur.col != 1 && visited[cur.row][cur.col - 1] == false)
    {
        p = maze[cur.row][cur.col - 1];
        if (!(p.row == last.row && p.col == last.col) && !p.flag)
            return p;
    }

    if (cur.col != colNum && visited[cur.row][cur.col + 1] == false)
    {
        p = maze[cur.row][cur.col + 1];
        if (!(p.row == last.row && p.col == last.col) && !p.flag)
            return p;
    }

    if (visited[cur.row - 1][cur.col] == false)
    {
        p = maze[cur.row - 1][cur.col];
        if (!(p.row == last.row && p.col == last.col) && !p.flag)
            return p;
    }
    if (visited[cur.row + 1][cur.col] == false)
    {
        p = maze[cur.row + 1][cur.col];
        if (!(p.row == last.row && p.col == last.col) && !p.flag)
            return p;
    }
    maze[cur.row][cur.col].flag = 1;
    return maze[cur.row][cur.col];
}

void find(Position **maze, stack<Position> &stk, bool visited[][MAX_COL], Position last, Position cur, Position end)
{
    if (cur.col != end.col || cur.row != end.row)
    {
        Position res = judgeConflict(maze, visited, last, cur);
        if (res.flag != 1)
        {
            last = cur;
            cur = res;
            stk.push(cur);
            visited[cur.row][cur.col] = true;
        }
        else if (!stk.empty())
        {
            stk.pop();
            if (!stk.empty())
            {
                cur = stk.top();
                stk.pop();
            }
            else
                return;
            last = stk.top();
            stk.push(cur);
        }
        find(maze, stk, visited, last, cur, end);
    }
    else
        return;
}

void findRoad(char **matrix, Position beg, Position end)
{
    //初始化操作 begin
    stack<Position> stk;
    bool visited[MAX_ROW][MAX_COL];
    for (int i = 0; i < MAX_ROW; i++)
        for (int j = 0; j < MAX_COL; j++)
            visited[i][j] = false;

    Position last = {100, 100, 0};
    Position **maze = (Position **)malloc(sizeof(Position *) * (rowNum + 2));
    for (int i = 0; i < rowNum + 2; i++)
        maze[i] = (Position *)malloc(sizeof(Position) * colNum);
    for (int i = 0; i < colNum; i++)
    {
        maze[0][i].row = 0;
        maze[0][i].col = i;
        maze[0][i].flag = 1;
        maze[rowNum + 1][i].row = rowNum + 1;
        maze[rowNum + 1][i].col = i;
        maze[rowNum + 1][i].flag = 1;
    }
    // 迷宫的最上一层与最下一层加一层墙壁

    stk.push(beg);
    visited[beg.row][beg.col] = true;
    for (int i = 1; i <= rowNum; i++)
    {
        for (int j = 1; j <= colNum; j++)
        {
            maze[i][j].row = i;
            maze[i][j].col = j;
            maze[i][j].flag = matrix[i - 1][j - 1] == '0' ? 0 : 1;
        }
    }
    //初始化操作 end
    //查找路径
    find(maze, stk, visited, last, beg, end);

    //显示路径
    while (!stk.empty())
    {
        Position cur = stk.top();
        matrix[cur.row - 1][cur.col - 1] = ' ';
        stk.pop();
    }
}

int main()
{
    char fileName[] = "D:\\Programming\\GitHub\\repository\\DataStruct\\CourseDesign\\.txt\\maze.txt";

    int x = 0, y = 0, n = 0, m = 0;
    while (1)
    {
        char **matrix = buildMaze(fileName);
        int num = 0;
        cout << "请输入操作: 1.继续 2.退出" << endl;
        cin >> num;
        if (num == 1)
        {
            cout << "请输入出发点坐标(从1开始): " << endl;
            cin >> x >> y >> m >> n;
            Position beg = {x, y, 0},
                     end = {m, n, 0};
            if (matrix[m - 1][n - 1] == '1')
            {
                cout << "No road" << endl;
                return -1;
            }
            findRoad(matrix, beg, end);
            for (int i = 0; i < rowNum; i++)
            {
                for (int j = 0; j < colNum; j++)
                    cout << matrix[i][j] << " ";
                cout << endl;
            }
        }
        else
            break;
    }
}