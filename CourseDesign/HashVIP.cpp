#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

#define FILE_NAME "D:\\Programming\\GitHub\\repository\\DataStruct\\CourseDesign\\.txt\\HashVIP.txt"

struct Date
{
    string month;
    string day;
};
struct Record
{
    string idCard;
    string name;
    string flightNum;
    Date flightDate;
    int distance;
};

typedef struct HashNode
{
    string idCard;
    int *index;
    HashNode *next;
    HashNode *end;
} HashNode, **LinkHashTable;

unsigned int myHash(string str)
{
    unsigned int hash = 0;
    char ch;
    string::iterator it = str.begin(), end = str.end();
    int seed = 131313;
    while (it != end)
    {
        ch = *it++;
        hash = hash * seed + ch;
    }
    int range = 60;
    hash = hash % range;
    return hash;
}

Date transToDate(string date)
{
    Date d;
    string::size_type pos = date.find('.');
    if (pos != string::npos)
    {
        d.month = date.substr(0, pos);
        d.day = date.substr(pos + 1, date.size());
    }
    return d;
}

vector<Record> readFromFile()
{
    fstream file(FILE_NAME, ios::in);
    if (file.fail())
    {
        cout << "Cannot open file!" << endl;
        exit(-1);
    }
    vector<Record> vec;
    string line;
    while (getline(file, line))
    {
        stringstream sstream(line);
        string info;
        int dis = 0;
        Date date;
        Record record;

        sstream >> info;
        record.name = info;
        sstream >> info;
        record.idCard = info;
        sstream >> info;
        record.flightNum;
        sstream >> info;
        record.flightDate = transToDate(info);
        sstream >> dis;
        record.distance = dis;
        vec.push_back(record);
    }
    file.close();
    return vec;
}

bool cmp(Record r1, Record r2)
{
    return myHash(r1.idCard) < myHash(r2.idCard);
}

void buildLinkHashTable(HashNode **hashTable, vector<Record> record, int size)
{
    int confNum = 0, rediNum = 0;
    // HashNode** hashTable = (HashNode**)malloc(sizeof(HashNode) * size);
    // 在函数中开辟空间并返回局部变量地址是很危险的操作，应当使用传参形式
    for (int i = 0; i < size; i++)
        hashTable[i] = NULL;

    // vector<Record>::iterator it2 = it + 1;
    for (vector<Record>::iterator it = record.begin(); it != record.end(); ++it)
    {
        unsigned int curHash = myHash(it->idCard);
        if (hashTable[curHash] == NULL)
        {
            hashTable[curHash] = new HashNode;
            hashTable[curHash]->index = (int *)malloc(sizeof(int) * (record.size() + 1));
            hashTable[curHash]->index[0] = 1;
            hashTable[curHash]->idCard = it->idCard;
            hashTable[curHash]->end = hashTable[curHash];
            hashTable[curHash]->next = NULL;
            hashTable[curHash]->index[(hashTable[curHash]->index[0])++] = it - record.begin();
        }
        else
        {
            //检查冲突
            HashNode *p = hashTable[curHash];
            while (p)
            {
                if (p->idCard == it->idCard)
                    break;
                p = p->next;
                rediNum++;
            }
            if (p)
                p->index[(p->index[0])++] = it - record.begin();
            else
            {
                HashNode *node = new HashNode; //由于有string,不能使用malloc开辟空间
                node->index = (int *)malloc(sizeof(int) * (record.size() + 1));
                hashTable[curHash]->end->next = node;
                node->index[0] = 1;
                node->end = node;
                hashTable[curHash]->end = node;
                node->idCard = it->idCard;
                node->next = NULL;
                node->index[(node->index[0])++] = it - record.begin();
                confNum++;
            }
        }
    }
    //删除多余空间
    for (int i = 0; i < size; i++)
    {
        HashNode *p = hashTable[i];
        while (p)
        {
            p->index = (int *)realloc(p->index, sizeof(int) * p->index[0]);
            // for (int j = 1; j < p->index[0]; j++)
            //     cout << i << " " << record[p->index[j]].name << " " << record[p->index[j]].idCard << " "
            //     << record[p->index[j]].flightDate.month << "." << record[p->index[j]].flightDate.day << " "
            //     << record[p->index[j]].distance << endl;
            p = p->next;
        }
    }
    cout << "链地址法建表完成，"
         << "冲突" << confNum << "次, "
         << "重定位" << rediNum << "次" << endl;
}

