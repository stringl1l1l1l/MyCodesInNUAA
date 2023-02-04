#include "compiler.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <vector>

// #include"lex.h"
using namespace std;
// char SYM;	用ca[p]代替;
struct nametable {
    string name;
    string information;
    int val; // const与call时时需要
    int link;
    int call_need; // 如果是procedure需要调用时的形参个数
};
vector<tuple<string, int, int>> CODE;
int Mid_p = 0; // 代码指针
int call_p; // 在调用时来记录返回地址
int act_sp; // sp指针
int static_p; // 静态链指针
int act_top = -1;
int call_number = 0;
vector<int> act(100000);
stack<int> data_s; // 单独的数据栈

void gen(string op, int x, int y)
{
    tuple<string, int, int> t(op, x, y);
    CODE.push_back(t);
}
stack<string> S, S1; // 存储运算符
const int tablespace = 100; // 符号表长度
bool check();
bool isaop();
bool isexp();
bool islop();
bool islexp();
bool isblock();
bool isbody();
bool isconst();
bool isvardecl();
bool iscondecl();
bool isprog();
bool isproc();
bool isstatement();
bool isterm();
bool isfactor();
char line;
char ch;
int p; // 指向第p+1个单词;
fstream input; // 测试文档
ofstream output("out.txt", ios::out); // 中间代码
int length;
vector<string> ca; // 字符类型
vector<string> st; // 字符名字
vector<int> sh; // 行号
struct nametable nametab[tablespace]; // 栈符号表
int display[tablespace]; // display表(指向btab)
int btable[tablespace][2]; // 每个[][0]最后一个形参位置;[][1]最后一个名字
int display_val = 0;
int level = 0;
// 符号表{+,-,*,/,=,<>,<,<=,>,>=};
int btab_key = 0;
int name_key = 0;
// int information_key = 0;
int link_val = 0;
string table_temp[15] = { "begin", "call", "const",
    "do", "end", "if", "odd",
    "procedure", "read", "then", "var",
    "while", "write", "program", "else" };
