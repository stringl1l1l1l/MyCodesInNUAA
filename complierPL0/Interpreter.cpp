#include "Interpreter.h"
#include "PL0.h"

size_t Interpreter::pc = 0;
size_t Interpreter::top = 0;
size_t Interpreter::sp = 0;
vector<int> Interpreter::running_stack;
// 取常量a放入数据栈栈顶
void Interpreter::lit(Operation op, int L, int a)
{
    running_stack.push_back(a);
    top++;
    pc++;
}

// 执行运算，a表示执行某种运算
void Interpreter::opr(Operation op, int L, int a)
{
    // top-1为数据栈顶，top-2为数据次栈顶
    // opr 0 0 执行断点返回并弹栈
    if (a == OPR_RETURN) {
        // 恢复断点
        pc = running_stack[sp + RA];
        // 将当前过程活动记录全部弹栈
        for (int i = 0; i <= top - sp; i++) {
            running_stack.pop_back();
            top--;
        }
    } else if (a == OPR_NEGTIVE) {
        // 栈顶取反(反码 + 1)
        running_stack[top - 1] = ~running_stack[top - 1] + 1;
    } else if (a == OPR_ADD) {
        // 次栈顶加栈顶
        int res = running_stack[top - 2] + running_stack[top - 1];
        // 弹栈两次
        running_stack.pop_back();
        running_stack.pop_back();
        // 新值入栈
        running_stack.push_back(res);
        top--;
    } else if (a == OPR_SUB) {
        // 次栈顶减栈顶
        int res = running_stack[top - 2] - running_stack[top - 1];
        // 弹栈两次
        running_stack.pop_back();
        running_stack.pop_back();
        // 新值入栈
        running_stack.push_back(res);
        top--;
    } else if (a == OPR_MULTI) {
        // 次栈顶乘栈顶
        int res = running_stack[top - 2] * running_stack[top - 1];
        // 弹栈两次
        running_stack.pop_back();
        running_stack.pop_back();
        // 新值入栈
        running_stack.push_back(res);
        top--;
    } else if (a == OPR_DIVIS) {
        // 次栈顶除栈顶
        int res = running_stack[top - 2] / running_stack[top - 1];
        // 弹栈两次
        running_stack.pop_back();
        running_stack.pop_back();
        // 新值入栈
        running_stack.push_back(res);
        top--;
    } else if (a == OPR_ODD) {
        // 栈顶元素为奇数结果为真
        running_stack[top - 1] = (running_stack[top - 1] & 0) == 0;
    } else if (a == OPR_EQL) {
        // 栈顶与次栈顶相等时结果为真
        bool res = running_stack[top - 2] == running_stack[top - 1];
        // 弹栈两次
        running_stack.pop_back();
        running_stack.pop_back();
        // 新值入栈
        running_stack.push_back(res);
        top--;
    } else if (a == OPR_NEQ) {
        // 栈顶与次栈顶不相等时结果为真
        bool res = running_stack[top - 2] != running_stack[top - 1];
        // 弹栈两次
        running_stack.pop_back();
        running_stack.pop_back();
        // 新值入栈
        running_stack.push_back(res);
        top--;
    } else if (a == OPR_LSS) {
        // 次栈顶 < 栈顶时结果为真
        bool res = running_stack[top - 2] < running_stack[top - 1];
        // 弹栈两次
        running_stack.pop_back();
        running_stack.pop_back();
        // 新值入栈
        running_stack.push_back(res);
        top--;
    } else if (a == OPR_LEQ) {
        // 次栈顶 <= 栈顶时结果为真
        bool res = running_stack[top - 2] <= running_stack[top - 1];
        // 弹栈两次
        running_stack.pop_back();
        running_stack.pop_back();
        // 新值入栈
        running_stack.push_back(res);
        top--;
    } else if (a == OPR_GRT) {
        // 次栈顶 > 栈顶时结果为真
        bool res = running_stack[top - 2] > running_stack[top - 1];
        // 弹栈两次
        running_stack.pop_back();
        running_stack.pop_back();
        // 新值入栈
        running_stack.push_back(res);
        top--;
    } else if (a == OPR_GEQ) {
        // 次栈顶 >= 栈顶时结果为真
        bool res = running_stack[top - 2] >= running_stack[top - 1];
        // 弹栈两次
        running_stack.pop_back();
        running_stack.pop_back();
        // 新值入栈
        running_stack.push_back(res);
        top--;
    }
    pc++;
}

