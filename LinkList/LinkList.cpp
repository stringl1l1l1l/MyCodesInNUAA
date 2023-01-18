#include <cstdlib>
#include <cstdio>

typedef struct LNode
{
	int data;
	LNode *next;
} LNode, *LinkList;

void destoryLinkList(LinkList &L);

//1.初始化、创建、销毁和清空
void initLinkList(LinkList &L)
{
	if (L)
		destoryLinkList(L);

	L = (LNode *)malloc(sizeof(LNode));
	L->next = NULL;
}

void createLinkList(LinkList &L, int num, int elm[])
{
	if (!L)
	{
		printf("Error:Initialize the list first!");
		exit(-1);
	}

	if (L->next)
	{
		printf("Error: The list has existed!");
		exit(-1);
	}

	LNode *p = NULL, *pre_p = L;
	for (int i = 0; i < num; i++)
	{
		p = (LNode *)malloc(sizeof(LNode));
		p->data = elm[i];
		pre_p->next = p;
		pre_p = p;
	}

	pre_p->next = NULL;
}

void destoryLinkList(LinkList &L)
{
	if (!L)
	{
		printf("Error:Initialize the list first!");
		exit(-1);
	}

	LNode *p = NULL;
	while (L->next)
	{
		p = L->next;
		L->next = p->next;
		free(p);
	}
	free(L);
	L = NULL;
	//free过后指针成为野指针，需要置为空指针
}
void clearLinkList(LinkList &L)
{
	destoryLinkList(L->next);
}

//2.访问型操作

bool isEmpty(LinkList &L)
{
	return L->next ? false : true;
}

int length(LinkList &L)
{
	if (!L)
	{
		printf("Error:Initialize the list first!");
		exit(-1);
	}
	else if (isEmpty(L))
		return 0;

	LNode *p = L;
	int i = 0;
	while (p && p->next)
	{
		p = p->next;
		i++;
	}
	return i;
}

LNode *getCurNode(LinkList &L, int loc)
{
	if (!L)
	{
		printf("Error:Initialize the list first!");
		exit(-1);
	}

	if (loc < 1 || loc > length(L))
	{
		printf("Error:Illegal parameter!");
		exit(-1);
	}

	LNode *p = L;
	int i = 1;
	while (p && p->next)
	{
		p = p->next;
		if (i++ == loc)
			return p;
	}

	printf("Error:Cannot find the element on the given location!");
	exit(-1);
}

int traverseLinkList(LinkList &L)
{
	if (!L)
	{
		printf("Error:Initialize the list first!");
		return -1;
	}

	if (isEmpty(L))
	{
		printf("Empty");
		return 0;
	}

	LNode *p = L;
	int i = 0;
	while (p && p->next)
	{
		printf("%d ", p->next->data);
		p = p->next;
	}
	printf("\n");
	return 0;
}

int locateElem(LinkList &L, int elem)
{
	if (!L)
	{
		printf("Error:Initialize the list first!");
		exit(-1);
	}

	LNode *p = L;
	int i = 1;
	while (p && p->next)
	{
		p = p->next;
		if (p->data == elem)
			return i;
		i++;
	}

	printf("Error:Cannot find the element!");
	exit(-1);
}

LNode *getPriorNode(LinkList &L, int elem)
{
	if (!L)
	{
		printf("Error:Initialize the list first!");
		exit(-1);
	}

	if (L->next->data == elem)
	{
		printf("Error:Current element is the first element!");
		exit(-1);
	}

	LNode *p = L, *q = L;
	while (p && p->next)
	{
		p = q->next;
		if (elem == p->data)
			return q;
		q = p;
	}

	printf("Error:Cannot get the element!");
	exit(-1);
}

LNode *getNextNode(LinkList &L, int elem)
{
	if (!L)
	{
		printf("Error:Initialize the list first!");
		exit(-1);
	}

	if (isEmpty(L))
	{
		printf("Error:Empty list!");
		exit(-1);
	}

	LNode *p = L, *q = L;
	while (p && p->next)
	{
		p = q->next;
		if (elem == q->data)
			return p;
		q = p;
	}

	printf("Error:Cannot get the element!");
	exit(-1);
}

