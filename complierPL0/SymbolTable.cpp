#include "SymbolTable.h"
#include <iomanip>
#include <map>
#include <vector>

size_t SymTable::sp = 1;
vector<SymTableItem> SymTable::table; // 一个程序唯一的符号表
vector<size_t> SymTable::display(PROC_CNT, -1); // 过程的嵌套层次表
vector<size_t> SymTable::proc_addrs; // 记录所有过程的地址
vector<size_t> SymTable::offset_stk(PROC_CNT, 0);

void SymTable::mkTable()
{
    size_t top = table.size();
    sp = top;
    display[level] = sp;
}

int SymTable::enter(wstring name, size_t offset, Category cat)
{
    int pos = position(name);
    // 如果在相同作用域有重复符号，且不为形参、过程名，则说明出现重定义
    if (pos != -1 && table[pos].info.level == level
        && table[pos].info.cat != Category::FORM) {
        error(REDEFINED_IDENT, name.c_str());
        return -1;
    }
    size_t top = table.size();
    if (top >= 1)
        table[top - 1].next_item = top;
    SymTableItem item;
    item.next_item = 0;
    item.name = name;
    VarInfo info;
    info.offset = offset;
    info.cat = cat;
    info.type = Type::INTERGER;
    info.level = level;
    item.info = info;
    table.push_back(item);
    return top;
}

int SymTable::enterProc(wstring name)
{
    // 若在相同作用域内有重复过程名
    int pos = -1;
    for (auto addr : proc_addrs) {
        if (table[addr].info.level == level
            && table[addr].name == name) {
            pos = addr;
        }
    }
    if (pos != -1) {
        error(REDEFINED_IDENT, name.c_str());
        return -1;
    }
    offset_stk[level] = 0;
    size_t top = table.size();
    SymTableItem item;
    item.next_item = 0;
    item.name = name;
    ProcInfo info;
    info.cat = Category::PROCE;
    info.level = level;

    item.info = info;
    table.push_back(item);
    proc_addrs.push_back(top);
    return top;
}

int SymTable::position(wstring name)
{
    unsigned int curAddr = 0;
    // i代表访问display的指针
    for (int i = level; i >= 0; i--) {
        curAddr = table[display[i]].next_item;
        // 遍历当前display指针指向的过程下的所有变量符号，直到遇到最后一个符号(pre == 0)
        do {
            if (table[curAddr].name == name) {
                return curAddr;
            }
            curAddr++;
        } while (table[curAddr].next_item != 0);
    }
    return -1;
}

void SymTable::addWidth(size_t addr, size_t width)
{
    table[addr].info.offset = width;
}

void symTableTest()
{
    wcout << L"SymTable____________________" << endl;
    wcout << setw(10) << L"name" << setw(10) << L"next" << endl;
    for (auto mem : SymTable::table) {
        wcout << setw(10) << mem.name << setw(10) << mem.next_item << endl;
    }

    wcout << L"display_____________________" << endl;
    wcout << setw(10) << L"addr" << setw(10) << L"proc" << endl;
    for (int i = 0; i < PROC_CNT; i++) {
        int mem = SymTable::display[i];
        if (mem != -1)
            wcout << setw(10) << mem << setw(10) << SymTable::table[mem].name << endl;
    }
    wcout << L"proc_size_____________________" << endl;
    wcout << setw(10) << L"proc" << setw(10) << L"size" << endl;
    for (int i = 0; i < SymTable::proc_addrs.size(); i++) {
        wcout << setw(10)
              << SymTable::table[SymTable::proc_addrs[i]].name
              << setw(10)
              << SymTable::table[SymTable::proc_addrs[i]].info.offset << endl;
    }
    // int pos = SymTable::position(L"a");
    // wcout << pos << endl;
}