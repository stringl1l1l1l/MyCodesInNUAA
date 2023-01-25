#include "SymbolTable.h"
#include <iomanip>
unsigned int SymTable::sp = 1;
vector<SymTableItem> SymTable::table; // 一个程序唯一的符号表
unsigned int SymTable::display[] = { 0 }; // 过程的嵌套层次表

void SymTable::mkTable()
{
    size_t top = table.size();
    sp = top;
    display[level] = sp;
}
size_t SymTable::enter(wstring name, size_t offset, Category cat)
{
    size_t top = table.size();
    SymTableItem item;
    item.pre_item = top + 1;
    item.name = name;
    VarInfo info;
    info.offset = offset;
    info.cat = cat;
    info.type = Type::INTERGER;
    item.info = info;
    table.push_back(item);
    return top;
}
void SymTable::addWidth(unsigned int width) { }
size_t SymTable::enterProc(wstring name)
{
    size_t top = table.size();
    SymTableItem item;
    item.pre_item = 0;
    item.name = name;
    ProcInfo info;
    info.cat = Category::PROCE;
    item.info = info;
    table.push_back(item);
    return top;
}
size_t SymTable::position(wstring name)
{
    unsigned int curAddr = 0;
    // i代表访问display的指针
    for (int i = level; i >= 0; i--) {
        curAddr = display[i];
        // 遍历当前display指针指向的过程下的所有符号(非过程名)，直到遇到下一个过程名(pre == 0)
        while (SymTable::table[curAddr].pre_item != 0) {
            if (SymTable::table[curAddr].name == name) {
                return curAddr;
            }
            curAddr++;
        }
    }
    return 0;
}

void symTableTest()
{
    wcout << L"SymTable____________________" << endl;
    wcout << setw(10) << L"name" << setw(10) << L"pre" << endl;
    for (auto mem : SymTable::table) {
        wcout << setw(10) << mem.name << setw(10) << mem.pre_item << endl;
    }

    wcout << L"display_____________________" << endl;
    wcout << setw(10) << L"addr" << setw(10) << L"proc" << endl;
    for (int i = 0; i < PROC_CNT; i++) {
        unsigned int mem = SymTable::display[i];
        if (mem > 0)
            wcout << setw(10) << mem << setw(10) << SymTable::table[mem - 1].name << endl;
    }
    size_t pos = SymTable::position(L"a");
    wcout << pos << endl;
}