void buildOpenAddrHashTable(int **hashTable, vector<Record> record, int size)
{
    int confNum = 0, rediNum = 0;
    // int** hashTable = (int**)malloc(sizeof(int*) * size);
    for (int i = 0; i < size; i++)
        hashTable[i] = NULL;
    for (vector<Record>::iterator it = record.begin(); it != record.end(); ++it)
    {
        unsigned int curHash = myHash(it->idCard);
        if (hashTable[curHash] == NULL)
        {
            hashTable[curHash] = new int[record.size() + 1];
            hashTable[curHash][0] = 1;
            hashTable[curHash][hashTable[curHash][0]++] = it - record.begin();
        }
        else
        {
            unsigned int hash = curHash;
            int i = 1, c = -1;
            while (i <= size)
            {
                if (!hashTable[hash] || it->idCard == record[hashTable[hash][1]].idCard)
                    break;
                hash = (curHash + (i++)) % size;
            }
            if (hashTable[hash] && i <= size)
                hashTable[hash][hashTable[hash][0]++] = it - record.begin();
            else
            {
                unsigned int hash = curHash;
                int i = 1, sign = 1;
                confNum++;
                while (hashTable[hash])
                {
                    hash = (curHash + (i++)) % size;
                    rediNum++;
                }
                hashTable[hash] = new int[record.size() + 1];
                hashTable[hash][0] = 1;
                hashTable[hash][hashTable[hash][0]++] = it - record.begin();
            }
        }
    }
    for (int i = 0; i < size; i++)
    {
        int *p = hashTable[i];
        if (p)
        {
            p = (int *)realloc(p, sizeof(int) * p[0]);
            // for (int j = 1; j < p[0]; j++)
            //     cout << i << " " << record[p[j]].name << " " << record[p[j]].idCard << " "
            //          << record[p[j]].flightDate.month << "." << record[p[j]].flightDate.day << " "
            //          << record[p[j]].distance << endl;
        }
    }
    cout << "开放定址法建表完成，"
         << "冲突" << confNum << "次, "
         << "重定位" << rediNum << "次" << endl;
}

void findClient(vector<Record> record, HashNode **&hashTable, string idCard, int size)
{
    unsigned int hash = myHash(idCard);
    HashNode *p = hashTable[hash];
    string isVIP = "否";
    while (p)
    {
        if (p->idCard == idCard)
            break;
        p = p->next;
    }
    int count = 0, disSum = 0;
    for (int j = 1; j < p->index[0]; j++)
    {
        count++;
        disSum += record[p->index[j]].distance;
    }
    if (disSum >= 10000)
        isVIP = "是";
    cout << "客户 " << setw(6) << record[p->index[1]].name << ",";
    cout << " 乘机 " << setw(3) << count << " 次,";
    cout << " 总里程 " << setw(6) << disSum << "km";
    cout << ", 是否为VIP用户: " << isVIP << endl;
}

void findClient(vector<Record> record, int **hashTable, string idCard, int size)
{
    unsigned int hash = myHash(idCard);
    unsigned int curHash = hash;
    int count = 0, disSum = 0;
    string isVIP = "否";
    int *p = hashTable[hash], i = 1;
    while (1)
    {
        if (!p)
        {
            hash = (curHash + (i++)) % size;
            continue;
        }
        if (record[p[1]].idCard == idCard)
            break;
        hash = (curHash + (i++)) % size;
        p = hashTable[hash];
    }
    for (int j = 1; j < p[0]; j++)
    {
        count++;
        disSum += record[p[j]].distance;
    }
    if (disSum >= 10000)
        isVIP = "是";
    cout << "客户 " << setw(6) << record[p[1]].name << ",";
    cout << " 乘机 " << setw(3) << count << " 次,";
    cout << " 总里程 " << setw(6) << disSum << "km";
    cout << ", 是否为VIP用户: " << isVIP << endl;
}

void countAllClientsByLHTable(vector<Record> record, int size)
{
    LinkHashTable hashTable = new HashNode *[size];
    buildLinkHashTable(hashTable, record, size);
    vector<string> people;
    for (vector<Record>::iterator it = record.begin(); it != record.end(); ++it)
        people.push_back(it->idCard);
    sort(people.begin(), people.end());
    people.erase(unique(people.begin(), people.end()), people.end());

    for (vector<string>::iterator it = people.begin(); it != people.end(); ++it)
        findClient(record, hashTable, *it, size);
}

void countAllClientsByOATable(vector<Record> record, int size)
{
    int **hashTable = new int *[size];
    buildOpenAddrHashTable(hashTable, record, size);
    vector<string> people;
    for (vector<Record>::iterator it = record.begin(); it != record.end(); ++it)
        people.push_back(it->idCard);
    sort(people.begin(), people.end());
    people.erase(unique(people.begin(), people.end()), people.end());
    for (vector<string>::iterator it = people.begin(); it != people.end(); ++it)
        findClient(record, hashTable, *it, size);
}

int main()
{
    vector<Record> record(readFromFile());
    // findClient(record, buildOpenAddrHashTable(record, 50), "060033198805004571", 60);
    countAllClientsByLHTable(record, 60);
    cout << "____________________________________________________________" << endl;
    countAllClientsByOATable(record, 60);
}