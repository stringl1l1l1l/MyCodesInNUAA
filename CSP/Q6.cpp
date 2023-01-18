#include <iostream>
#include <cstdlib>

using namespace std;

typedef struct Position
{
    int x;
    int y;
} Position;

typedef struct Window
{
    int num;
    Position bottomLeft;
    Position topRight;
} Window;

typedef struct Node
{
    Window win;
    Node *next;
} Node, *WindowList;

int insertToTop(WindowList &L, Position click)
{
    Node *pre_p = L, *p = pre_p->next;
    Node *topNode = NULL, *pre_topNode = NULL;
    while (p)
    {
        if (click.x >= p->win.bottomLeft.x && click.y >= p->win.bottomLeft.y &&
            click.x <= p->win.topRight.x && click.y <= p->win.topRight.y)
        {
            topNode = p;
            pre_topNode = pre_p;
        }
        pre_p = pre_p->next;
        p = pre_p->next;
    }

    if (pre_topNode && topNode)
    {
        pre_p->next = topNode;
        pre_topNode->next = topNode->next;
        topNode->next = NULL;
        return topNode->win.num;
    }
    else
        return -1;
}

int main()
{
    int N = 0, M = 0;
    cin >> N >> M;

    Node *head = (Node *)malloc(sizeof(Node));
    head->next = NULL;
    //初始化链表
    Node *p = head;
    for (int i = 0; i < N; i++)
    {
        Node *node = (Node *)malloc(sizeof(Node));
        node->win.num = i + 1;
        cin >> node->win.bottomLeft.x >> node->win.bottomLeft.y >>
            node->win.topRight.x >> node->win.topRight.y;
        p->next = node;
        p = p->next;
    }
    p->next = NULL;

    Position click[11] = {{0, 0}};
    for (int i = 0; i < M; i++)
        cin >> click[i].x >> click[i].y;
    //产生点击事件的坐标顺序表

    for (int i = 0; i < M; i++)
    {
        int res = insertToTop(head, click[i]);
        if (res == -1)
            cout << "IGNORED" << endl;
        else
            cout << res << endl;
    }

    return 0;
}