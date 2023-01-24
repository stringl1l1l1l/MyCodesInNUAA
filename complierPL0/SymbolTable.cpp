#include "SymbolTable.h"
unsigned int SymTable::sp = 1;
vector<SymTableItem> SymTable::table(SYM_ITEMS_CNT); // 一个程序唯一的符号表
vector<unsigned int> SymTable::display(PROC_CNT); // 过程的嵌套层次表
void SymTable::mkTable(unsigned int pre)
{
}
void SymTable::enter(wstring name, size_t offset, Type t)
{
    SymTableItem item;
    VarInfo info;
    info.offset = offset;
    info.cat = Category::VAR;
    info.type = t;
    item.name = name;
    item.info = info;
}
void SymTable::addWidth(unsigned int width) { }
void SymTable::enterProc(wstring name) { }
