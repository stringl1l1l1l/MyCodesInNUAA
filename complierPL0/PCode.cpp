#include "PCode.h"
#include <iostream>
using namespace std;

size_t PCodeList::pc;
size_t PCodeList::base;
vector<PCode> PCodeList::code_list;
vector<unsigned int> PCodeList::running_stack;

wstring op_map[P_CODE_CNT] = {
    L"LIT",
    L"OPR",
    L"LOD",
    L"STO",
    L"CAL",
    L"INT",
    L"JMP",
    L"JPC",
    L"RED",
    L"WRT"
};

void PCodeList::emit(Operation op, int L, int a)
{
    code_list.push_back(PCode(op, L, a));
}

void PCodeList::printCode()
{
    for (PCode mem : code_list) {
        wcout << op_map[mem.op] << L", " << mem.L << L", " << mem.a << endl;
    }
}