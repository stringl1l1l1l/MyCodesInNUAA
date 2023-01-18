#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <queue>
using namespace std;

#define FILE_NAME "D:\\Programming\\GitHub\\repository\\DataStruct\\CourseDesign\\.txt\\AVLTree.txt"
typedef struct AVLTreeNode
{
    int balVal;
    int data;
    AVLTreeNode *parent, *left, *right;
} AVLTreeNode, *AVLTree;

void initAVLTree(AVLTree &T)
{
    T = new AVLTreeNode;
    T->balVal = 0;
    T->data = -1;
    T->parent = T->right = T->left = NULL;
}

int getDepth(AVLTree &T)
{
    if (!T)
        return 0;
    else
    {
        int d1 = getDepth(T->left) + 1,
            d2 = getDepth(T->right) + 1;
        return d1 >= d2 ? d1 : d2;
    }
}

AVLTreeNode *leftRotate(AVLTree &T, AVLTreeNode *curRoot)
{
    AVLTreeNode *newRoot = curRoot->right;
    curRoot->right = newRoot->left;
    if (newRoot->left)
        newRoot->left->parent = curRoot;
    newRoot->left = curRoot;
    curRoot->parent = newRoot;
    int d1 = getDepth(curRoot->left),
        d2 = getDepth(curRoot->right),
        d3 = getDepth(newRoot->left),
        d4 = getDepth(newRoot->right);
    curRoot->balVal = d2 - d1;
    newRoot->balVal = d4 - d3;
    newRoot->parent = NULL;
    return newRoot;
}

AVLTreeNode *rightRotate(AVLTree &T, AVLTreeNode *curRoot)
{
    AVLTreeNode *newRoot = curRoot->left;
    curRoot->left = newRoot->right;
    if (newRoot->right)
        newRoot->right->parent = curRoot;
    newRoot->right = curRoot;
    curRoot->parent = newRoot;
    int d1 = getDepth(curRoot->left),
        d2 = getDepth(curRoot->right),
        d3 = getDepth(newRoot->left),
        d4 = getDepth(newRoot->right);
    curRoot->balVal = d2 - d1;
    newRoot->balVal = d4 - d3;
    newRoot->parent = NULL;
    return newRoot;
}

void changeBalVal(AVLTree &T, AVLTreeNode *curNode)
{
    AVLTreeNode *parPtr = curNode;
    while (parPtr)
    {
        int d1 = getDepth(parPtr->left),
            d2 = getDepth(parPtr->right);
        parPtr->balVal = d2 - d1;
        parPtr = parPtr->parent;
    }
}

void adjustBalance(AVLTree &T, AVLTreeNode *curNode)
{
    AVLTreeNode *p = curNode;
    while (p)
    {
        if (abs(p->balVal) == 2)
            break;
        p = p->parent;
    }
    if (!p)
        return;
    else
    {
        if (p->balVal == 2 && p->right->balVal >= 0) //RR
        {
            AVLTreeNode *parPtr = p->parent,
                        *newChild = leftRotate(T, p);
            if (!parPtr)
            {
                T = newChild;
                return;
            }
            if (newChild->data < parPtr->data)
                parPtr->left = newChild;
            else
                parPtr->right = newChild;
            newChild->parent = parPtr;
        }
        else if (p->balVal == 2 && p->right->balVal <= 0) //RL
        {
            AVLTreeNode *parPtr = p->parent,
                        *newChild = rightRotate(T, p->right);
            p->right = newChild;
            newChild->parent = p;
            newChild = leftRotate(T, p);
            if (!parPtr)
            {
                T = newChild;
                return;
            }
            if (newChild->data < parPtr->data)
                parPtr->left = newChild;
            else
                parPtr->right = newChild;
            newChild->parent = parPtr;
        }
        else if (p->balVal == -2 && p->left->balVal >= 0) //LR
        {
            AVLTreeNode *parPtr = p->parent,
                        *newChild = leftRotate(T, p->left);
            p->left = newChild;
            newChild->parent = p;
            newChild = rightRotate(T, p);
            if (!parPtr)
            {
                T = newChild;
                return;
            }
            if (newChild->data < parPtr->data)
                parPtr->left = newChild;
            else
                parPtr->right = newChild;
            newChild->parent = parPtr;
        }
        else if (p->balVal == -2 && p->left->balVal <= 0) //LL
        {
            AVLTreeNode *parPtr = p->parent,
                        *newChild = rightRotate(T, p);
            if (!parPtr)
            {
                T = newChild;
                return;
            }
            if (newChild->data < parPtr->data)
                parPtr->left = newChild;
            else
                parPtr->right = newChild;
            newChild->parent = parPtr;
        }
    }
}

