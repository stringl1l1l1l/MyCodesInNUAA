#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <queue>
using namespace std;

typedef char ElemType;
typedef struct TreeNode
{
    ElemType data;
    TreeNode *firstChild, *nextBro;
} TreeNode, *Tree;

void initTree(Tree &T)
{
    T = (TreeNode *)malloc(sizeof(TreeNode));
    T->firstChild = NULL;
    T->nextBro = NULL;
}

void createTree(Tree &T, ElemType *elem)
{
    static int i = 0;
    static ElemType *p = NULL;
    if (i++ == 0)
        p = elem;
    if (elem[0] == '#')
    {
        T = NULL;
        p++;
        return;
    }
    else
    {
        T = (TreeNode *)malloc(sizeof(TreeNode));
        T->data = elem[0];
        p++;
        createTree(T->firstChild, p);
        createTree(T->nextBro, p);
    }
}

void recPreTraverse(Tree T)
{
    if (T)
    {
        cout << T->data << " ";
        recPreTraverse(T->firstChild);
        recPreTraverse(T->nextBro);
    }
}

void showGeneration(Tree &T, int i)
{
    if (T)
    {
        if (i == 1)
        {
            cout << T->data;
            return;
        }
        else
        {
            TreeNode *p = T, *pre_p = NULL;
            int count = 1;
            while (i - 1 != count)
            {
                while (p)
                {
                    if (i - 1 == count)
                        break;
                    pre_p = p;
                    p = p->firstChild;
                    count++;
                } //找到第i-1代结点
                //如果到底了代数不够，转向右兄弟
                if (!p)
                {
                    p = pre_p->nextBro;
                    count--;
                }
            }
            while (p)
            {
                if (p->firstChild)
                {
                    TreeNode *pCur = p->firstChild;
                    while (pCur)
                    {
                        cout << pCur->data << " ";
                        pCur = pCur->nextBro;
                    }
                }
                p = p->nextBro;
            }
        }
    }
}

void levelOrderTraverse(Tree T)
{
    if (T)
    {
        queue<TreeNode *> Q;

        Q.push(T);
        Q.push(NULL);
        TreeNode *curNode;
        while (!Q.empty())
        {
            curNode = Q.front();
            if (!Q.front())
            {
                cout << "| ";
                Q.pop();
                if (!Q.empty())
                    Q.push(NULL);
            }
            else
            {
                Q.pop();
                cout << curNode->data << " ";
                if (curNode->firstChild)
                    Q.push(curNode->firstChild);
                if (curNode->nextBro)
                    Q.push(curNode->nextBro);
            }
        }
        cout << endl;
    }
}
int main()
{
    TreeNode *T;
    char elem[30] = {'A', 'B', '#', 'C', '#', 'D', 'E', 'F', '#', '#', 'G', '#', '#', '#', '#'};
    initTree(T);
    createTree(T, elem);
    levelOrderTraverse(T);
    showGeneration(T, 3);
}