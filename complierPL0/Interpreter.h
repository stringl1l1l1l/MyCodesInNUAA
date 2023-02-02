#ifndef _INTER_H
#define _INTER_H
#include "PCode.h"
#include <vector>

using namespace std;

// P代码的解释器
class Interpreter {
public:
    static size_t pc;
    static size_t top;
    static size_t sp;
    static vector<int> running_stack;
    static vector<unsigned int> display;

    static void run();
private:
    static void lit(Operation op, int L, int a);
    static void opr(Operation op, int L, int a);
    static void lod(Operation op, int L, int a);
    static void sto(Operation op, int L, int a);
    static void cal(Operation op, int L, int a);
    static void alc(Operation op, int L, int a);
    static void jmp(Operation op, int L, int a);
    static void jpc(Operation op, int L, int a);
    static void red(Operation op, int L, int a);
    static void wrt(Operation op, int L, int a);
};

#endif