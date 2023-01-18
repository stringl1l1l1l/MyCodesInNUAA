#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <queue>
using namespace std;

typedef int ElemType;
typedef struct BinTreeNode
{
    ElemType data;
    BinTreeNode *left;
    BinTreeNode *right;
    int weight;
} BinTreeNode, *BinTree;

typedef struct HuffNode
{
    int weight;
    int parent, left, right;
} HuffNode;

void buildHuffTreeByTable(BinTree &huffTree, HuffNode *table)
{
    if (table[huffTree->data].left != -1)
    {
        huffTree->left = (BinTreeNode *)malloc(sizeof(BinTreeNode));
        huffTree->left->data = table[huffTree->data].left;
        huffTree->left->weight = table[table[huffTree->data].left].weight;

        huffTree->right = (BinTreeNode *)malloc(sizeof(BinTreeNode));
        huffTree->right->data = table[huffTree->data].right;
        huffTree->right->weight = table[table[huffTree->data].right].weight;

        buildHuffTreeByTable(huffTree->left, table);
        buildHuffTreeByTable(huffTree->right, table);
    }
    else
    {
        huffTree->left = NULL;
        huffTree->right = NULL;
    }
}

void createHuffmanTree(BinTree &huffTree, int N, int *weight)
{
    HuffNode* table = (HuffNode*)malloc(sizeof(HuffNode) * (2 * N - 1));
    //初始化操作
    for (int i = 0; i < 2 * N - 1; i++)
    {
        table[i].left = -1;
        table[i].parent = -1;
        table[i].right = -1;
        table[i].weight = -1;
    }

    for (int i = 0; i < N; i++)
        table[i].weight = weight[i];

    int count = N; //记录当前树的个数
    while (count < 2 * N - 1)
    {
        int minIndex = -1, secMinIndex = -1, min = 100, secMin = 100;
        for (int i = 0; i < count; i++)
        {
            //找到当前未访问树中的权值最小值与次小值对应的树
            if (table[i].parent == -1)
            {
                if (table[i].weight < min)
                {
                    secMinIndex = minIndex;
                    secMin = min;
                    minIndex = i;
                    min = table[i].weight;
                }
                else
                {
                    if (table[i].weight < secMin)
                    {
                        secMinIndex = i;
                        secMin = table[i].weight;
                    }
                }
            }
        } //若最小值与次小值相等，则将序号靠前的树作为左子树（即次小值对应的树）
        if (table[secMinIndex].weight == table[minIndex].weight)
        {
            table[count].left = minIndex;
            table[count].right = secMinIndex;
        }
        //否则最小值对应的树作为左子树
        else
        {
            table[count].left = minIndex;
            table[count].right = secMinIndex;
        }
        table[minIndex].parent = table[secMinIndex].parent = count;
        table[count].weight = table[minIndex].weight + table[secMinIndex].weight;
        count++;
    }

    huffTree = (BinTreeNode *)malloc(sizeof(BinTreeNode));
    for (int i = 0; i < 2 * N - 1; i++)
    {
        if (table[i].parent == -1)
        {
            huffTree->data = i;
            huffTree->weight = table[i].weight;
            break;
        }
    }
    buildHuffTreeByTable(huffTree, table);
}

void levelOrderTraverse(BinTree T)
{
    if (T)
    {
        queue<BinTreeNode *> Q;

        Q.push(T);
        Q.push(NULL);
        BinTreeNode *curNode;
        while (Q.size() > 1)
        {
            curNode = Q.front();
            if (!Q.front())
            {
                cout << "| ";
                Q.pop();
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

int main()
{
    BinTree huffTree;
    int weight[5] = {10, 15, 12, 3, 4};
    createHuffmanTree(huffTree, 5, weight);
    levelOrderTraverse(huffTree);
}