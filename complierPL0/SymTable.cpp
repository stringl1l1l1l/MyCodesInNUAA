#include "SymTable.h"
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

size_t SymTable::sp = 1;
vector<SymTableItem> SymTable::table; // 一个程序唯一的符号表
vector<size_t> SymTable::display(PROC_CNT, -1); // 过程的嵌套层次表
vector<size_t> SymTable::proc_addrs; // 记录所有过程的地址
vector<size_t> SymTable::offset_stk(PROC_CNT, 0);
wstring cat_map[6] = {
    L"null",
    L"array",
    L"var",
    L"procedure",
    L"const",
    L"formal var"
};
// wstring 转 int
int w_str2int(wstring num_str)
{
    if (num_str.empty()) {
        wcout << L"Cannot transfer empty string!" << endl;
        return 0;
    }
    int num = 0;
    // 先遍历一遍字符串，判断合法性
    size_t size = num_str.size();
    for (auto w_ch : num_str) {
        if (!(w_ch <= L'9' && w_ch >= L'0')) {
            wcout << L"Illegal string to transfer!" << endl;
            return 0;
        }
    }
    for (size_t i = 0; i < size; i++) {
        num = (num << 3) + (num << 1); // num*10
        num += (num_str[i] - L'0');
    }
    return num;
}

Information::Information()
{
    this->offset = 0;
    this->cat = Category::NIL;
    this->level = 0;
}

void Information::show()
{
    wcout << setw(10) << L"cat: " << setw(13) << cat_map[cat]
          << setw(10) << L"offset: " << setw(5) << offset
          << setw(10) << L"level: " << setw(5) << level;
}
VarInfo::VarInfo()
    : Information()
{
    this->value = 0;
    this->type = Type::INTERGER;
}

void VarInfo::setValue(wstring val_str) { this->value = w_str2int(val_str); }

int VarInfo::getValue() { return this->value; }

void VarInfo::show()
{
    wcout << setw(10) << L"cat:" << setw(15) << cat_map[cat]
          << setw(10) << L"offset:" << setw(5) << offset
          << setw(10) << L"level:" << setw(5) << level
          << setw(10) << L"value:" << setw(5) << value;
}

ProcInfo::ProcInfo()
    : Information()
{
    this->entry = 0;
}

void ProcInfo::setEntry(size_t entry) { this->entry = entry; }

size_t ProcInfo::getEntry() { return this->entry; }

void ProcInfo::show()
{
    wcout << setw(10) << L"cat:" << setw(15) << cat_map[cat]
          << setw(10) << L"size:" << setw(5) << offset
          << setw(10) << L"level:" << setw(5) << level
          << setw(10) << L"entry:" << setw(5) << entry;
}

void SymTableItem::show()
{
    wcout << setw(10) << name << setw(10) << next_item;
    info->show();
    wcout << endl;
}
void SymTable::mkTable()
{
    size_t top = table.size();
    sp = top;
    display[level] = sp;
}

int SymTable::enter(wstring name, size_t offset, Category cat)
{
    int pos = lookUpVar(name);
    // 如果在相同作用域有重复符号，且不为形参、过程名，则说明出现重定义
    if (pos != -1 && table[pos].info->level == level
        && table[pos].info->cat != Category::FORM) {
        error(REDEFINED_IDENT, name.c_str());
        return -1;
    }
    size_t top = table.size();
    if (top >= 1)
        table[top - 1].next_item = top;
    SymTableItem item;
    item.next_item = 0;
    item.name = name;
    VarInfo* varInfo = new VarInfo;
    varInfo->offset = offset;
    varInfo->cat = cat;
    varInfo->level = level;
    item.info = varInfo;
    table.push_back(item);
    return top;
}

int SymTable::enterProc(wstring name)
{
    // 若在相同作用域内有重复过程名
    int pos = lookUpProc(name);
    if (pos != -1 && SymTable::table[pos].info->level == level) {
        error(REDEFINED_IDENT, name.c_str());
        return -1;
    }
    // 初始化level处的值
    offset_stk[level] = 0;

    size_t top = table.size();
    SymTableItem item;
    item.next_item = 0;
    item.name = name;
    ProcInfo* procInfo = new ProcInfo;
    procInfo->offset = 0;
    procInfo->cat = Category::PROCE;
    procInfo->level = level;
    item.info = procInfo;
    table.push_back(item);
    proc_addrs.push_back(top);
    return top;
}

int SymTable::lookUpVar(wstring name)
{
    unsigned int curAddr = 0;
    // i代表访问display的指针
    for (int i = level; i >= 0; i--) {
        curAddr = table[display[i]].next_item;
        // 遍历当前display指针指向的过程下的所有变量符号，直到遇到最后一个符号(pre == 0)
        do {
            if (table[curAddr].name == name)
                return curAddr;
        } while (table[curAddr++].next_item != 0);
    }
    return -1;
}

int SymTable::lookUpProc(wstring name)
{
    for (auto addr : proc_addrs) {
        if (table[addr].name == name)
            return addr;
    }
    return -1;
}

void SymTable::addWidth(size_t addr, size_t width)
{
    table[addr].info->offset = width;
}

void symTableTest()
{
    // wcout << L"SymTable____________________" << endl;
    // wcout << setw(10) << L"name" << setw(10) << L"next" << endl;
    // for (auto mem : SymTable::table) {
    //     wcout << setw(10) << mem.name << setw(10) << mem.next_item << endl;
    // }

    // wcout << L"display_____________________" << endl;
    // wcout << setw(10) << L"addr" << setw(10) << L"proc" << endl;
    // for (int i = 0; i < PROC_CNT; i++) {
    //     int mem = SymTable::display[i];
    //     if (mem != -1)
    //         wcout << setw(10) << mem << setw(10) << SymTable::table[mem].name << endl;
    // }
    // wcout << L"proc_size_____________________" << endl;
    // wcout << setw(10) << L"proc" << setw(10) << L"size" << endl;
    // for (int i = 0; i < SymTable::proc_addrs.size(); i++) {
    //     wcout << setw(10)
    //           << SymTable::table[SymTable::proc_addrs[i]].name
    //           << setw(10)
    //           << SymTable::table[SymTable::proc_addrs[i]].info.offset << endl;
    // }
    wcout << L"SymTable____________________" << endl;
    for (auto mem : SymTable::table) {
        mem.show();
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
              << SymTable::table[SymTable::proc_addrs[i]].info->offset << endl;
    }
}