#include <iostream>
#include <fstream>
#include <map>
using namespace std;

#define INFINITY 2000000000
#define SOURCE_FILE "D:\\Programming\\GitHub\\repository\\DataStruct\\CourseDesign\\.txt\\HuffmanSource.txt"
#define CODE_FILE "D:\\Programming\\GitHub\\repository\\DataStruct\\CourseDesign\\.dat\\HuffmanCode.dat"
#define DECODE_FILE "D:\\Programming\\GitHub\\repository\\DataStruct\\CourseDesign\\.txt\\HuffmanDecode.txt"

typedef struct HuffNode
{
    char ch;
    int weight;
    int parent, left, right;
} HuffNode, *HuffTable;

void countAllChar(map<char, int> &chMap)
{
    fstream file(SOURCE_FILE, ios::in);
    if (file.fail())
    {
        cout << "Cannot open file!" << endl;
        exit(-1);
    }
    char ch;
    while (file.get(ch))
        chMap[ch]++;
    chMap[0b11111100]++; //结束符
    file.close();
}

void buildHuffmanTable(HuffTable &table, map<char, int> chMap)
{
    int size = chMap.size();
    table = new HuffNode[2 * size - 1];
    //初始化操作
    for (int i = 0; i < 2 * size - 1; i++)
    {
        table[i].ch = 0;
        table[i].left = -1;
        table[i].parent = -1;
        table[i].right = -1;
        table[i].weight = -1;
    }
    int i = 0;
    for (map<char, int>::iterator it = chMap.begin(); it != chMap.end(); ++it)
    {
        table[i].ch = it->first;
        table[i].weight = it->second;
        i++;
    }
    int count = size; //记录当前树的个数
    while (count < 2 * size - 1)
    {
        int minIndex = -1, secMinIndex = -1, min = INFINITY, secMin = INFINITY;
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
}

void HuffmanCode(map<char, char *> &codeMap, int n, HuffTable HT)
{
    char *temp = new char[n];
    for (int i = 0; i < n; i++)
    {
        int num = 0;
        for (int j = i, k = HT[i].parent; k != -1; j = k, k = HT[k].parent)
        {
            if (HT[k].left == j)
                temp[num++] = '0';
            else
                temp[num++] = '1';
        }
        temp[num] = 0;
        //逆置
        char *code = new char[num + 1];
        for (int j = num - 1; j >= 0; j--)
            code[num - j - 1] = temp[j];
        code[num] = 0;
        codeMap.insert(make_pair(HT[i].ch, code));
    }

    delete[] temp;
    /* for (map<char, char *>::iterator it = codeMap.begin(); it != codeMap.end(); ++it)
         cout << it->first << " " << it->second << endl;*/
}

void codePassage(map<char, char *> codeMap)
{
    fstream inFile(SOURCE_FILE, ios::in), outFile(CODE_FILE, ios::out | ios::binary);
    if (inFile.fail() || outFile.fail())
    {
        cout << "Cannot open file!" << endl;
        exit(-1);
    }
    string pasCode;
    char ch;

    while (inFile.get(ch))
        pasCode.append(codeMap.at(ch));
    pasCode.append(codeMap.at(0b11111100));
    int i = 0;
    char Byte = 0b00000000;
    while (i < pasCode.size())
    {
        Byte = 0b00000000;
        for (int j = 7; j >= 0 && i < pasCode.size(); j--)
        {
            char bit = pasCode[i++] - '0';
            Byte ^= bit << j;
        }
        outFile.write(&Byte, sizeof(Byte));
    }
    inFile.close();
    outFile.close();
}

void decode(map<char, char *> codeMap)
{
    fstream inFile(CODE_FILE, ios::in | ios::binary), outFile(DECODE_FILE, ios::out);
    if (inFile.fail() || outFile.fail())
    {
        cout << "Cannot open file!" << endl;
        exit(-1);
    }
    char ch;
    string pasCode;
    while (inFile.get(ch))
    {
        unsigned char usCh = ch;
        char code[9];
        for (int i = 0; i < 8; i++)
            code[i] = '0';
        code[8] = 0;
        int num = 7;
        while (usCh)
        {
            code[num--] = usCh % 2 + '0';
            usCh /= 2;
        }
        pasCode.append(code);
    }

    string code, passage;
    char sign;
    for (int i = 0; i < pasCode.size(); i++)
    {
        code.push_back(pasCode[i]);
        for (map<char, char *>::iterator it = codeMap.begin(); it != codeMap.end(); ++it)
        {
            if (code == it->second)
            {
                sign = it->first;
                if ((unsigned char)sign == 0b11111100)
                    return;
                outFile.put(sign);
                sign = 0;
                code = "";
                break;
            }
        }
    }
    outFile.close();
    inFile.close();
}

int main()
{
    HuffTable HT;
    map<char, int> countMap;
    map<char, char *> codeMap;
    countAllChar(countMap);
    buildHuffmanTable(HT, countMap);
    HuffmanCode(codeMap, countMap.size(), HT);
    codePassage(codeMap);
    decode(codeMap);
}