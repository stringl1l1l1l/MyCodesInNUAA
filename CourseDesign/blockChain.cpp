#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <string.h>
using namespace std;

#define FILE_NAME "D:\\Programming\\GitHub\\repository\\DataStruct\\CourseDesign\\.txt\\blockChain.txt"
typedef struct InfoNode
{
    int num;
    char info[200];
    int checkCode;
} InfoNode;

typedef struct LNode
{
    InfoNode data;
    LNode *next;
} LNode, *LinkList;

void initLinkList(LinkList &L)
{
    if (L)
    {
        L = (LNode *)malloc(sizeof(LNode));
        L->data.num = -1; //���ڲ��������ͷ�������Ϊ-1
        L->data.checkCode = 0;
        L->next = NULL;
    }
}

void createLinkList(LinkList &L)
{
    if (L)
    {
        fstream file(FILE_NAME, ios::in);
        if (file.fail())
        {
            cout << "Cannot open file!" << endl;
            return;
        }

        int num = 0,
            checkCode = 0, /*��¼��һ��У����*/
            asciiSum = 0;
        char str[200] = "";

        //������һ���ڵ�
        file >> str;
        for (int i = 0; str[i] != '\0'; i++)
            asciiSum += str[i];

        LNode *node = (LNode *)malloc(sizeof(LNode));
        L->next = node;
        node->data.num = num++;
        strcpy(node->data.info, str);
        node->data.checkCode = asciiSum % 113;
        checkCode = node->data.checkCode;

        //���������ڵ�
        LNode *p = node->next, *pre_p = node;
        while (!file.eof())
        {
            asciiSum = 0;
            p = (LNode *)malloc(sizeof(LNode));

            file >> str;
            for (int i = 0; str[i] != '\0'; i++)
                asciiSum += str[i];

            p->data.num = num++;
            strcpy(p->data.info, str);
            p->data.checkCode = checkCode + p->data.num + asciiSum % 113;
            checkCode = p->data.checkCode;

            pre_p->next = p;
            pre_p = p;
            p = p->next;
        }
        pre_p->next = NULL;
        file.close();
    }
}
int judgeValidation(LinkList &L)
{
    if (L)
    {
        LNode *p = L->next, *pre_p = NULL;
        while (p)
        {
            int asciiSum = 0;
            for (int i = 0; p->data.info[i] != '\0'; i++)
                asciiSum += p->data.info[i];
            int validCheckCode =
                p->data.num == 0 ? asciiSum % 113 : p->data.num + pre_p->data.checkCode + asciiSum % 113;
            if (p->data.checkCode != validCheckCode)
                return p->data.num;
            pre_p = p;
            p = p->next;
        }
    }
    return -1;
}

void updateNode(LinkList &L, int num, char *str)
{
    if (L)
    {
        int res = judgeValidation(L);
        if (res < num && res != -1)
            return;
        //�ҵ�Ŀ����
        LNode *p = L, *pre_p = NULL;
        while (p)
        {
            if (p->data.num == num)
                break;
            pre_p = p;
            p = p->next;
        }
        //���µ�ǰ���
        int asciiSum = 0, oldCheckCode = p->data.checkCode;
        strcpy(p->data.info, str);
        for (int i = 0; p->data.info[i] != '\0'; i++)
            asciiSum += p->data.info[i];
        if (pre_p)
            p->data.checkCode = pre_p->data.checkCode + p->data.num + asciiSum % 113;
        else
            p->data.checkCode = asciiSum % 113;
        pre_p = p;
        p = p->next;

        //���º������У����
        while (p)
        {
            int checkCode = 0;
            checkCode = p->data.checkCode;
            p->data.checkCode = p->data.checkCode - oldCheckCode + pre_p->data.checkCode;
            //�������¼��㣬���Ǽ�ȥ��һ�����δ����ʱ��У�����ټ��ϸ��º��У���룬�����ٱ����ַ���
            pre_p = p;
            p = p->next;
            oldCheckCode = checkCode;
        }
    }
}

void insertNode(LinkList &L, int num, char *str)
{
    if (L)
    {
        int res = judgeValidation(L);
        if (res != -1)
        {
            LNode *p = L->next;
            while (p)
            {
                if (p->data.num == res)
                    break;
                p = p->next;
            }
            updateNode(L, res, p->data.info);
        }
        LNode *p = L;
        while (p)
        {
            if (p->data.num == num - 1)
                break;
            p = p->next;
        }
        //�ҵ�Ŀ����
        LNode *node = (LNode *)malloc(sizeof(LNode));
        node->next = p->next;
        p->next = node;
        //�����ٵĽ������Ӧλ��

        int asciiSum = 0, oldCheckCode = p->data.num >= 0 ? p->data.checkCode : 0;
        strcpy(node->data.info, str);
        node->data.num = num;
        for (int i = 0; node->data.info[i] != '\0'; i++)
            asciiSum += node->data.info[i];
        if (p->data.num >= 0)
            node->data.checkCode = p->data.checkCode + node->data.num + asciiSum % 113;
        else
            node->data.checkCode = asciiSum % 113;

        //���²���Ľ����Ϣ
        p = node->next;
        //���º�����ı����У����
        LNode *pre_p = node;
        while (p)
        {
            int checkCode = p->data.checkCode;
            p->data.num++;
            p->data.checkCode = p->data.checkCode - oldCheckCode + pre_p->data.checkCode + 1;
            oldCheckCode = checkCode;

            pre_p = p;
            p = p->next;
        }
    }
}

void traverseLinkList(LinkList &L)
{
    if (L)
    {
        LNode *p = L->next;
        while (p)
        {
            cout << p->data.num << " " << p->data.checkCode << " " << p->data.info << endl;
            p = p->next;
        }
        cout << endl;
    }
}

int main()
{
    LinkList L;
    initLinkList(L);
    createLinkList(L);
    cout << "�������" << endl;
    cout << "����������Ҫ�Ĳ���: " << endl;
    while (1)
    {
        int num = 0;
        cout << "1.���� 2.���� 3.���� 4.�۸� 5.��֤ 6.�˳�  ";
        cin >> num;
        if (num == 1)
        {
            char elem[100] = "";
            int loc;
            cout << "�������λ�ü�������Ԫ��: " << endl;
            cin >> loc >> elem;
            insertNode(L, loc, elem);
        }
        else if (num == 2)
        {
            char elem[100] = "";
            int loc;
            cout << "��������½���ż���Ҫ���³�Ϊ��Ԫ��: " << endl;
            cin >> loc >> elem;
            updateNode(L, loc, elem);
        }
        else if (num == 3)
            traverseLinkList(L);
        else if (num == 4)
        {
            char elem[100] = "";
            int loc;
            cout << "������۸�Ԫ�ؼ��۸�λ��: " << endl;
            cin >> loc >> elem;
            LNode *p = L->next;
            for (int i = 0; p; p = p->next, i++)
            {
                if (i == loc)
                {
                    strcpy(p->data.info, elem);
                    break;
                }
            }
        }
        else if (num == 5)
        {
            int res = judgeValidation(L);
            if (res == -1)
                cout << "��Ч" << endl;
            else
                cout << res << "�Ž����Ч" << endl;
        }
        else
            break;
    }
}