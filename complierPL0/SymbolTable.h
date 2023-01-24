#include "PL0.h"
#include <vector>

using namespace std;

struct VarInfo {
    enum Category cat; // 种属
    enum Type type; // 类型
    unsigned int size; // 占用存储单元数
    unsigned int offset; // 相对地址（偏移量）
    bool isFormVar; // 是否为形参的标记
    bool hasAssigned; // 是否进行过赋值的标记
    void* extraInfo;
};

struct ProcInfo {
    vector<VarInfo> formVarList;
};

// 内情向量
struct DopeVector {
    unsigned int dim; // 维数
    unsigned int stride; // 一数组单元的大小
};

// 符号表项
struct SymTableItem {    SymTableItem* pre_item;
    void* info;
    char name[ID_MAX + 1]; // 符号名
};
