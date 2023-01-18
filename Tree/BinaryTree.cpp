#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stack>
#include <queue>
#include <math.h>

using namespace std;

typedef char ElemType;
typedef struct BinTreeNode
{
    ElemType data;
    BinTreeNode *left;
    BinTreeNode *right;
} BinTreeNode, *BinTree;

void initBinTree(BinTree &T)
{
    T = (BinTreeNode *)malloc(sizeof(BinTreeNode));
    T->left = NULL;
    T->right = NULL;
}

void createBinTree(BinTree &T, ElemType *elem)
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
        T = (BinTreeNode *)malloc(sizeof(BinTreeNode));
        T->data = elem[0];
        p++;
        createBinTree(T->left, p);
        createBinTree(T->right, p);
    }
}

BinTreeNode *destroyBinTree(BinTree &T)
{
    if (T)
    {
        T->left = destroyBinTree(T->left);
        T->right = destroyBinTree(T->right);
        free(T);
        T = NULL;
        return NULL;
    }
}

void clearBinTree(BinTree &T)
{
    if (T)
    {
        clearBinTree(T->left);
        clearBinTree(T->right);
        T->data = 0;
    }
}

bool isEmpty(BinTree &T)
{
    return T->left == NULL && T->right == NULL ? true : false;
}

int getBinTreeDeepth(BinTree &T)
{
    if (!T)
        return 0;
    int depLeft = getBinTreeDeepth(T->left) + 1;
    int depRight = getBinTreeDeepth(T->right) + 1;
    return (depLeft > depRight) ? depLeft : depRight;
}

ElemType getRoot(BinTree &T)
{
    if (T)
        return T->data;
}

ElemType getValue(BinTree &T, BinTreeNode *pCur)
{
    if (T)
        return pCur->data;
}

BinTreeNode *getParent(BinTree &T, BinTreeNode *pCur)
{
    if (T)
    {
        stack<BinTreeNode *> stk;
        stk.push(T);
        while (!stk.empty())
        {
            BinTreeNode *curNode = stk.top();
            if (curNode->left == pCur || curNode->right == pCur)
                return curNode;
            stk.pop();

            if (curNode->right)
                stk.push(curNode->right);

            if (curNode->left)
                stk.push(curNode->left);
        }
    }
}

BinTreeNode *getLeftChild(BinTree &T, BinTreeNode *pCur)
{
    if (T)
    {
        return pCur->left;
    }
}

BinTreeNode *getRightChild(BinTree &T, BinTreeNode *pCur)
{
    if (T)
    {
        return pCur->right;
    }
}

BinTreeNode *getLeftBrother(BinTree T, BinTreeNode *pCur)
{
    if (T)
    {
        queue<BinTreeNode *> Q;

        Q.push(T);
        Q.push(NULL);
        BinTreeNode *curNode = NULL, *last = NULL;
        while (Q.size() > 1)
        {
            curNode = Q.front();
            if (!Q.front())
            {
                Q.pop();
                Q.push(NULL);
                last = NULL;
            }
            else
            {
                if (curNode == pCur)
                    return last;
                Q.pop();
                last = curNode;
                if (curNode->left)
                    Q.push(curNode->left);
                if (curNode->right)
                    Q.push(curNode->right);
            }
        }
    }
}

BinTreeNode *getRightBrother(BinTree T, BinTreeNode *pCur)
{
    if (T)
    {
        queue<BinTreeNode *> Q;

        Q.push(T);
        Q.push(NULL);
        BinTreeNode *curNode = NULL;
        int flag = 0;
        while (Q.size() > 1)
        {
            curNode = Q.front();
            if (!Q.front())
            {
                if (flag == 1)
                    return NULL;
                Q.pop();
                Q.push(NULL);
            }
            else
            {
                Q.pop();
                if (flag == 1)
                    return curNode;
                if (curNode == pCur)
                    flag = 1;

                if (curNode->left)
                    Q.push(curNode->left);
                if (curNode->right)
                    Q.push(curNode->right);
            }
        }
    }
    return NULL;
}
void recPreOrderTraverse(BinTree T)
{
    if (T)
    {
        cout << T->data << " ";
        recPreOrderTraverse(T->left);
        recPreOrderTraverse(T->right);
    }
}

void preOrderTraverse(BinTree T)
{
    if (T)
    {
        stack<BinTreeNode *> stk;
        stk.push(T);
        while (!stk.empty())
        {
            BinTreeNode *curNode = stk.top();
            stk.pop();
            cout << curNode->data << " ";

            if (curNode->right)
                stk.push(curNode->right);

            if (curNode->left)
                stk.push(curNode->left);
        }
        cout << endl;
    }
}

void recInOrderTraverse(BinTree T)
{
    if (T)
    {
        recPreOrderTraverse(T->left);
        cout << T->data << " ";
        recPreOrderTraverse(T->right);
    }
}