//3.加工型操作
int setElem(LinkList &L, int loc, int elem)
{
	LNode *p = getCurNode(L, loc);
	int oldVal = p->data;
	p->data = elem;
	return oldVal;
}

void insertElem(LinkList &L, int loc, int elem)
{
	if (!L)
	{
		printf("Error:Initialize the list first!");
		exit(-1);
	}

	if (loc < 1 || loc > length(L) + 1)
	{
		printf("Error:Illegal parameter!");
		exit(-1);
	}

	LNode *s = (LNode *)malloc(sizeof(LNode));
	s->data = elem;

	LNode *p = L, *q = L;
	int i = 1;
	while (q->next) //若条件设置为p->next，则最后一步不会执行i++
	{
		p = q->next;
		if (i++ == loc)
		{
			q->next = s;
			s->next = p;
		}
		q = p;
	}

	//两种情况下会退出循环：空表、遍历到尾
	//若在循环中i始终小于loc,判断退出循环时++后的i是否等于loc,若相等，说明在链表尾部插入元素
	//其实if可以不用写，此处方便理解
	if (i == loc || isEmpty(L))
	{
		p->next = s;
		s->next = NULL;
	}
}

int deleteElem(LinkList &L, int loc)
{
	if (!L)
	{
		printf("Error:Initialize the list first!");
		exit(-1);
	}

	if (loc < 1 || loc > length(L))
	{
		printf("Error:Illegal parameter!");
		exit(-1);
	}

	LNode *p = L, *q = L;
	int i = 1;
	while (p->next)
	{
		p = q->next;
		if (i++ == loc)
		{
			q->next = p->next;
			int elem = p->data;
			free(p);
			return elem;
		}
		q = p;
	}
}

//4.附加操作
int reverseLinkList(LinkList &L)
{
	if (!L)
	{
		printf("Error:Initialize the list first!");
		exit(-1);
	}

	if (isEmpty(L) || !L->next->next)
		return 0;

	LNode *p = L->next->next, *q = p->next;
	L->next->next = NULL;
	while (p) //条件为q时结果一样，因为while的条件是在代码执行后判断的
	{
		q = p->next;
		p->next = L->next;
		//若先让L->next=p,则L->next后面的元素会丢失
		L->next = p;
		p = q;
	}
	return 0;
}

int deleteDuplicateElem(LinkList &L)
{
	if (!L)
	{
		printf("Error:Initialize the list first!");
		exit(-1);
	}

	if (isEmpty(L) || !L->next->next)
		return 0;

	LNode *p = L->next, *t = p, *q = t->next;
	//t指向q前一个结点
	//p指向被比较的结点
	//q用于遍历是否有和p元素一样的结点
	while (p->next)
	{
		t = p;
		q = t->next;
		//不要放在后面写
		while (q)
		{

			if (p->data == q->data)
			{
				t->next = q->next;
				free(q);
			}
			else
				t = q;

			q = t->next;
		}

		p = p->next;
	}

	//对比

	/*while (p)
	{
		while (t->next) //不能以q为条件，条件判断在t改变之后，在q改变之前
		{

			if (p->data == q->data)
			{
				t->next = q->next;
				free(q);
				q = t->next;
			}
			else
			{
				t = q;
				q = t->next;
			}
		}

		p = p->next;
		//每次遍历循环结束调整t,q指针
		t = p;
		if (t) //由于最后一步p成为了空指针，必须加上该条件
			q = t->next;
	}
	*/
}

