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
// char SYM;	��ca[p]����;
struct nametable {
    string name;
    string information;
    int val; // const��callʱʱ��Ҫ
    int link;
    int call_need; // �����procedure��Ҫ����ʱ���βθ���
};
vector<tuple<string, int, int>> CODE;
int Mid_p = 0; // ����ָ��
int call_p; // �ڵ���ʱ����¼���ص�ַ
int act_sp; // spָ��
int static_p; // ��̬��ָ��
int act_top = -1;
int call_number = 0;
vector<int> act(100000);
stack<int> data_s; // ����������ջ

void gen(string op, int x, int y)
{
    tuple<string, int, int> t(op, x, y);
    CODE.push_back(t);
}
stack<string> S, S1; // �洢�����
const int tablespace = 100; // ���ű���
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
int p; // ָ���p+1������;
fstream input; // �����ĵ�
ofstream output("out.txt", ios::out); // �м����
int length;
vector<string> ca; // �ַ�����
vector<string> st; // �ַ�����
vector<int> sh; // �к�
struct nametable nametab[tablespace]; // ջ���ű�
int display[tablespace]; // display��(ָ��btab)
int btable[tablespace][2]; // ÿ��[][0]���һ���β�λ��;[][1]���һ������
int display_val = 0;
int level = 0;
// ���ű�{+,-,*,/,=,<>,<,<=,>,>=};
int btab_key = 0;
int name_key = 0;
// int information_key = 0;
int link_val = 0;
string table_temp[15] = { "begin", "call", "const",
    "do", "end", "if", "odd",
    "procedure", "read", "then", "var",
    "while", "write", "program", "else" };