// 根据层级和偏移量，取指定单元的数据放入栈顶
void Interpreter::lod(Operation op, int L, int a)
{
    // 根据层级和偏移量，查找display表
    // running_stack[sp + DISPLAY + L]即指定层级L的活动记录基地址
    running_stack.push_back(running_stack[running_stack[sp + DISPLAY + L] + a]);
    pc++;
}

// 根据层级和偏移量，将栈顶数据存入指定单元，并弹栈
void Interpreter::sto(Operation op, int L, int a)
{
    // 根据层级和偏移量，查找display表
    // running_stack[sp + DISPLAY + L]即指定层级L的活动记录基地址
    running_stack[running_stack[sp + DISPLAY + L] + a] = running_stack[top - 1];
    // 读取数据后弹栈
    running_stack.pop_back();
    top--;
    pc++;
}

// 调用过程，先保存断点，然后调整sp
void Interpreter::cal(Operation op, int L, int a)
{
    // 预先开辟空间
    for (int i = 0; i < ACT_PRE_REC_SIZE + L + 1; i++)
        running_stack.push_back(0);
    // 保存断点
    running_stack[top + RA] = pc + 1;
    // 复制全局display的L+1个单元到即将开辟的活动记录
    // running_stack[sp + GLO_DIS]表示当前全局display表的基地址
    // top + DISPLAY表示即将开辟的活动记录的display表基地址
    for (int i = 0; i <= L; i++)
        running_stack[top + DISPLAY + i] = running_stack[running_stack[sp + GLO_DIS] + i];
    // 第L+1个单元是即将开辟的活动记录的基地址
    running_stack[top + DISPLAY + L] = top;
    // 记录并调整sp到即将开辟的活动记录
    running_stack[top + DL] = sp;
    sp = top;
    // 跳转
    pc = a;
}

// 在当前栈top处开辟a个内存单元，
void Interpreter::alc(Operation op, int L, int a)
{
    // 开辟空间时减去已经额外开辟的空间
    for (int i = 0; i < a - (running_stack.size() - top); i++)
        running_stack.push_back(0);
    // 数据栈顶与内存栈顶对齐
    top = running_stack.size();
    // 将新的display地址送到新的活动记录中的全局display处
    running_stack[sp + GLO_DIS] = sp + DISPLAY;
    pc++;
}

// 无条件跳转
void Interpreter::jmp(Operation op, int L, int a)
{
    pc = a;
}

// 当前栈顶条件为假时跳转
void Interpreter::jpc(Operation op, int L, int a)
{
    if (!running_stack[top - 1])
        pc = a;
    else
        pc++;
}

// 从命令行读取一个数据到栈顶
void Interpreter::red(Operation op, int L, int a)
{
    int data;
    wcin >> data;
    // 数据入栈
    running_stack.push_back(data);
    top++;
    pc++;
}

// 将当前栈顶输出
void Interpreter::wrt(Operation op, int L, int a)
{
    wcout << running_stack[top - 1] << endl;
    // 弹栈
    running_stack.pop_back();
    top--;
    pc++;
}

void Interpreter::run()
{
    // 按照pc的指示运行程序
    for (int i = 0; i < PCodeList::code_list.size(); i = pc) {
        wcout << pc << endl;
        PCode code = PCodeList::code_list[i];
        switch (code.op) {
        case Operation::lit:
            lit(code.op, code.L, code.a);
            break;
        case Operation::opr:
            opr(code.op, code.L, code.a);
            break;
        case Operation::load:
            lod(code.op, code.L, code.a);
            break;
        case Operation::store:
            sto(code.op, code.L, code.a);
            break;
        case Operation::call:
            cal(code.op, code.L, code.a);
            break;
        case Operation::alloc:
            alc(code.op, code.L, code.a);
            break;
        case Operation::jmp:
            jmp(code.op, code.L, code.a);
            break;
        case Operation::jpc:
            jpc(code.op, code.L, code.a);
            break;
        case Operation::red:
            red(code.op, code.L, code.a);
            break;
        case Operation::wrt:
            wrt(code.op, code.L, code.a);
            break;
        default:
            break;
        }
    }
}
