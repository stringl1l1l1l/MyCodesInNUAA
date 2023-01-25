#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H

#include "PL0.h"
#include <string>
#include <type_traits>
#include <vector>

using namespace std;
class Information {
public:
    enum Category cat; // 种属
};
class VarInfo : public Information {
public:
    enum Type type; // 类型
    unsigned int size; // 占用存储单元数
    unsigned int offset; // 相对地址（偏移量）
    bool isFormVar; // 是否为形参的标记
    bool hasAssigned; // 是否进行过赋值的标记
    unsigned int level;
    void* extraInfo;
};

class ProcInfo : public Information {
public:
    vector<VarInfo> formVarList;
};
// 内情向量
struct DopeVector {
    unsigned int dim; // 维数
    unsigned int stride; // 一数组单元的大小
};

// 符号表项
typedef struct SymTableItem {
    unsigned int pre_item;
    Information info;
    wstring name; // 符号名
} SymTableItem;

class SymTable {
public:
    static unsigned int sp; // 指向当前子过程符号表的首地址
    static vector<SymTableItem> table; // 一个程序唯一的符号表
    static unsigned int display[PROC_CNT]; // 过程的嵌套层次表
public:
    SymTable();
    ~SymTable();
    // 创建子符号表
    static void mkTable();
    // 将变量名登入符号表
    static size_t enter(wstring name, size_t offset, Category cat);
    static void addWidth(unsigned int width);
    // 将过程名登入符号表
    static size_t enterProc(wstring name);
    // 查找符号在符号表中位置
    static size_t position(wstring name);
};

void symTableTest();
#endif