vector<string> table; // 保留字表
string strToken;
bool check()
{ // 同层有无重复定义
    int last = btable[display[level]][1];
    while (last != 0) {
        if (nametab[last].name == st[p]) {
            cout << nametab[last].name << "----" << level << "----";
            return 1; // 发现重复
        }
        last = nametab[last].link;
    }
    return 0; // 0为无重复
}
int check_proc(string name)
{
    int temp, last;
    for (temp = 0; temp <= level; temp++) {
        last = btable[display[level - temp]][1];
        while (last != 0) {
            if (nametab[last].name == name) { // 可能存在变量
                // res.push_back(temp);res.push_back(var_number);
                if (nametab[last].information == "procedure") {
                    gen("CAL", temp - 1, nametab[last].val); //***层数差与变量稍有不同
                    return nametab[last].call_need;
                }
            }
            last = nametab[last].link;
        }
    }

    return -1;
}
int count_var(int level_temp)
{ // 计算此层级有几个变量
    int res = 0;
    int last = btable[display[level_temp]][1];
    while (last != 0) {
        if (nametab[last].information == "var") {
            res++;
        }
        last = nametab[last].link;
    }
    return res;
}
int count_can(int level_temp)
{
    int res = 0;
    int last = btable[display[level_temp]][0];
    while (last != 0) {
        if (nametab[last].information == "var") {
            res++;
        }
        last = nametab[last].link;
    }
    return res;
}
bool var_loc(string var, int flag1)
{ // 返回位置[0]嵌套层差/[1]在该层的对应深度//为找到则返回-1;flag1=1时是写入操作，0是读操作
    int temp, last, var_number;
    for (temp = 0; temp <= level; temp++) {
        last = btable[display[level - temp]][1];
        var_number = count_var(level - temp);
        while (last != 0) {
            if (nametab[last].name == var) { // 可能存在变量
                // res.push_back(temp);res.push_back(var_number);
                if (nametab[last].information == "var") {
                    if (flag1 == 0)
                        gen("LOD", temp, var_number + 2);
                    else
                        gen("STO", temp, var_number + 2);
                    return 1;
                } else if (nametab[last].information == "const" && flag1 == 0) {
                    gen("LIT", 0, nametab[last].val);
                    return 1;
                }
            }
            if (nametab[last].information == "var")
                var_number--;
            last = nametab[last].link;
        }
    }
    cout << var << "使用时未先声明" << endl;
    return 0;
}
void Getchar()
{
    ch = input.get();
}
void GetBC()
{
    while (ch == ' ' || ch == '	')
        Getchar();
}
bool IsLetter()
{
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
        return 1;
    else
        return 0;
}
bool IsDigit()
{
    if (ch >= '0' && ch <= '9')
        return 1;
    else
        return 0;
}
void Contact()
{
    strToken += ch;
}
int Reserve()
{
    for (int i = 0; i < table.size(); i++) {
        if (table[i] == strToken)
            return i + 1;
    }
    return 0;
}
void InsertId()
{
}
void Retract()
{
}
void lex()
{
    int count_line = 1;
    int code;
    string temp;
    input.open("test.txt");
    Getchar();
    while (ch != EOF) {
        strToken = "";
        GetBC();
        if (ch == '\n') {
            count_line++;
        }
        if (IsLetter()) {
            while (IsLetter() || IsDigit()) {
                Contact();
                Getchar();
            }
            // Retract();
            code = Reserve();
            if (code == 0) {
                ca.push_back("id");
                st.push_back(strToken);
                sh.push_back(count_line);
                cout << "id"
                     << "-- " << strToken << endl;
            } else {
                ca.push_back(table[code - 1]);
                st.push_back(table[code - 1]);
                sh.push_back(count_line);
                cout << table[code - 1] << "-- " << strToken << endl;
            }
        } else if (IsDigit()) {
            while (IsDigit()) {
                Contact();
                Getchar();
            }
            // Retract();
            ca.push_back("integer");
            st.push_back(strToken);
            sh.push_back(count_line);
            cout << "integer"
                 << "-- " << strToken << endl;
        } else if (ch == '=') {
            ca.push_back("=");
            st.push_back("=");
            sh.push_back(count_line);
            cout << "=--" << endl;
            Getchar();
        } else if (ch == '+') {
            ca.push_back("+");
            st.push_back("+");
            sh.push_back(count_line);
            cout << "+--" << endl;
            Getchar();
        } else if (ch == '-') {
            ca.push_back("-");
            st.push_back("-");
            sh.push_back(count_line);
            cout << "---" << endl;
            Getchar();
        } else if (ch == '*') {
            ca.push_back("*");
            st.push_back("*");
            sh.push_back(count_line);
            cout << "*--" << endl;
            Getchar();
        } else if (ch == '/') {
            ca.push_back("/");
            st.push_back("/");
            sh.push_back(count_line);
            cout << "/--" << endl;
            Getchar();
        } else if (ch == ':') {
            Getchar();
            if (ch == '=') {
                Getchar();
                ca.push_back(":=");
                st.push_back(":=");
                sh.push_back(count_line);
                cout << "becomes"
                     << "--"
                     << ":=" << endl;
            } else {
                ca.push_back("error");
                st.push_back("error");
                sh.push_back(count_line);
                cout << "error" << endl;
            }

        } else if (ch == '(') {
            ca.push_back("(");
            st.push_back("(");
            sh.push_back(count_line);
            cout << ch << "--" << endl;
            Getchar();
        } else if (ch == ')') {
            ca.push_back(")");
            st.push_back(")");
            sh.push_back(count_line);
            cout << ch << "--" << endl;
            Getchar();
        } else if (ch == ',') {
            ca.push_back(",");
            st.push_back(",");
            sh.push_back(count_line);
            cout << ","
                 << "--" << endl;
            Getchar();
        } else if (ch == ';') {
            ca.push_back(";");
            st.push_back(";");
            sh.push_back(count_line);
            cout << ch << "--" << endl;
            Getchar();
        } else if (ch == '.') {
            ca.push_back(".");
            st.push_back(".");
            sh.push_back(count_line);
            cout << ch << ":" << endl;
            Getchar();
        } else if (ch == '<') {
            Getchar();
            if (ch == '>') {
                ca.push_back("<>");
                st.push_back("<>");
                sh.push_back(count_line);
                cout << "neq:" << ch << endl;
                Getchar();
            } else if (ch == '=') {
                ca.push_back("<=");
                st.push_back("<=");
                sh.push_back(count_line);
                cout << "<=:" << endl;
                Getchar();
            } else {
                cout << "<:" << endl;
                ca.push_back("<");
                st.push_back("<");
                sh.push_back(count_line);
            }

        } else if (ch == '>') {
            Getchar();
            if (ch == '=') {
                Getchar();
                cout << ">=--" << endl;
                ca.push_back(">=");
                st.push_back(">=");
                sh.push_back(count_line);
            } else {
                ca.push_back(">");
                st.push_back(">");
                sh.push_back(count_line);
                cout << ">--" << endl;
            }

        } else if (ch == '\n')
            Getchar();
        else {
            ca.push_back("error");
            st.push_back("error");
            sh.push_back(count_line);
            cout << "error--" << ch << endl;
            Getchar();
        }
    }
}
bool isaop()
{
    if (ca[p] == "+" || ca[p] == "-") {
        S.push(ca[p]);
        p++;
        return 1;
    } else {
        return 0;
    }
}
bool isexp()
{
    if (ca[p] == "+" || ca[p] == "-") {
        if (ca[p] == "-") {
            S.push("--");
        }
        p++;
    }
    if (isterm()) {
        while (isaop()) {
            if (!isterm()) {
                return 0;
            }
            if (S.top() == "+") {
                gen("OPR", 0, 2);
            } else {
                gen("OPR", 0, 3);
            }
            S.pop();
        }
    } else {
        return 0;
    }
    if (S.size() != 0) {
        if (S.top() == "--") {
            gen("OPR", 0, 1);
            S.pop();
        }
    }
    return 1;
}
bool islop()
{
    if (ca[p] == "=" || ca[p] == "<>" || ca[p] == "<" || ca[p] == "<=" || ca[p] == ">" || ca[p] == ">=") {
        S.push(ca[p]);
        p++;
        return 1;
    } else {
        cout << p << "---" << ca[p] << "---lopError" << endl;
        return 0;
    }
}
bool islexp()
{
    if (isexp()) {
        if (islop()) {
            if (isexp()) {
                if (S.top() == "=") {
                    gen("OPR", 0, 8);
                    S.pop();
                } else if (S.top() == "<>") {
                    gen("OPR", 0, 9);
                    S.pop();
                } else if (S.top() == "<") {
                    gen("OPR", 0, 10);
                    S.pop();
                } else if (S.top() == "<=") {
                    gen("OPR", 0, 13);
                    S.pop();
                } else if (S.top() == ">") {
                    gen("OPR", 0, 12);
                    S.pop();
                } else if (S.top() == ">=") {
                    gen("OPR", 0, 11);
                }
                return 1;
            } else {
                return 0;
            }
        } else
            return 0;
    }
    if (ca[p] == "odd") {
        p++;
        if (isexp()) {
            gen("OPR", 0, 6);
            return 1;
        } else
            return 0;
    }
    return 0;
}
// block里面的proc层数＋
bool isblock()
{
    int flag = 1;
    int can_number = 0;
    int code_p1 = 0;
    code_p1 = CODE.size();
    gen("JMP", 0, -1);
    if (ca[p] == "const") {
        if (!iscondecl()) {
            cout << "block中const错误" << endl;
            flag = 0;
        }
    }
    if (ca[p] == "var") {
        if (!isvardecl()) {
            flag = 0;
        }
    }
    if (ca[p] == "procedure") { // 改成while？x
        string proc_temp = st[p];
        if (!isproc()) {
            flag = 0;
        }
    }
    get<2>(CODE[code_p1]) = CODE.size();
    gen("INT", 0, count_var(level) + 3); // 生成活动记录表空间
    can_number = count_can(level);
    /*for (int i = can_number + 2; i >2; i--)
    {
            gen("STO", 0, i);
    }*/
    // 形参入活动记录
    if (!isbody()) {
        flag = 0;
    }
    return flag;
}
bool isbody()
{
    int flag = 1;
    if (ca[p] != "begin") {
        cout << sh[p] << "------body中begin错误" << endl;
        flag = 0;
    } else {
        p++;
    }
    if (!isstatement()) {
        flag = 0;
    }
    while (ca[p] == ";") {
        p++;
        /*/***2.1
        if (ca[p] == "end") {
                flag = 0;
                cout << sh[p] << "行------可能有多余的;" << endl;//***这边进行了猜测
                break;
        }
        */
        if (!isstatement()) {
            flag = 0;
        }
    }
    if (ca[p] != "end") {
        cout << sh[p] << "------body中end错误或缺少;" << endl;
        flag = 0;
    } else
        p++;
    return flag;
}
bool isconst()
{
    /*	if (ca[p] != "const") {
                    cout << "const缺少标识const" << endl;
                    return 0;
            }
            p++;
    */
    // 错误代码
    int flag = 1;
    if (ca[p] != "id") {
        cout << sh[p] << "------const中id错误" << endl;
        flag = 0;
    }
    if (check()) {
        cout << sh[p] << "------同层次常量重复申明" << endl;
        flag = 0;
    }
    nametab[name_key].name = st[p]; //***20:17
    // cout << name_key << "---" << nametab[name_key].name << endl;
    nametab[name_key].information = "const";
    nametab[name_key].link = btable[display[level]][1]; //***20:26此处应为0
    btable[btab_key][1] = name_key;
    // link_val = name_key;

    p++;
    if (ca[p] != ":=") {
        cout << sh[p] << "------const中:=错误" << endl;
        flag = 0;
    }
    p++;
    if (ca[p] != "integer") {
        cout << sh[p] << "------const中integer错误" << endl;
        flag = 0;
    }
    nametab[name_key].val = stoi(st[p]);
    name_key++;
    p++;
    return flag;
}
bool isvardecl()
{
    int flag = 1;
    if (ca[p] != "var") {
        cout << sh[p] << "------var错误" << endl;
        flag = 0;
    }
    p++;
    if (ca[p] != "id") {
        cout << sh[p] << "------id错误" << endl;
        flag = 0;
    }
    if (check()) {
        cout << sh[p] << "------同层次变量重复申明" << endl;
        flag = 0;
    }
    nametab[name_key].name = st[p]; //***21:13
    nametab[name_key].information = "var";
    nametab[name_key].link = btable[display[level]][1]; //
    btable[display[level]][1] = name_key;
    // link_val = name_key;
    name_key++;
    p++;
    while (ca[p] == ",") {
        p++;
        if (ca[p] != "id") {
            cout << sh[p] << "------varcedl中,后错误" << endl;
            flag = 0;
        }
        if (check()) {
            cout << sh[p] << "------同层次变量重复申明" << endl;
            flag = 0;
        }
        nametab[name_key].name = st[p];
        nametab[name_key].information = "var";
        nametab[name_key].link = btable[display[level]][1]; //
        btable[display[level]][1] = name_key;
        name_key++;
        p++;
    }
    if (ca[p] != ";") {
        cout << sh[p] << "行-----varcedl缺少;" << endl;
        flag = 0;
    } else
        p++;
    return flag;
}
bool iscondecl()
{
    if (ca[p] != "const")
        return 0;
    p++;
    if (!isconst())
        return 0;
    // p++;
    while (ca[p] == ",") {
        p++;
        if (!isconst()) //||p>=length
            return 0;
    }
    if (ca[p] != ";") {
        cout << sh[p] << "行------const缺少;" << endl;
        return 0;
    }
    p++;
    return 1;
}
bool isprog()
{
    int flag = 1;
    if (ca[p] != "program")
        flag = 0;
    p++;
    if (ca[p] != "id")
        flag = 0;
    nametab[name_key].name = st[p];
    btable[btab_key][0] = name_key; // 0-0
    btable[btab_key][1] = name_key; // program的表0-0
    // 1/29	在0存p？
    name_key++;
    display[level] = btab_key; // 此处应为0-0
    p++;
    if (ca[p] != ";")
        flag = 0;
    else {
        p++;
    }
    if (!isblock())
        return 0;
    gen("OPR", 0, 0);
    return flag;
}
bool isproc()
{
    int flag = 1;
    int call_need = 0;
    int temp2;
    if (ca[p] != "procedure") {
        cout << sh[p] << "------proc中procedure错误" << endl;
        flag = 0;
    }

    p++;
    if (ca[p] != "id") {
        cout << sh[p] << "行------proc中缺少函数名错误" << endl;
        flag = 0;
    } else if (check()) {
        cout << sh[p] << "行---procedure-名字重复定义" << endl;
        flag = 0;
    }
    temp2 = name_key;
    nametab[name_key].name = st[p];
    nametab[name_key].information = "procedure";
    nametab[name_key].link = btable[display[level]][1];
    nametab[name_key].val = CODE.size(); // bug?
    btable[display[level]][1] = name_key;
    name_key++;
    p++;

    if (ca[p] != "(") {
        cout << sh[p] << "------ proc中(错误";
        flag = 0;
    }
    btab_key++; // 向下生长
    level++; // 遇到了新的proc
    display[level] = btab_key; // 压入新的display
    btable[btab_key][0] = 0;
    btable[btab_key][1] = 0;
    p++;
    if (ca[p] == "id") {
        if (check()) {
            cout << sh[p] << "行函数" << st[p] << "形参重复定义" << endl;
            flag = 0;
        }
        call_need++;
        nametab[name_key].link = btable[display[level]][1];
        nametab[name_key].name = st[p];
        nametab[name_key].information = "var";
        btable[btab_key][0] = name_key;
        btable[btab_key][1] = name_key;
        name_key++;
        p++;
        while (ca[p] == ",") {
            p++;
            if (ca[p] != "id") {
                cout << sh[p] << "------proc中id缺少" << endl;
                flag = 0;
            }
            if (check()) {
                cout << sh[p] << "行函数" << st[p] << "形参重复定义" << endl;
                flag = 0;
            }
            call_need++;
            nametab[name_key].link = btable[display[level]][1];
            nametab[name_key].name = st[p];
            nametab[name_key].information = "var";
            btable[btab_key][0] = name_key;
            btable[btab_key][1] = name_key;
            name_key++;
            p++;
        }
    }
    if (ca[p] != ")") {
        cout << sh[p] << "------proc中)错误" << endl;
        flag = 0;
    }
    nametab[temp2].call_need = call_need;
    p++;
    if (ca[p] != ";") {
        cout << sh[p] << "------proc中;错误" << endl;
        flag = 0;
    }
    p++;
    if (!isblock()) {
        flag = 0;
    }
    // 输出：
    /*int temp = 0;
    temp = btable[display[level]][1];
    cout << "level=" << level << "------------------" << endl;
    cout << display[level] << endl;
    while (temp != 0) {
            cout << temp << "---" << nametab[temp].name << "---" << nametab[temp].information << "---" << nametab[temp].link << endl;
            temp = nametab[temp].link;
    }
    cout << "------------------" << endl;
    */
    gen("OPR", 0, 0);
    level--; // 退出当前proc，display出栈
    if (ca[p] == ";") { // 是while?
        p++;
        if (!isproc()) {
            flag = 0;
        }
    }
    return flag;
}
bool isstatement()
{
    int flag = 1;
    if (ca[p] == "id") {
        S1.push(st[p]);
        p++;
        if (ca[p] == ":=") {
            p++;
            if (isexp()) {
                flag = var_loc(S1.top(), 1);
                S1.pop();
                return flag;
            }
            cout << sh[p] << "-----idexpError" << endl;
            return 0;
        }
        cout << sh[p] << "------id:=Error" << endl;
        return 0;
    }
    if (ca[p] == "if") {
        int code_p1, code_p2;
        p++;
        if (islexp()) {
            code_p1 = CODE.size();
            gen("JPC", 0, -1); // 指向else开始？
            if (ca[p] == "then") {
                p++;
                if (isstatement()) {
                    code_p2 = CODE.size();
                    if (ca[p] == "else") {
                        gen("JMP", 0, -1); // 指向else结束
                        get<2>(CODE[code_p1]) = code_p2 + 1;
                        p++;
                        if (!isstatement()) {
                            cout << sh[p] << "---else-statement错误" << endl;
                            return 0;
                        } else {
                            get<2>(CODE[code_p2]) = CODE.size();
                        }
                    } else {
                        get<2>(CODE[code_p1]) = code_p2;
                    }
                    return 1;
                }
                return 0;
            }
            cout << sh[p] << "------ifthenError" << endl;
            return 0;
        }
        cout << sh[p] << "------if-lexp错误" << endl;
        return 0;
    }
    if (ca[p] == "while") {
        int code_p1, code_p2;
        code_p2 = CODE.size(); // 循环入口
        p++;
        if (islexp()) {
            code_p1 = CODE.size();
            gen("JPC", 0, -1);
            if (ca[p] == "do") {
                p++;
                if (isstatement()) {
                    gen("JMP", 0, code_p2);
                    get<2>(CODE[code_p1]) = CODE.size();
                    return 1;
                }
                cout << sh[p] << "------statement-while-statement错误" << endl;
                return 0;
            }
            cout << sh[p] << "------while-do错误" << endl;
            return 0;
        }
        wcout << sh[p] << "------while-exp错误" << endl;
        return 0;
    }
    if (ca[p] == "call") {
        string proc1;
        int res_number;
        call_number = 0;
        p++;
        if (ca[p] == "id") {
            proc1 = st[p];
            p++;
            if (ca[p] == "(") {
                p++;
                if (isexp()) {
                    gen("STO", -1, 3);
                    call_number++;
                    while (ca[p] == ",") {
                        p++;
                        if (!isexp()) {
                            cout << sh[p] << "------call缺少exp" << endl;
                            return 0;
                        }
                        call_number++;
                        gen("STO", -1, call_number + 2);
                    }
                }
                if (ca[p] == ")") {
                    res_number = check_proc(proc1);
                    if (res_number == -1) {
                        flag = 0;
                        cout << sh[p] << "行------" << proc1 << "使用时未先声明" << endl;
                    } else if (res_number != call_number) {
                        flag = 0;
                        cout << sh[p] << "形参个数不匹配,预期输入:" << res_number << "  实际输入:" << call_number << endl;
                    }
                    p++;
                    return flag;
                } else {
                    cout << sh[p] << "------call缺少)" << endl;
                    return 0;
                }
            } else {
                cout << sh[p] << "------call缺少(" << endl;
                return 0;
            }
        } else {
            cout << sh[p] << "------callid错误" << endl;
            return 0;
        }
    }
    if (ca[p] == "begin") {
        if (isbody())
            return 1;
        else {
            return 0;
        }
    }
    if (ca[p] == "read") {
        p++;
        if (ca[p] == "(") {
            p++;
            if (ca[p] != "id") {
                cout << sh[p] << "---"
                     << "---readid错误" << endl;
                return 0;
            }

            gen("OPR", 0, 16);
            flag = var_loc(st[p], 1);
            p++;
            while (ca[p] == ",") {
                p++;
                if (ca[p] == "id") {
                    gen("OPR", 0, 16);
                    flag = var_loc(st[p], 1) & flag;
                    p++;
                } else {
                    cout << sh[p] << "---" << ca[p] << "---read缺少id" << endl;
                    return 0;
                }
            }
            if (ca[p] == ")") {
                p++;
                return flag;
            } else {
                cout << sh[p] << "------read缺少)" << endl;
                return 0;
            }
        }
        cout << sh[p] << "------read缺少(" << endl;
        return 0;
    }
    if (ca[p] == "write") { // 输出至屏幕
        p++;
        if (ca[p] == "(") {
            p++;
            if (isexp()) {
                gen("OPR", 0, 14);
                while (ca[p] == ",") {
                    p++;
                    if (isexp()) {
                        gen("OPR", 0, 14);
                    } else {
                        cout << sh[p] << "---write参数错误" << endl;
                        return 0;
                    }
                }
                if (ca[p] == ")") {
                    p++;
                    return 1;
                } else {
                    cout << sh[p] << "行------write)缺少" << endl;
                    return 0;
                }
            }
            return 0;
        } else {
            cout << sh[p] << "---" << ca[p] << "---statement-write(Error" << endl;
            return 0;
        }
    } else {
        cout << sh[p] << "行---" << ca[p] << "---statement无法识别的开头类型" << endl;
        return 0;
    }
}
// bool iscondition() {}
bool isterm()
{
    if (isfactor()) {
        while (ca[p] == "*" || ca[p] == "/") {
            S.push(ca[p]);
            p++;
            if (!isfactor()) {
                cout << sh[p] << "行"
                     << "乘除没有右半值" << endl;
                return 0;
            }
            if (S.top() == "*") {
                gen("OPR", 0, 4);
            } else {
                gen("OPR", 0, 5);
            }
            S.pop();
        }
        return 1;
    }
    // cout << "termfactor1Error" << endl;
    return 0;
}
bool isfactor()
{
    int flag = 1;
    if (ca[p] == "id") {
        flag = var_loc(st[p], 0); // 生成放入数据栈代码
        p++;
        return flag;
    }

    else if (ca[p] == "integer") {
        gen("LIT", 0, stoi(st[p]));
        p++;
        return 1;
    }

    else if (ca[p] == "(") {
        p++;
        if (isexp()) {
            if (ca[p] == ")") {
                p++;
                return 1;
            } else {
                cout << "表达式缺少)" << endl;
                return 0;
            }
        } else {
            cout << "缺少表达式" << endl;
            return 0;
        }
    } else {
        // cout << "factorError" << endl;
        return 0;
    }
}
int main()
{
    for (int i = 0; i < 15; i++) {
        table.push_back(table_temp[i]);
    }
    lex();
    p = 0;
    //	ca = lexi();
    length = ca.size();
    cout << "词法分析完成" << endl;
    cout << "----------------------------------------" << endl;
    for (int i = 0; i < length; i++) {
        cout << sh[i] << "---" << i << "---" << st[i] << "---" << ca[i] << endl;
    }
    cout << "----------------------------------------" << endl;
    int result = 1;
    result = isprog();
    cout << "----------------------------------------" << endl;
    if (p != length && result == 1)
        cout << "有多余的字符输入" << endl;
    else if (result == 1) {
        cout << "语法分析正确" << endl;
        cout << "----------------------------------------" << endl;
        cout << "中间代码生成如下" << endl;
        for (int i = 0; i < CODE.size(); i++) {
            cout << setw(2) << i << "  " << get<0>(CODE[i]) << "  " << get<1>(CODE[i]) << "  " << get<2>(CODE[i]) << endl;
            output << get<0>(CODE[i]) << "  " << get<1>(CODE[i]) << "  " << get<2>(CODE[i]) << endl;
        }
        cout << "----------------------------------------" << endl;
        call_p = CODE.size();
        while (Mid_p < CODE.size()) {
            Compiler(CODE[Mid_p]);
        }
    } else if (result == 0)
        cout << "请进行修改" << endl;
    return result;
}