int bubbleSort(LinkList &L)
{
	if (!L)
	{
		printf("Error:Initialize the list first!");
		exit(-1);
	}

	if (isEmpty(L) || !L->next->next)
		return 0;

	LNode *t = L, *p = L->next, *q = p->next;
	int len = length(L);
	for (int i = 0; i < len - 1; i++)
	{
		t = L;
		p = L->next;
		q = p->next;
		//每次循环结束后最值沉底，需从头开始遍历
		for (int j = 0; j < len - i - 1; j++)
		{
			if (p->data > q->data)
			{
				p->next = q->next;
				q->next = p;
				t->next = q;
			}
			t = t->next;
			p = t->next; //不要写成p = q,因为交换过后p、q的位置互换
			q = p->next;
		}
	}
	return 0;
}

int selectiveSort(LinkList &L)
{
	if (!L)
	{
		printf("Error:Initialize the list first!");
		exit(-1);
	}

	if (isEmpty(L) || !L->next->next)
		return 0;

	LNode *pre_p = L, *p = L->next, *q = p->next;
	int len = length(L);

	while (pre_p->next->next)
	{
		p = pre_p->next;
		q = p->next;
		LNode *k = p, *pre_q = p, *pre_k = pre_p;
		while (q)
		{
			if (k->data > q->data)
			{
				k = q;
				pre_k = pre_q;
			}
			pre_q = q;
			q = q->next;
		}
		if (k == p)
			;
		//不能写continue，否则不会执行指针移位
		else if (pre_k == p)
		{
			p->next = k->next;
			pre_p->next = k;
			k->next = p;
		}
		else
		{
			pre_p->next = p->next;
			pre_k->next = k->next;
			//摘下两结点
			k->next = pre_p->next;
			pre_p->next = k;
			p->next = pre_k->next;
			pre_k->next = p;
			//交换两结点
		}
		pre_p = pre_p->next;
	}
}

int combineLinkListInDescOrder(LinkList &L1, LinkList &L2)
{
	if (isEmpty(L2))
		return 0;
	else if (isEmpty(L1))
	{
		L1 = L2;
		return 0;
	}

	LNode *pre_p2 = L2, *pre_p1 = L1, *p1 = pre_p1->next, *p2 = pre_p2->next;

	while (p1 && p2)
	{

		if (p2->data > p1->data)
		{
			pre_p2->next = p2->next;
			//摘下待插入元素
			p2->next = p1;
			pre_p1->next = p2;
			//插入操作

			pre_p1 = p2;
			p2 = pre_p2->next;
		}
		else if (p2->data == p1->data)
		{
			pre_p2 = pre_p2->next;
			p2 = pre_p2->next;

			pre_p1 = pre_p1->next;
			p1 = pre_p1->next;
		}
		else
		{
			pre_p1 = pre_p1->next;
			p1 = pre_p1->next;
		}
	}

	if (p2 && p2->data < pre_p1->data)
	{
		while (p2)
		{
			pre_p1->next = p2;
			pre_p1 = p2;
			p2 = p2->next;
		}
	}

	return 0;
}

int JosephusByLinkList(LinkList &L, int n, int m)
{
	if (!L)
	{
		printf("Error:Initialize the list first!");
		exit(-1);
	}

	if (isEmpty(L))
		return 0;

	LNode *tail = getCurNode(L, n);
	tail->next = L;
	//普通链表转为循环链表
	LNode *pre_p = L, *p = pre_p->next;
	int num = 1;
	while (L->next != L)
	{
		if (p == L)
			pre_p = pre_p->next;
		//跳过头结点(不执行num++)
		else if ((num++ % m) == 0)
		{
			pre_p->next = p->next;
			printf("%d ", p->data);
			free(p);
			p = NULL;
		} //出列即删除操作，p前指针不会变，不需要将其移位
		else
			pre_p = pre_p->next;
		//还未到出列的成员，执行上面的num++
		p = pre_p->next;
	}
	return 0;
}

int main()
{
	LinkList l1 = NULL, l2 = NULL;
	int elem1[4] = {7, 5, 2, 1};
	int elem2[7] = {9, 8, 7, 4, 3, 0, -1};
	initLinkList(l1);
	initLinkList(l2);
	createLinkList(l1, 4, elem1);
	createLinkList(l2, 7, elem2);
	combineLinkListInDescOrder(l1, l2);
	traverseLinkList(l1);
}
