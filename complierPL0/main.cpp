#include "PCode.h"
#include "PL0.h"
#include "SymTable.h"
#include "Interpreter.h"
int main()
{
    PL0Test();
    symTableTest();
    PCodeList::printCode();
    Interpreter::run();
}