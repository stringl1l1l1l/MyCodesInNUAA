#include "Interpreter.h"
#include "PCode.h"
#include "PL0.h"
#include "SymTable.h"
int main()
{
    init();
    // PL0Test();
    string filename;
    wcout << L"请输入待编译的文件名称：" << endl;
    while (cin >> filename) {
        init();
        readFile2USC2("E:\\Programming\\GitHub\\repository\\DataStruct\\complierPL0\\"
            + filename + ".txt");
        analyze();
        // symTableTest();
        // PCodeList::printCode();
        Interpreter::run();
        wcout << L"请输入下一个待编译的文件名称, 或按Ctrl+C结束" << endl;
    }
}