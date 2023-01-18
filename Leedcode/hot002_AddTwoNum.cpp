#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

struct ListNode
{
    int val;
    ListNode *next;
    ListNode() : val(0), next(NULL) {}
    ListNode(int x) : val(x), next(NULL) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution
{
public:
    ListNode *addTwoNumbers(ListNode *l1, ListNode *l2)
    {
        ListNode *pL1 = l1->next, *pL2 = l2->next;
        ListNode *l3 = new ListNode;
        ListNode *pL3 = l3;
        ListNode *node = new ListNode;
        pL3->next = node;
        int lastCin = 0, curCin = 0;
        while (pL1 && pL2)
        {
            pL3 = pL3->next;
            int digit = pL1->val + pL2->val + lastCin;
            if (digit >= 10)
            {
                curCin = 1;
                digit -= 10;
            }
            else
                curCin = 0;
            pL3->val = digit;
            pL1 = pL1->next;
            pL2 = pL2->next;
            ListNode *node = new ListNode;
            node->next = NULL;
            node->val = curCin;
            pL3->next = node;
            lastCin = curCin;
        }
        while (pL1)
        {
            pL3 = pL3->next;
            int digit = pL1->val + lastCin;
            if (digit >= 10)
            {
                curCin = 1;
                digit -= 10;
            }
            else
                curCin = 0;
            pL1 = pL1->next;
            pL3->val = digit;
            ListNode *node = new ListNode;
            node->next = NULL;
            node->val = curCin;
            pL3->next = node;
            lastCin = curCin;
        }
        while (pL2)
        {
            pL3 = pL3->next;
            int digit = pL2->val + lastCin;
            if (digit >= 10)
            {
                curCin = 1;
                digit -= 10;
            }
            else
                curCin = 0;
            pL2 = pL2->next;
            pL3->val = digit;
            ListNode *node = new ListNode;
            node->next = NULL;
            node->val = curCin;
            pL3->next = node;
            lastCin = curCin;
        }
        if (pL3->next->val == 0)
        {
            delete(pL3->next);
            pL3->next = NULL;
        }
        return l3;
    }
};

// class Solution
// {
// public:
//     ListNode *addTwoNumbers(ListNode *l1, ListNode *l2)
//     {
//         ListNode *pL1 = l1, *pL2 = l2;
//         ListNode *l3 = new ListNode;
//         ListNode *pL3 = l3, *prepL3 = l3;
//         int lastCin = 0, curCin = 0;
//         while (pL1 && pL2)
//         {
//             int digit = pL1->val + pL2->val + lastCin;
//             if (digit >= 10)
//             {
//                 curCin = 1;
//                 digit -= 10;
//             }
//             else
//                 curCin = 0;
//             pL3->val = digit;
//             pL1 = pL1->next;
//             pL2 = pL2->next;
//             ListNode *node = new ListNode;
//             node->next = nullptr;
//             node->val = curCin;
//             pL3->next = node;
//             prepL3 = pL3;
//             pL3 = pL3->next;
//             lastCin = curCin;
//         }
//         while (pL1)
//         {
//             int digit = pL1->val + lastCin;
//             if (digit >= 10)
//             {
//                 curCin = 1;
//                 digit -= 10;
//             }
//             else
//                 curCin = 0;
//             pL1 = pL1->next;
//             pL3->val = digit;
//             ListNode *node = new ListNode;
//             node->next = nullptr;
//             node->val = curCin;
//             pL3->next = node;
//             prepL3 = pL3;
//             pL3 = pL3->next;
//             lastCin = curCin;
//         }
//         while (pL2)
//         {
//             int digit = pL2->val + lastCin;
//             if (digit >= 10)
//             {
//                 curCin = 1;
//                 digit -= 10;
//             }
//             else
//                 curCin = 0;
//             pL2 = pL2->next;
//             pL3->val = digit;
//             ListNode *node = new ListNode;
//             node->next = nullptr;
//             node->val = curCin;
//             pL3->next = node;
//             prepL3 = pL3;
//             pL3 = pL3->next;
//             lastCin = curCin;
//         }
//         if (pL3->val == 0)
//         {
//             delete (pL3);
//             prepL3->next = nullptr;
//         }
//         return l3;
//     }
// };

// #include "hot002_AddTwoNum.cpp"

// void tranverseList(ListNode *L)
// {
//     ListNode *p = L;
//     while (p && p->next)
//     {
//         p = p->next;
//         cout << p->val << " ";
//     }
//     cout << endl;
// }
// void buildList(ListNode *L, int a[], int lenth)
// {
//     ListNode *p = L;
//     for (int i = 0; i < lenth; i++)
//     {
//         ListNode *node = new ListNode;
//         node->val = a[i];
//         node->next = NULL;
//         p->next = node;
//         p = p->next;
//     }
// }

// int main()
// {
//     int arr1[3] = {2, 4, 3};
//     int arr2[3] = {5, 6, 4};
//     ListNode *l1 = new ListNode, *pL1 = l1;
//     ListNode *l2 = new ListNode, *pL2 = l2;
//     buildList(l1, arr1, 3);
//     buildList(l2, arr2, 3);
//     tranverseList(l1);
//     tranverseList(l2);
//     Solution sl;
//     tranverseList(sl.addTwoNumbers(l1, l2));
// }