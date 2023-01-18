#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

#define FILE_NAME "D:\\Programming\\GitHub\\repository\\DataStruct\\CourseDesign\\.txt\\familyTree.txt"

struct Date
{
    string year;
    string month;
    string day;
};

struct PersonInfo
{
    string name;
    Date birthDay, deathDay;
};

typedef struct TreeNode
{
    PersonInfo data;
    TreeNode *parent;
    TreeNode *firstChild, *nextBro;
} TreeNode, *Tree;

void str_to_TreeNode(TreeNode *node, string info)
{
    node = new TreeNode;
    string::size_type pos = info.find('.');
    if (pos != string::npos)
    {
        node->data.name = info.substr(0, pos);
        string dateStr = info.substr(pos + 1, info.size());
        string::size_type pos1 = 0, pos2 = dateStr.find('.');
        node->data.birthDay.year = dateStr.substr(pos1, pos2);
    }
}

void buildTreeFromFile(Tree &T)
{
    T = NULL;
    fstream file(FILE_NAME, ios::in);
    if (file.fail())
    {
        cout << "Cannot open file!" << endl;
        exit(-1);
    }

    TreeNode *p = T, *lastHead = NULL;
    string line;
    while (getline(file, line))
    {
        string info;
        stringstream sstream(line);

        while (sstream >> info)
        {
            if (info == "#")
            {
                p = p->nextBro;
                continue;
            }
            TreeNode *node = NULL;
            str_to_TreeNode(node, info);
        }
    }
    file.close();
}

int main()
{
}