AVLTreeNode *insert(AVLTree &T, int elem)
{
    if (!T)
    {
        initAVLTree(T);
        T->data = elem;
        return T;
    }
    else if (T->data == -1)
    {
        T->data = elem;
        return T;
    }

    AVLTreeNode *p = T, *pre = NULL;
    while (p)
    {
        if (elem == p->data)
            return p;
        else if (elem < p->data)
        {
            pre = p;
            p = p->left;
        }
        else
        {
            pre = p;
            p = p->right;
        }
    }
    p = new AVLTreeNode;
    p->balVal = 0;
    p->data = elem;
    p->left = p->right = NULL;
    if (elem > pre->data)
        pre->right = p;
    else
        pre->left = p;
    p->parent = pre;
    changeBalVal(T, pre); //调整平衡因子
    adjustBalance(T, p);  //调整为AVL树
    return p;
}

AVLTreeNode *findElem(AVLTree &T, int elem)
{
    AVLTreeNode *p = T;
    while (p)
    {
        if (p->data == elem)
            return p;
        else if (elem < p->data)
            p = p->left;
        else
            p = p->right;
    }
    if (!p)
    {
        AVLTreeNode *p = insert(T, elem);
        return p;
    }
}

void levelOrderTraverse(AVLTree &T)
{
    if (T)
    {
        queue<AVLTreeNode *> Q;

        Q.push(T);
        Q.push(NULL);
        AVLTreeNode *curNode;
        while (!Q.empty())
        {
            curNode = Q.front();
            if (!Q.front())
            {
                cout << "\n";
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

void deleteElem(AVLTree &T, int elem)
{
    AVLTreeNode *p = findElem(T, elem);
    if (!p->left && !p->right) //叶结点
    {
        if (p->parent)
        {
            if (p->data < p->parent->data)
                p->parent->left = NULL;
            else
                p->parent->right = NULL;
            changeBalVal(T, p->parent);
            adjustBalance(T, p->parent);
        }
        else
            T = NULL;
        free(p);
    }
    else if (!p->left && p->right) //只有右孩子
    {
        if (p->parent)
        {
            if (p->data < p->parent->data)
            {
                p->right->parent = p->parent;
                p->parent->left = p->right;
            }
            else
            {
                p->right->parent = p->parent;
                p->parent->left = p->right;
            }
            changeBalVal(T, p->parent);
            adjustBalance(T, p->parent);
        }
        else
        {
            T = p->right;
            T->parent = NULL;
            T->balVal = 0;
        }
        free(p);
    }
    else if (p->left && !p->right) //只有左孩子
    {
        if (p->parent)
        {
            if (p->data < p->parent->data)
                p->parent->left = p->left;
            else
                p->parent->left = p->left;
            changeBalVal(T, p->parent);
            adjustBalance(T, p->parent);
        }
        else
        {
            T = p->left;
            T->parent = NULL;
            T->balVal = 0;
        }
        free(p);
    }
    else //左右都有
    {
        AVLTreeNode *q = p->left;
        while (q->right)
            q = q->right;
        p->data = q->data;
        if (!q->left && !q->right)
        {
            if (q->data < q->parent->data)
                q->parent->left = NULL;
            else
                q->parent->right = NULL;
        }
        else
        {
            if (q->data < q->parent->data)
            {
                q->left->parent = q->parent;
                q->parent->left = q->left;
            }
            else
            {
                q->left->parent = q->parent;
                q->parent->right = q->left;
            }
        }
        changeBalVal(T, q->parent);
        adjustBalance(T, q->parent);
        free(q);
    }
}

void buildTreeFromFile(AVLTree &T)
{

    fstream file(FILE_NAME, ios::in);
    if (file.fail())
    {
        cout << "Cannot open file!" << endl;
        exit(-1);
    }

    int num;
    while (file >> num)
        insert(T, num);
    file.close();
}

int main()
{
    AVLTree T;
    initAVLTree(T);
    buildTreeFromFile(T);
    cout << "创建完毕" << endl;
    cout << "请输入您需要的操作: " << endl;
    while (1)
    {
        int num = 0;
        cout << "1.插入 2.删除 3.遍历 4.退出  ";
        cin >> num;
        if (num == 1)
        {
            int elem;
            cout << "输入待插入元素: " << endl;
            cin >> elem;
            insert(T, elem);
        }
        else if (num == 2)
        {
            int elem;
            cout << "输入待删除元素: " << endl;
            cin >> elem;
            deleteElem(T, elem);
        }
        else if (num == 3)
            levelOrderTraverse(T);
        else
            break;
    }
    // deleteElem(T, 60);
    // levelOrderTraverse(T);
    // insert(T, 60);
    // insert(T, 64);
    // levelOrderTraverse(T);
    // deleteElem(T, 55);
    // levelOrderTraverse(T);
    // deleteElem(T, 50);
    // levelOrderTraverse(T);
}