vector<string> table; // �����ֱ�
string strToken;
bool check()
{ // ͬ�������ظ�����
    int last = btable[display[level]][1];
    while (last != 0) {
        if (nametab[last].name == st[p]) {
            cout << nametab[last].name << "----" << level << "----";
            return 1; // �����ظ�
        }
        last = nametab[last].link;
    }
    return 0; // 0Ϊ���ظ�
}
int check_proc(string name)
{
    int temp, last;
    for (temp = 0; temp <= level; temp++) {
        last = btable[display[level - temp]][1];
        while (last != 0) {
            if (nametab[last].name == name) { // ���ܴ��ڱ���
                // res.push_back(temp);res.push_back(var_number);
                if (nametab[last].information == "procedure") {
                    gen("CAL", temp - 1, nametab[last].val); //***��������������в�ͬ
                    return nametab[last].call_need;
                }
            }
            last = nametab[last].link;
        }
    }

    return -1;
}
int count_var(int level_temp)
{ // ����˲㼶�м�������
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
{ // ����λ��[0]Ƕ�ײ��/[1]�ڸò�Ķ�Ӧ���//Ϊ�ҵ��򷵻�-1;flag1=1ʱ��д�������0�Ƕ�����
    int temp, last, var_number;
    for (temp = 0; temp <= level; temp++) {
        last = btable[display[level - temp]][1];
        var_number = count_var(level - temp);
        while (last != 0) {
            if (nametab[last].name == var) { // ���ܴ��ڱ���
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
    cout << var << "ʹ��ʱδ������" << endl;
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
// block�����proc������
bool isblock()
{
    int flag = 1;
    int can_number = 0;
    int code_p1 = 0;
    code_p1 = CODE.size();
    gen("JMP", 0, -1);
    if (ca[p] == "const") {
        if (!iscondecl()) {
            cout << "block��const����" << endl;
            flag = 0;
        }
    }
    if (ca[p] == "var") {
        if (!isvardecl()) {
            flag = 0;
        }
    }
    if (ca[p] == "procedure") { // �ĳ�while��x
        string proc_temp = st[p];
        if (!isproc()) {
            flag = 0;
        }
    }
    get<2>(CODE[code_p1]) = CODE.size();
    gen("INT", 0, count_var(level) + 3); // ���ɻ��¼��ռ�
    can_number = count_can(level);
    /*for (int i = can_number + 2; i >2; i--)
    {
            gen("STO", 0, i);
    }*/
    // �β�����¼
    if (!isbody()) {
        flag = 0;
    }
    return flag;
}
bool isbody()
{
    int flag = 1;
    if (ca[p] != "begin") {
        cout << sh[p] << "------body��begin����" << endl;
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
                cout << sh[p] << "��------�����ж����;" << endl;//***��߽����˲²�
                break;
        }
        */
        if (!isstatement()) {
            flag = 0;
        }
    }
    if (ca[p] != "end") {
        cout << sh[p] << "------body��end�����ȱ��;" << endl;
        flag = 0;
    } else
        p++;
    return flag;
}
bool isconst()
{
    /*	if (ca[p] != "const") {
                    cout << "constȱ�ٱ�ʶconst" << endl;
                    return 0;
            }
            p++;
    */
    // �������
    int flag = 1;
    if (ca[p] != "id") {
        cout << sh[p] << "------const��id����" << endl;
        flag = 0;
    }
    if (check()) {
        cout << sh[p] << "------ͬ��γ����ظ�����" << endl;
        flag = 0;
    }
    nametab[name_key].name = st[p]; //***20:17
    // cout << name_key << "---" << nametab[name_key].name << endl;
    nametab[name_key].information = "const";
    nametab[name_key].link = btable[display[level]][1]; //***20:26�˴�ӦΪ0
    btable[btab_key][1] = name_key;
    // link_val = name_key;

    p++;
    if (ca[p] != ":=") {
        cout << sh[p] << "------const��:=����" << endl;
        flag = 0;
    }
    p++;
    if (ca[p] != "integer") {
        cout << sh[p] << "------const��integer����" << endl;
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
        cout << sh[p] << "------var����" << endl;
        flag = 0;
    }
    p++;
    if (ca[p] != "id") {
        cout << sh[p] << "------id����" << endl;
        flag = 0;
    }
    if (check()) {
        cout << sh[p] << "------ͬ��α����ظ�����" << endl;
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
            cout << sh[p] << "------varcedl��,�����" << endl;
            flag = 0;
        }
        if (check()) {
            cout << sh[p] << "------ͬ��α����ظ�����" << endl;
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
        cout << sh[p] << "��-----varcedlȱ��;" << endl;
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
        cout << sh[p] << "��------constȱ��;" << endl;
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
    btable[btab_key][1] = name_key; // program�ı�0-0
    // 1/29	��0��p��
    name_key++;
    display[level] = btab_key; // �˴�ӦΪ0-0
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
        cout << sh[p] << "------proc��procedure����" << endl;
        flag = 0;
    }

    p++;
    if (ca[p] != "id") {
        cout << sh[p] << "��------proc��ȱ�ٺ���������" << endl;
        flag = 0;
    } else if (check()) {
        cout << sh[p] << "��---procedure-�����ظ�����" << endl;
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
        cout << sh[p] << "------ proc��(����";
        flag = 0;
    }
    btab_key++; // ��������
    level++; // �������µ�proc
    display[level] = btab_key; // ѹ���µ�display
    btable[btab_key][0] = 0;
    btable[btab_key][1] = 0;
    p++;
    if (ca[p] == "id") {
        if (check()) {
            cout << sh[p] << "�к���" << st[p] << "�β��ظ�����" << endl;
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
                cout << sh[p] << "------proc��idȱ��" << endl;
                flag = 0;
            }
            if (check()) {
                cout << sh[p] << "�к���" << st[p] << "�β��ظ�����" << endl;
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
        cout << sh[p] << "------proc��)����" << endl;
        flag = 0;
    }
    nametab[temp2].call_need = call_need;
    p++;
    if (ca[p] != ";") {
        cout << sh[p] << "------proc��;����" << endl;
        flag = 0;
    }
    p++;
    if (!isblock()) {
        flag = 0;
    }
    // �����
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
    level--; // �˳���ǰproc��display��ջ
    if (ca[p] == ";") { // ��while?
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
            gen("JPC", 0, -1); // ָ��else��ʼ��
            if (ca[p] == "then") {
                p++;
                if (isstatement()) {
                    code_p2 = CODE.size();
                    if (ca[p] == "else") {
                        gen("JMP", 0, -1); // ָ��else����
                        get<2>(CODE[code_p1]) = code_p2 + 1;
                        p++;
                        if (!isstatement()) {
                            cout << sh[p] << "---else-statement����" << endl;
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
        cout << sh[p] << "------if-lexp����" << endl;
        return 0;
    }
    if (ca[p] == "while") {
        int code_p1, code_p2;
        code_p2 = CODE.size(); // ѭ�����
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
                cout << sh[p] << "------statement-while-statement����" << endl;
                return 0;
            }
            cout << sh[p] << "------while-do����" << endl;
            return 0;
        }
        wcout << sh[p] << "------while-exp����" << endl;
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
                            cout << sh[p] << "------callȱ��exp" << endl;
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
                        cout << sh[p] << "��------" << proc1 << "ʹ��ʱδ������" << endl;
                    } else if (res_number != call_number) {
                        flag = 0;
                        cout << sh[p] << "�βθ�����ƥ��,Ԥ������:" << res_number << "  ʵ������:" << call_number << endl;
                    }
                    p++;
                    return flag;
                } else {
                    cout << sh[p] << "------callȱ��)" << endl;
                    return 0;
                }
            } else {
                cout << sh[p] << "------callȱ��(" << endl;
                return 0;
            }
        } else {
            cout << sh[p] << "------callid����" << endl;
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
                     << "---readid����" << endl;
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
                    cout << sh[p] << "---" << ca[p] << "---readȱ��id" << endl;
                    return 0;
                }
            }
            if (ca[p] == ")") {
                p++;
                return flag;
            } else {
                cout << sh[p] << "------readȱ��)" << endl;
                return 0;
            }
        }
        cout << sh[p] << "------readȱ��(" << endl;
        return 0;
    }
    if (ca[p] == "write") { // �������Ļ
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
                        cout << sh[p] << "---write��������" << endl;
                        return 0;
                    }
                }
                if (ca[p] == ")") {
                    p++;
                    return 1;
                } else {
                    cout << sh[p] << "��------write)ȱ��" << endl;
                    return 0;
                }
            }
            return 0;
        } else {
            cout << sh[p] << "---" << ca[p] << "---statement-write(Error" << endl;
            return 0;
        }
    } else {
        cout << sh[p] << "��---" << ca[p] << "---statement�޷�ʶ��Ŀ�ͷ����" << endl;
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
                cout << sh[p] << "��"
                     << "�˳�û���Ұ�ֵ" << endl;
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
        flag = var_loc(st[p], 0); // ���ɷ�������ջ����
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
                cout << "���ʽȱ��)" << endl;
                return 0;
            }
        } else {
            cout << "ȱ�ٱ��ʽ" << endl;
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
    cout << "�ʷ��������" << endl;
    cout << "----------------------------------------" << endl;
    for (int i = 0; i < length; i++) {
        cout << sh[i] << "---" << i << "---" << st[i] << "---" << ca[i] << endl;
    }
    cout << "----------------------------------------" << endl;
    int result = 1;
    result = isprog();
    cout << "----------------------------------------" << endl;
    if (p != length && result == 1)
        cout << "�ж�����ַ�����" << endl;
    else if (result == 1) {
        cout << "�﷨������ȷ" << endl;
        cout << "----------------------------------------" << endl;
        cout << "�м������������" << endl;
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
        cout << "������޸�" << endl;
    return result;
}