void inOrderTraverse(BinTree T)
{
    if (T)
    {
        BinTreeNode *p = T;
        stack<BinTreeNode *> stk;
        while (p != NULL || !stk.empty())
        {
            if (p)
            {
                stk.push(p);
                p = p->left;
            }
            else
            {
                BinTreeNode *curNode = stk.top();
                stk.pop();
                cout << curNode->data << " ";
                p = curNode->right;
            }
        }
        cout << endl;
    }
}

void recPostOrderTraverse(BinTree T)
{
    if (T)
    {
        recPostOrderTraverse(T->left);
        recPostOrderTraverse(T->right);
        cout << T->data << " ";
    }
}

void postOrderTraverse(BinTree T)
{
    if (T)
    {
        BinTreeNode *p = T;
        stack<BinTreeNode *> stk;
        BinTreeNode *last = NULL;
        while (p != NULL || !stk.empty())
        {
            while (p)
            {
                stk.push(p);
                p = p->left;
            }

            BinTreeNode *curTop = stk.top();
            if (!curTop->right || curTop->right == last) //结点左右都为空或者右孩子已经被访问过
            {
                cout << curTop->data << " ";
                stk.pop();
                last = curTop;
            }
            else
                p = curTop->right;
        }
        cout << endl;
    }
}

void levelOrderTraverse(BinTree T)
{
    if (T)
    {
        queue<BinTreeNode *> Q;

        Q.push(T);
        Q.push(NULL);
        BinTreeNode *curNode;
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
                if (curNode->left)
                    Q.push(curNode->left);
                if (curNode->right)
                    Q.push(curNode->right);
            }
        }
        cout << endl;
    }
}

void assign(BinTree T, BinTreeNode *pCur, ElemType val)
{
    if (T)
        pCur->data = val;
}

void insertChild(BinTree &T, BinTreeNode *pCur, bool LR, BinTree &C)
{
    if (T)
    {
        if (LR == 0)
            pCur->left = C;
        else
            pCur->right = C;
    }
}

void deleteChild(BinTree &T, BinTreeNode *pCur, bool LR)
{
    if (T)
    {
        if (LR == 0)
            destroyBinTree(pCur->left);
        else
            destroyBinTree(pCur->right);
    }
}

//这是判断满二叉树，写错了
bool isCompleteBinTree(BinTree T)
{
    if (T)
    {
        if (T)
        {
            queue<BinTreeNode *> Q;
            Q.push(T);
            Q.push(NULL);
            BinTreeNode *curNode;
            int count = 0, level = 0;
            while (!Q.empty())
            {
                curNode = Q.front();
                if (!Q.front())
                {
                    Q.pop();
                    if (!Q.empty())
                        Q.push(NULL);
                    if (pow(2, level) != count)
                        return false;
                    level++;
                    count = 0;
                }
                else
                {
                    Q.pop();
                    count++;
                    if (curNode->left)
                        Q.push(curNode->left);
                    if (curNode->right)
                        Q.push(curNode->right);
                }
            }
        }
    }

    return true;
}

void deleteAllSubBinTree(BinTree &T, ElemType val) //不加引用会出错
{
    if (T)
    {
        if (T->data == val)
        {
            destroyBinTree(T);
            return;
        }

        if (T->left && T->left->data == val)
            destroyBinTree(T->left);
        else
            deleteAllSubBinTree(T->left, val);

        if (T->right && T->right->data == val)
            destroyBinTree(T->right);
        else
            deleteAllSubBinTree(T->right, val);
    }
}

int getWidth(BinTree &T)
{
    if (T)
    {
        queue<BinTreeNode *> Q;

        Q.push(T);
        Q.push(NULL);
        BinTreeNode *curNode;

        int maxWidth = 0, count = 0;
        while (!Q.empty())
        {
            curNode = Q.front();
            if (!Q.front())
            {
                if (count > maxWidth)
                    maxWidth = count;
                count = 0;
                Q.pop();
                if (!Q.empty())
                    Q.push(NULL);
            }
            else
            {
                Q.pop();
                count++;
                if (curNode->left)
                    Q.push(curNode->left);
                if (curNode->right)
                    Q.push(curNode->right);
            }
        }
        return maxWidth;
    }
    return 0;
}
int main()
{
    BinTree T;
    initBinTree(T);
    char elem1[20] = {'A', 'B', 'D', '#', 'G', '#', '#', '#', 'C', 'E', '#', '#', 'F', '#', '#'};
    char elem[20] = {'A', 'B', '#', 'C', 'F', '#', '#', 'D', 'G', '#', '#', 'H', '#', '#', '#'};
    char elem2[20] = {'A', 'B', 'D', '#', '#', 'E', '#', '#', 'C', 'B', '#', '#', 'G', '#', '#'};
    createBinTree(T, elem2);
    postOrderTraverse(T);
    recPostOrderTraverse(T);
    destroyBinTree(T);
}