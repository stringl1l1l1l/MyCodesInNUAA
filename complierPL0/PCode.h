#ifndef _P_CODE_H
#define _P_CODE_H
#include <string>
using namespace std;
#define P_CODE_CNT 10
// 中间代码指令集
enum Function {
    LIT,
    OPR,
    LOD,
    STO,
    CAL,
    INT,
    JMP,
    JPC,
    RED,
    WRT
};

struct PCode {
    Function function; // 伪操作码
    unsigned int level; // 层差
    unsigned int addr; // 相对地址
};

void gen(Function f, unsigned int level, unsigned int addr);

#endif