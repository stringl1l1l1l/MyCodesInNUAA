#include "PCode.h"
#include "PL0.h"
#include "SymTable.h"
int main()
{
    PL0Test();
    symTableTest();
    PCodeList::printCode();
}