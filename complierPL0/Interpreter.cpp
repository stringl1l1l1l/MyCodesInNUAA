#include "Interpreter.h"
#include "PL0.h"

size_t Interpreter::pc = 0;
size_t Interpreter::top = 0;
size_t Interpreter::sp = 0;
vector<int> Interpreter::running_stack;
vector<unsigned int> Interpreter::display(PROC_CNT, -1);

// 取常量a放入数据栈栈顶
void Interpreter::lit(Operation op, int L, int a)
{
    running_stack.push_back(a);
    pc++;
}

// 执行运算，a表示执行某种运算
void Interpreter::opr(Operation op, int L, int a)
{
    size_t cur_top = running_stack.size(); // cur_top表示当前运行栈栈顶，即有效数据上面一个单元
    // cur_top-1 为栈顶数据地址，cur_top-2为次栈顶数据地址
    if (a == 0) {
        // 执行返回并弹栈
    } else if (a == OPR_NEGTIVE) {
        // 栈顶取反(反码 + 1)
        running_stack[cur_top - 1] = ~running_stack[cur_top - 1] + 1;
    } else if (a == OPR_ADD) {
        // 次栈顶加栈顶
        int res = running_stack[cur_top - 2] + running_stack[cur_top - 1];
        // 弹栈两次
        running_stack.pop_back();
        running_stack.pop_back();
        // 新值入栈
        running_stack.push_back(res);
    } else if (a == OPR_SUB) {
        // 次栈顶减栈顶
        int res = running_stack[cur_top - 2] - running_stack[cur_top - 1];
        // 弹栈两次
        running_stack.pop_back();
        running_stack.pop_back();
        // 新值入栈
        running_stack.push_back(res);
    } else if (a == OPR_MULTI) {
        // 次栈顶乘栈顶
        int res = running_stack[cur_top - 2] * running_stack[cur_top - 1];
        // 弹栈两次
        running_stack.pop_back();
        running_stack.pop_back();
        // 新值入栈
        running_stack.push_back(res);
    } else if (a == OPR_DIVIS) {
        // 次栈顶除栈顶
        int res = running_stack[cur_top - 2] / running_stack[cur_top - 1];
        // 弹栈两次
        running_stack.pop_back();
        running_stack.pop_back();
        // 新值入栈
        running_stack.push_back(res);
    } else if (a == OPR_ODD) {
        // 栈顶元素为奇数结果为真
        running_stack[cur_top - 1] = (running_stack[cur_top - 1] & 0) == 0;
    } else if (a == OPR_EQL) {
        // 栈顶与次栈顶相等时结果为真
        bool res = running_stack[cur_top - 2] == running_stack[cur_top - 1];
        // 弹栈两次
        running_stack.pop_back();
        running_stack.pop_back();
        // 新值入栈
        running_stack.push_back(res);
    } else if (a == OPR_NEQ) {
        // 栈顶与次栈顶不相等时结果为真
        bool res = running_stack[cur_top - 2] != running_stack[cur_top - 1];
        // 弹栈两次
        running_stack.pop_back();
        running_stack.pop_back();
        // 新值入栈
        running_stack.push_back(res);
    } else if (a == OPR_LSS) {
        // 次栈顶 < 栈顶时结果为真
        bool res = running_stack[cur_top - 2] < running_stack[cur_top - 1];
        // 弹栈两次
        running_stack.pop_back();
        running_stack.pop_back();
        // 新值入栈
        running_stack.push_back(res);
    } else if (a == OPR_LEQ) {
        // 次栈顶 <= 栈顶时结果为真
        bool res = running_stack[cur_top - 2] <= running_stack[cur_top - 1];
        // 弹栈两次
        running_stack.pop_back();
        running_stack.pop_back();
        // 新值入栈
        running_stack.push_back(res);
    } else if (a == OPR_GRT) {
        // 次栈顶 > 栈顶时结果为真
        bool res = running_stack[cur_top - 2] > running_stack[cur_top - 1];
        // 弹栈两次
        running_stack.pop_back();
        running_stack.pop_back();
        // 新值入栈
        running_stack.push_back(res);
    } else if (a == OPR_GEQ) {
        // 次栈顶 >= 栈顶时结果为真
        bool res = running_stack[cur_top - 2] >= running_stack[cur_top - 1];
        // 弹栈两次
        running_stack.pop_back();
        running_stack.pop_back();
        // 新值入栈
        running_stack.push_back(res);
    }
    pc++;
}

// 根据层差和偏移量，取指定单元的数据放入栈顶
void Interpreter::lod(Operation op, int L, int a)
{
    pc++;
}

// 根据层差和偏移量，将栈顶数据存入指定单元，并弹栈
void Interpreter::sto(Operation op, int L, int a)
{
    running_stack.pop_back();
    pc++;
}

void Interpreter::cal(Operation op, int L, int a)
{
    pc++;
}

void Interpreter::alc(Operation op, int L, int a)
{
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
    if (!running_stack[running_stack.size() - 1])
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
    pc++;
}

// 将当前栈顶输出
void Interpreter::wrt(Operation op, int L, int a)
{
    wcout << running_stack[running_stack.size() - 1] << endl;
    // 弹栈
    running_stack.pop_back();
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
