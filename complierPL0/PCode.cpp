#include "PCode.h"
#include <iostream>
using namespace std;
string func2str[P_CODE_CNT] = {
    "LIT",
    "OPR",
    "LOD",
    "STO",
    "CAL",
    "INT",
    "JMP",
    "JPC",
    "RED",
    "WRT"
};

void gen(Function f, unsigned int level, unsigned int addr)
{
}