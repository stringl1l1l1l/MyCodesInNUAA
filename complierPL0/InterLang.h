#ifndef _INTER_LANG_H
#define _INTER_LANG_H

#include <vector>
using namespace std;
enum Operation {
    add, // 加
    subtract, // 减
    multiply, // 乘
    divide, // 除
    assign, // 赋值
    j, // 无条件跳转
    jge, //>=跳转
    jgt, //>跳转
    jle, //<=跳转
    jlt, //<跳转
    call,
    read,
    write,
    ret,
    param,
};

struct Quaternation {
    Operation op;
    int arg1;
    int arg2;
    long long result;
};
struct ListNode {
    Quaternation quater;
    size_t next;
};

class List {
private:
    static size_t begin;
    static size_t end;
    static vector<ListNode> list;

public:
    List();
    ~List();
    static void emit(Operation op, int arg1, int arg2, long long result);
    static List* makelist(int);
    static List* merge(List*, List*);
    static void backpatch(List*, int);
};
#endif