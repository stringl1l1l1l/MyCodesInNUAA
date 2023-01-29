#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H

#include "PL0.h"
#include <map>
#include <string>
#include <vector>

using namespace std;
class Information {
public:
    enum Category cat; // 种属
    size_t offset;
    size_t level;
};
class VarInfo : public Information {
public:
    enum Type type; // 类型
    unsigned int size; // 占用存储单元数
    bool isFormVar; // 是否为形参的标记
    bool hasAssigned; // 是否进行过赋值的标记
    void* extraInfo;
};

class ProcInfo : public Information {
public:
    vector<VarInfo> formVarList;
};

// 符号表项
typedef struct SymTableItem {
    unsigned int next_item;
    Information info;
    wstring name; // 符号名
} SymTableItem;
struct Couple {
    size_t tablePtr;
    size_t offset;
};
class SymTable {
public:
    static size_t sp; // 指向当前子过程符号表的首地址
    static vector<SymTableItem> table; // 一个程序唯一的符号表
    static vector<size_t> display; // 过程的嵌套层次表
    static vector<size_t> proc_addrs;
    static vector<size_t> offset_stk;

public:
    SymTable();
    ~SymTable();
    // 创建子符号表
    static void mkTable();
    // 将变量名登入符号表
    static int enter(wstring name, size_t offset, Category cat);
    static void addWidth(size_t addr, size_t width);
    // 将过程名登入符号表
    static int enterProc(wstring name);
    // 查找符号在符号表中位置
    static int position(wstring name);
};

void symTableTest();
#endif