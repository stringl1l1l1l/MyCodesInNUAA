#include "PL0.h"
#include "Interpreter.h"
#include "PCode.h"
#include "SymTable.h"
#include <cstddef>
#include <fcntl.h>
#include <fstream>
#include <io.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
#include <windows.h>
#include <winuser.h>

wchar_t w_ch = 32; // 用于词法分析器，存放最近一次从文件中读出的字符
unsigned long sym; // 最近一次识别出来的 token 的类型
wstring strToken; // 最近一次识别出来的标识符的名字
size_t strToken_len = 0; // 当前token长度
size_t col_pos = 0; // 列指针
size_t row_pos = 1; // 行指针
size_t pre_word_col = 0; // 上一个非空白合法词尾列指针
size_t pre_word_row = 1; // 上一个非空白合法词行指针
size_t err_cnt = 0; /* 出错总次数 */
size_t level = 0; //  层差
size_t glo_offset = 0;

wstring progm_w_str; // 源程序代码的wchar字符串形式
size_t progm_lenth;
size_t ch_ptr;
unordered_map<unsigned long, wstring> sym_map; // 保留字编号与字符串的映射
wstring err_msg[ERR_CNT]; // 错误信息表
ifstream file;

// first集
unsigned long first_block = CONST_SYM | VAR_SYM | PROC_SYM | BEGIN_SYM;
unsigned long first_stmt = IDENT | IF_SYM | WHILE_SYM | CALL_SYM | BEGIN_SYM | READ_SYM | WRITE_SYM;
unsigned long first_factor = IDENT | NUMBER | LPAREN;
unsigned long first_term = first_factor;
unsigned long first_exp = first_term | PLUS | MINUS;
unsigned long first_lexp = first_exp | ODD_SYM;
unsigned long first_lop = EQL | NEQ | LSS | LEQ | GRT | GEQ;

// follow集
unsigned long follow_condecl = VAR_SYM | PROC_SYM | BEGIN_SYM;
unsigned long follow_vardecl = PROC_SYM | BEGIN_SYM;
unsigned long follow_body = SEMICOLON | END_SYM | ELSE_SYM;
unsigned long follow_stmt = END_SYM | SEMICOLON | ELSE_SYM;
unsigned long follow_lexp = THEN_SYM | DO_SYM;
unsigned long follow_exp = follow_stmt | COMMA | RPAREN | first_lop | follow_lexp;
unsigned long follow_term = follow_exp | PLUS | MINUS;
unsigned long follow_factor = MULTI | DIVIS | follow_term;
unsigned long follow_ident = COMMA | SEMICOLON | LPAREN | RPAREN | follow_factor;
unsigned long follow_block = NUL | SEMICOLON;
unsigned long follow_constdef = COMMA | SEMICOLON;
unsigned long follow_proc = BEGIN_SYM;
// 保留字表
wstring resv_table[RSV_WORD_MAX] = {
    L"odd", L"begin", L"end", L"if", L"then", L"while", L"do", L"call",
    L"const", L"var", L"procedure", L"write", L"read", L"program", L"else"
};

// 运算符号表
wchar_t opr_table[OPR_MAX] = { L'+', L'-', L'*', L'/', L'=', L'<',
    L'>', L'(', L')', L',', L';' };
void init()
{
    w_ch = L' '; // 用于词法分析器，存放最近一次从文件中读出的字符
    sym = NUL; // 最近一次识别出来的 token 的类型
    strToken = L""; // 最近一次识别出来的标识符的名字
    strToken_len = 0; // 当前token长度
    col_pos = 0; // 列指针
    row_pos = 1; // 行指针
    pre_word_col = 0; // 上一个非空白合法词尾列指针
    pre_word_row = 1; // 上一个非空白合法词行指针
    err_cnt = 0; /* 出错总次数 */
    level = 0; //  层差
    progm_w_str = L""; // 源程序代码的wchar字符串形式
    progm_lenth = 0;
    glo_offset = 0;
    // 清空原来的文件字符串
    progm_w_str.clear();
    // 重置字符指针
    ch_ptr = 0;
    // 清空符号表
    SymTable::clear();
    // 清空中间代码单元
    PCodeList::clear();
    // 清空运行单元
    Interpreter::clear();
    // 以Unicode方式打开输入输出流
    _setmode(_fileno(stdout), _O_U16TEXT);
    // 符号名表初始化
    sym_map[NUL] = L"NUL";
    sym_map[IDENT] = L"IDENT";
    sym_map[NUMBER] = L"NUMBER";
    sym_map[PLUS] = L"PLUS";
    sym_map[MINUS] = L"MINUS";
    sym_map[MULTI] = L"MULTI";
    sym_map[DIVIS] = L"DIVIS";
    sym_map[ODD_SYM] = L"ODD_SYM";
    sym_map[EQL] = L"EQL";
    sym_map[NEQ] = L"NEQ";
    sym_map[LSS] = L"LSS";
    sym_map[LEQ] = L"LEQ";
    sym_map[GRT] = L"GRT";
    sym_map[GEQ] = L"GEQ";
    sym_map[LPAREN] = L"LPAREN";
    sym_map[RPAREN] = L"RPAREN";
    sym_map[COMMA] = L"COMMA";
    sym_map[SEMICOLON] = L"SEMICOLON";
    sym_map[ASSIGN] = L"BECOMES";
    sym_map[BEGIN_SYM] = L"BEGIN_SYM";
    sym_map[END_SYM] = L"END_SYM";
    sym_map[IF_SYM] = L"IF_SYM";
    sym_map[THEN_SYM] = L"THEN_SYM";
    sym_map[WHILE_SYM] = L"WHILE_SYM";
    sym_map[DO_SYM] = L"DO_SYM";
    sym_map[CALL_SYM] = L"CALL_SYM";
    sym_map[CONST_SYM] = L"CONST_SYM";
    sym_map[VAR_SYM] = L"VAR_SYM";
    sym_map[PROC_SYM] = L"PROC_SYM";
    sym_map[WRITE_SYM] = L"WRITE_SYM";
    sym_map[READ_SYM] = L"READ_SYM";
    sym_map[PROGM_SYM] = L"PROGM_SYM";
    sym_map[ELSE_SYM] = L"ELSE_SYM";
    // 报错信息初始化
    // missing错误
    err_msg[MISSING] = L"Missing %s";
    // undeclare错误
    err_msg[UNDECLARED_IDENT] = L"Undeclared identifier '%s'";
    err_msg[UNDECLARED_PROC] = L"Undeclared procedure name '%s'";
    // redefined错误
    err_msg[REDECLEARED_IDENT] = L"Redecleared identifier '%s'";
    err_msg[REDECLEARED_PROC] = L"Redecleared procedure name '%s'";
    // illegal错误
    err_msg[ILLEGAL_DEFINE] = L"Illegal %s definition ";
    err_msg[ILLEGAL_WORD] = L"Illegal word %s";
    err_msg[ILLEGAL_RVALUE_ASSIGN] = L"Cannot assign a rvalue";
    // expect错误
    err_msg[EXPECT] = L"Expecting %s";
    err_msg[EXPECT_STH_FIND_ANTH] = L"Expecting %s but %s was found";
    // redundant错误
    err_msg[REDUNDENT] = L"Redundent %s";
    // 其他错误
    err_msg[INCOMPATIBLE_VAR_LIST] = L"The real variable list is incompatible with formal variable list";
    err_msg[UNDEFINED_PROC] = L"Calling undefined procedure '%s'";
}

/*UTF8 编码格式（xxx 是用来填充二进制 Unicode 码点的）
1字节	0xxxxxxx
2字节	110xxxxx 10xxxxxx
3字节	1110xxxx 10xxxxxx 10xxxxxx
4字节	11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
有效的 Unicode 码点范围:
0000 0000 ~ 0000 007F | 0xxxxxxx
0000 0080 ~ 0000 07FF | 110xxxxx 10xxxxxx
0000 0800 ~ 0000 FFFF | 1110xxxx 10xxxxxx 10xxxxxx
0001 0000 ~ 0010 FFFF | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
*/
// 读取 UTF8 文件, 返回Unicode（UCS-2）字符串
void readFile2USC2(string filename)
{
    // 打开文件
    file.open(filename);
    if (!file.is_open()) {
        wcout << L"cannot open file!" << endl;
        return;
    }
    wcout << L"\e[32mCompiling file '" << filename.c_str() << L"'!\e[0m" << endl;

    // 禁止过滤空白符
    file >> noskipws;
    // 跳过 UTF8 BOM（0xEFBBBF）
    if (file.get() != 0xEF || file.get() != 0xBB || file.get() != 0xBF) {
        file.seekg(0, ios::beg);
    }

    byte B; // 1字节
    wchar_t wchar; // 2字节存储UCS-2码点
    wstring w_str(L""); // 用于存储转换结果的 Unicode 码点序列
    int len; // 单个 UTF8 字符的编码长度

    while ((file >> B) && !file.eof()) {
        // 单字节编码 0xxxxxxx
        if (B < 0b10000000) {
            wchar = B;
        } // 多字节编码，获取编码长度
        else {
            // 超出可用 Unicode 范围
            if (B > 0b11110100) {
                wcout << L"Invalid unicode range" << endl;
                return;
            } // 4字节编码 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
            else if (B >= 0b11110000) {
                len = 4;
            }
            // 1110xxxx 10xxxxxx 10xxxxxx
            else if (B >= 0b11100000) {
                len = 3;
            }
            // 110xxxxx 10xxxxxx
            else if (B >= 0b11000000) {
                len = 2;
            } else {
                // 除单字节外，首字节不能小于 0b11000000
                wcout << L"Invalid utf8 leading code" << endl;
                return;
            }
            // 通过左移再右移的方法去掉首字节中的 UTF8 标记
            B = B << (len + 1);
            wchar = B >> (len + 1);
            // 处理后续字节
            while (len > 1) {
                B = file.get();
                // 如果 f 到达 eof，则 c 会返回 255
                // 后续编码必须是 0b10xxxxxx 格式
                if (B >= 0b11000000) {
                    wcout << L"Invalid utf8 tailing code" << endl;
                    return;
                }
                len--;
                B = B & 0b00111111; // 去掉 UTF8 标记
                wchar = wchar << 6; // 腾出 6 个 bit 的位置
                wchar += B; // 将去掉了 UTF8 标记的编码合并进来
            }
        }
        // 存储解解析结果
        w_str.push_back(wchar);
    }
    w_str.push_back(L'#');
    progm_w_str = w_str;
    file.clear();
    // file.seekg(0, file.beg);
    file.close();
}

// // 读取待编译源代码文件
// void readFile2Str()
// {
//     fstream file(PRGM_PATH, ios::in);
//     stringstream stream;
//     if (!file.is_open()) {
//         cout << "cannot open file!" << endl;
//         exit(0);
//         cout << "cannot open file!" << endl;
//     }
//     stream << file.rdbuf() << '#';
//     progm_str = stream.str();
//     progm_lenth = progm_str.length();
//     progm_c_str = new wchar_t[progm_lenth];
//     strcpy(progm_c_str, progm_str.c_str());
// }

// 打印错误信息
void printPreWord(const wchar_t msg[])
{
    wcout << L"\e[31m(" << pre_word_row << "," << pre_word_col << L")"
          << L" Error: " << msg << L"\e[0m " << endl;
}

void printCurWord(const wchar_t msg[])
{
    wcout << L"\e[31m(" << row_pos << "," << col_pos << L")"
          << L" Error: " << msg << L"\e[0m " << endl;
}

template <class... T>
void error(size_t n, T... extra)
{
    wchar_t msg[200] = L"";
    wsprintf(msg, err_msg[n].c_str(), extra...);
    err_cnt++;
    if (n == REDUNDENT || n == MISSING)
        printPreWord(msg);
    else
        printCurWord(msg);
}

// 格式化输出分析结果
void over()
{
    if (err_cnt == 0) {
        wcout << L"\e[32mNo error. Congratulations!\e[0m" << endl;
        wcout << L"\e[32m______________________________Compile compelete!________________________________\e[0m\n"
              << endl;
    } else {
        wcout << L"\e[31mTotol: " << err_cnt << L" errors\e[0m" << endl;
        wcout << L"\e[33m_______________________________Compile failed!_________________________________\e[0m\n"
              << endl;
    }
}

// 判断是否为数字
bool isDigit(wchar_t ch)
{
    if (ch >= L'0' && ch <= L'9')
        return true;
    else
        return false;
}

// 判断是否为字母
bool isLetter(wchar_t ch)
{
    if ((ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z'))
        return true;
    else
        return false;
}

// 判断是否为终止符
bool isTerminate(wchar_t ch)
{
    return ch == L' ' || ch == L'\t' || ch == L'\n' || ch == L'#' || ch == L'\0' || ch == L';' || ch == L',';
}

// 判断是否为运算符，是则返回在表中位置，否则返回-1
int isOprator(wchar_t ch)
{
    wchar_t* p = opr_table;
    while (p - opr_table != OPR_MAX) {
        if (*p == ch) {
            return p - opr_table;
        }
        p++;
    }
    return -1;
}

// 跳过所有空白符，并将读指针置于空白符下一个位置，但并不装载下一个字符
void skipBlank()
{
    while (progm_w_str[ch_ptr] && (progm_w_str[ch_ptr] == L' ' || progm_w_str[ch_ptr] == L'\t')) {
        col_pos++;
        ch_ptr++;
    }
}

// 获取ch_ptr对应位置的字符，并移动读指针到下一位置。若当前字符为回车符，则跳过当前字符
void getCh()
{
    w_ch = progm_w_str[ch_ptr++];
    col_pos++;
}

// 退一个字符
void retract()
{
    w_ch = progm_w_str[--ch_ptr];
    col_pos--;
}

// 将当前字符追加到token中
void contract()
{
    strToken += w_ch;
    strToken_len++;
}

// 查询str是否为保留字，是保留字返回保留字在表中位置，否则返回-1
int reserve(wstring str)
{
    for (int i = 0; i < RSV_WORD_MAX; i++) {
        if (resv_table[i] == str) {
            return i;
        }
    }
    return -1;
}

// 词法分析器
void getWord()
{
    if (w_ch != L'\n') {
        pre_word_col = col_pos;
        pre_word_row = row_pos;
    }
    strToken.clear();
    strToken_len = 0;
    skipBlank();
    getCh();
    if (w_ch == L'\0')
        return;
    // 跳过连续的回车符
    if (w_ch == L'\n') {
        col_pos = 0;
        row_pos++;
        getWord();
        return;
    } else if (w_ch == L'#') {
        contract();
        sym = NUL;
    }
    // 纯字母
    else if (isLetter(w_ch)) {
        while (isLetter(w_ch) || isDigit(w_ch)) {
            contract();
            getCh();
        }
        // 查表，判断是否为保留字
        int code = reserve(strToken);
        switch (code) {
        case -1:
            sym = IDENT;
            break;
        case 0:
            sym = ODD_SYM;
            break;
        case 1:
            sym = BEGIN_SYM;
            break;
        case 2:
            sym = END_SYM;
            break;
        case 3:
            sym = IF_SYM;
            break;
        case 4:
            sym = THEN_SYM;
            break;
        case 5:
            sym = WHILE_SYM;
            break;
        case 6:
            sym = DO_SYM;
            break;
        case 7:
            sym = CALL_SYM;
            break;
        case 8:
            sym = CONST_SYM;
            break;
        case 9:
            sym = VAR_SYM;
            break;
        case 10:
            sym = PROC_SYM;
            break;
        case 11:
            sym = WRITE_SYM;
            break;
        case 12:
            sym = READ_SYM;
            break;
        case 13:
            sym = PROGM_SYM;
            break;
        case 14:
            sym = ELSE_SYM;
            break;
        default:
            sym = NUL;
            break;
        }
        retract();
    }
    // 开头为数字，判断是否为数值类型
    else if (isDigit(w_ch)) {
        while (isDigit(w_ch)) {
            contract();
            getCh();
        }
        // 遇到字母
        if (isLetter(w_ch)) {
            error(ILLEGAL_WORD, (L"'" + strToken + L"'").c_str());
            // 跳过错误至下一个终止符
            while (!isTerminate(w_ch))
                getCh();
            retract();
            strToken.clear();
            sym = NUL;
        }
        // 遇到其他字符
        else {
            sym = NUMBER;
            retract();
        }
    } // 遇到:判断是否为赋值符号
    else if (w_ch == L':') {
        contract();
        getCh();
        if (w_ch == L'=') {
            contract();
            pre_word_col++;
            sym = ASSIGN;
        } else {
            error(MISSING, L"'='");
            strToken.clear();
            sym = NUL;
        }
    } else if (w_ch == L'<') {
        contract();
        getCh();
        if (w_ch == L'=') {
            contract();
            pre_word_col++;
            sym = LEQ;
        } else if (w_ch == L'>') {
            contract();
            pre_word_col++;
            sym = NEQ;
        } else {
            sym = LSS;
            retract();
        }
    } else if (w_ch == L'>') {
        contract();
        getCh();
        if (w_ch == L'=') {
            contract();
            pre_word_col++;
            sym = GEQ;
        } else {
            sym = GRT;
            retract();
        }
    } else {
        int code = isOprator(w_ch);
        if (code != -1) {
            contract();
            switch (code) {
            case 0:
                sym = PLUS;
                break;
            case 1:
                sym = MINUS;
                break;
            case 2:
                sym = MULTI;
                break;
            case 3:
                sym = DIVIS;
                break;
            case 4:
                sym = EQL;
                break;
            // 这里注释掉是因为前面判断是不是复合符号的时候已经判断过了，
            // 能进入到这段逻辑肯定不是'<'和'>'了
            // case 5:
            //     sym = LSS;
            //     break;
            // case 6:
            //     sym = GRT;
            //     break;
            case 7:
                sym = LPAREN;
                break;
            case 8:
                sym = RPAREN;
                break;
            case 9:
                sym = COMMA;
                break;
            case 10:
                sym = SEMICOLON;
                break;
            default:
                break;
            }
        } else {
            contract();
            error(ILLEGAL_WORD, (L"'" + strToken + L"'").c_str());
            sym = NUL;
        }
    }
    // wcout << L"(" << row_pos << L"," << col_pos << L")\t" << setw(15)
    //       << strToken << setw(20) << sym_map[sym] << endl;
}

// 用于错误恢复的函数，若当前符号在s1中，则读取下一符号；若当前符号不在s1中，则报错，接着循环查找下一个在中s1 ∪ s2的符号
template <class... T>
void judge(unsigned long s1, unsigned long s2, size_t n, T... extra)
{
    if (!(sym & s1)) // 当前符号不在s1中
    {
        error(n, extra...);
        unsigned long s3 = s1 | s2; // 把s2补充进s1

        while (!(sym & s3)) // 循环找到下一个合法的符号
        {
            if (w_ch == L'\0')
                over();
            getWord(); // 继续词法分析
        }
        if (sym & s1)
            getWord();
    } else
        getWord();
}
// 开始语法分析
// const -> id:=number
void constDef()
{
    if (sym == IDENT) {
        // 将常量登入符号表，常量属于右值，不需要记录offset
        wstring name = strToken;
        SymTable::enter(name, 0, Category::CST);
        getWord();
        // const -> id:=
        if (sym & (ASSIGN | EQL)) {
            if (sym == EQL) {
                error(EXPECT_STH_FIND_ANTH, L":=", L"=");
            }
            getWord();
            // const -> id:=number
            if (sym == NUMBER) {
                SymTable::table[SymTable::table.size() - 1].info->setValue(strToken);
                getWord();
            } else {
                error(EXPECT_NUMEBR_AFTER_BECOMES);
            }
        }
    } else {
        // 没有找到id则跳转到第一个constDef的follow集中的符号
        judge(0, follow_constdef, ILLEGAL_DEFINE, L"constDef");
    }
}

// <condecl> -> const <constDef>{,<constDef>};
void condecl()
{
    // const
    if (sym == CONST_SYM) {
        // const <constDef>
        getWord();
        constDef();
        // const <constDef> {,<constDef>}
        while (sym & (COMMA | IDENT)) {
            judge(COMMA, IDENT, MISSING, L"','");
            if (sym == IDENT) // FIRST(<constDef>)
            {
                constDef();
            } else {
                judge(0, IDENT | SEMICOLON, EXPECT, L"constDef"); // todo expect常量定义式
            }
        }
        // const <constDef> {,<constDef>};
        if (sym == SEMICOLON) {
            getWord();
            return;
        } else {
            judge(0, follow_condecl, MISSING, L"';'"); // todo expect ;
        }
    } else {
        judge(0, follow_condecl, ILLEGAL_DEFINE, L"condecl");
    }
}

// <vardecl> -> var <id>{,<id>};
void vardecl()
{
    // var
    if (sym == VAR_SYM) {
        getWord();
        // var <id>
        if (sym == IDENT) {
            // 将标识符登入到符号表
            SymTable::enter(strToken, glo_offset, Category::VAR);
            glo_offset += VAR_WIDTH;
            getWord();
        } else {
            judge(0, COMMA, MISSING, L"identifier");
        }
        // var <id>{,<id>}
        while (sym == COMMA) {
            getWord();
            if (sym == IDENT) {
                // 将标识符登入到符号表
                SymTable::enter(strToken, glo_offset, Category::VAR);
                glo_offset += VAR_WIDTH;
                getWord();
            } else {
                error(REDUNDENT, L"','"); // todo expect 标识符
            }
        }
        // var <id>{,<id>};
        if (sym == SEMICOLON) {
            getWord();
        } else {
            judge(0, follow_vardecl, MISSING, L"';'");
        }
    } else {
        judge(0, follow_vardecl, ILLEGAL_DEFINE, L"vardecl");
    }
}

// <proc> -> procedure id ([id {,id}]);<block> {;<proc>}
void proc()
{
    if (sym == PROC_SYM) {
        getWord();
        // <proc> -> procedure id
        ProcInfo* cur_info = nullptr; // 临时变量，记录当前过程符号表项的信息
        if (sym == IDENT) {
            // 将过程名登入符号表
            SymTable::mkTable();
            int cur_proc = SymTable::enterProc(strToken);
            if (cur_proc != -1) {
                cur_info = (ProcInfo*)SymTable::table[cur_proc].info;
                // 子过程的入口地址登入符号表，待回填
                size_t entry = PCodeList::emit(jmp, 0, 0);
                SymTable::table[SymTable::table.size() - 1].info->setEntry(entry);
            }
            getWord();
        } else {
            judge(0, LPAREN, EXPECT_STH_FIND_ANTH, L"identifier", (L"'" + strToken + L"'").c_str());
        }
        // <proc> -> procedure id (
        if (sym == LPAREN) {
            // 层级增加，display表扩张
            SymTable::display.push_back(0);
            level++;
            getWord();
        } else {
            judge(0, IDENT | RPAREN, MISSING, L"'('");
        }
        // <proc> -> procedure id ([id {,id}]
        // 分析至形参列表
        if (sym == IDENT) {
            // 将过程的形参登入符号表，并与相应的过程绑定
            int form_var = SymTable::enter(strToken, glo_offset, Category::FORM);
            glo_offset += VAR_WIDTH;
            if (cur_info)
                cur_info->form_var_list.push_back(form_var);
            getWord();
            while (sym == COMMA) {
                getWord();
                if (sym == IDENT) {
                    // 将过程的形参登入符号表，并与相应的过程绑定
                    int form_var = SymTable::enter(strToken, glo_offset, Category::FORM);
                    glo_offset += VAR_WIDTH;
                    if (cur_info)
                        cur_info->form_var_list.push_back(form_var);
                    getWord();
                } else {
                    error(REDUNDENT, L"','");
                }
            }
        }
        // <proc> -> procedure id ([id {,id}])
        if (sym == RPAREN) {
            getWord();
        } else {
            judge(0, SEMICOLON, MISSING, L"')'");
        }
        // <proc> -> procedure id ([id {,id}]);
        if (sym == SEMICOLON) {
            getWord();
        } else {
            judge(0, first_block, MISSING, L"';'");
        }
        // <proc> -> procedure id ([id {,id}]);<block> {;<proc>}
        if (sym & first_block) {
            block();
            // 执行返回，并弹栈
            PCodeList::emit(opr, 0, OPR_RETURN);
            // 层级减少，display表弹出
            SymTable::display.pop_back();
            level--;
            // 当前过程结束，开始分析下面的过程
            while (sym == SEMICOLON) {
                getWord();
                // FIRST(proc)
                if (sym == PROC_SYM) {
                    proc();
                } else {
                    error(REDUNDENT, L"';'");
                }
            }
        } else {
            judge(0, follow_block, ILLEGAL_DEFINE, L"block");
        }
    } else {
        judge(0, follow_proc, ILLEGAL_DEFINE, L"procedure");
    }
}

// <exp> -> [+|-] <term>{[+|-] <term>}
void exp()
{
    // <exp> -> [+|-]
    unsigned long aop = NUL;
    if (sym == PLUS || sym == MINUS) {
        aop = sym;
        getWord();
    }
    // <exp> -> <term>{[+|-] <term>}
    if (sym & first_term) // FIRST(term) 、 FIRST(factor)
    {
        term();
        // 若有负号，栈顶取反
        if (aop == MINUS)
            PCodeList::emit(opr, 0, OPR_NEGTIVE);
        while (sym == PLUS || sym == MINUS) {
            aop = sym;
            getWord();
            // FIRST(term)
            if (sym & first_term) {
                term();
                // 减
                if (aop == MINUS)
                    PCodeList::emit(opr, 0, OPR_SUB);
                // 加
                else
                    PCodeList::emit(opr, 0, OPR_ADD);
            } else {
                error(REDUNDENT, strToken.c_str());
            }
        }
    } else {
        judge(0, follow_exp, ILLEGAL_DEFINE, L"expression");
    }
}

// <factor> -> id|number|(<exp>)
void factor()
{
    if (sym == IDENT) {
        // 查找变量符号
        int pos = SymTable::lookUp(strToken);
        VarInfo* cur_info = nullptr;
        if (pos == -1)
            error(UNDECLARED_IDENT, strToken.c_str());
        // 若为常量，直接获取其符号表中的右值
        else if (SymTable::table[pos].info->cat != Category::PROCE)
            // 用临时变量记录当前查到的信息
            cur_info = (VarInfo*)SymTable::table[pos].info;
        // 查找到重名的过程符号
        else {
            // todo
        }
        if (cur_info) {
            if (cur_info->cat == Category::CST) {
                int val = cur_info->getValue();
                PCodeList::emit(lit, cur_info->level, val);
            }
            // 若为变量，取左值
            else {
                PCodeList::emit(load, cur_info->level, cur_info->offset / UNIT_SIZE + ACT_PRE_REC_SIZE + cur_info->level + 1);
            }
        }
        getWord();
    } else if (sym == NUMBER) {
        // 数值，直接入栈
        PCodeList::emit(lit, 0, w_str2int(strToken));
        getWord();
    } else if (sym == LPAREN) {
        getWord();
        exp();
        if (sym == RPAREN) {
            getWord();
        } else {
            judge(0, follow_factor, MISSING, L"')'");
        }
    } else {
        judge(0, follow_factor, ILLEGAL_DEFINE, L"factor");
    }
}

// <term> -> <factor>{[*|/] <factor>}
void term()
{
    if (sym & first_term) {
        factor();
        // factor()执行完毕后，当前栈顶即为factor的值
        while (sym == MULTI || sym == DIVIS) {
            unsigned long nop = sym;
            getWord();
            if (sym & first_term) {
                factor();
                // 乘
                if (nop == MULTI)
                    PCodeList::emit(opr, 0, OPR_MULTI);
                // 除
                else
                    PCodeList::emit(opr, 0, OPR_DIVIS);
            } else {
                error(REDUNDENT, strToken.c_str());
            }
        }
    } else {
        judge(0, follow_term, ILLEGAL_DEFINE, L"term");
    }
}

// <lexp> -> <exp> <lop> <exp> | odd <exp>
void lexp()
{
    // <lexp> -> odd <exp>
    if (sym == ODD_SYM) {
        getWord();
        if (sym & first_exp) {
            exp();
            // odd
            PCodeList::emit(opr, 0, OPR_ODD);
        } else {
            error(EXPECT, L"expression");
        }
    }
    // <lexp> -> <exp> <lop> <exp>
    else if (sym & first_exp) {
        exp();
        if (sym & first_lop) {
            unsigned int lop = sym;
            getWord();
            exp();
            switch (lop) {
                // <
            case LSS:
                PCodeList::emit(opr, 0, OPR_LSS);
                break;
                // <=
            case LEQ:
                PCodeList::emit(opr, 0, OPR_LEQ);
                break;
                // >
            case GRT:
                PCodeList::emit(opr, 0, OPR_GRT);
                break;
                // >=
            case GEQ:
                PCodeList::emit(opr, 0, OPR_GEQ);
                break;
                // <>
            case NEQ:
                PCodeList::emit(opr, 0, OPR_NEQ);
                break;
                // =
            case EQL:
                PCodeList::emit(opr, 0, OPR_EQL);
                break;
            default:
                break;
            }
        } else {
            judge(0, first_exp, MISSING, L"'<' or '<=' or '>' or '>=' or '<>' or '='");
            exp();
        }
    } else {
        judge(0, follow_lexp, ILLEGAL_DEFINE, L"lexp");
    }
}

void statement()
{
    // <statement> -> id := <exp>
    if (sym == IDENT) {
        // 查找当前变量是否在符号表中
        int pos = SymTable::lookUp(strToken);
        VarInfo* cur_info = nullptr;
        // 未查找到符号
        if (pos == -1)
            error(UNDECLARED_IDENT, strToken.c_str());
        else if (SymTable::table[pos].info->cat != Category::PROCE) // 用临时变量记录当前查到的信息
            cur_info = (VarInfo*)SymTable::table[pos].info;
        // 查找到重名的过程符号
        else {
            // todo
        };
        getWord();
        if (sym == ASSIGN) {
            // 查找到右值，右值不可被赋值
            if (cur_info && cur_info->cat == Category::CST)
                error(ILLEGAL_RVALUE_ASSIGN);
            getWord();
        } // 不是赋值号：=而是等于号=
        else if (sym == EQL) {
            error(EXPECT_STH_FIND_ANTH, L"=", L":=");
            getWord();
        } else {
            // 跳过非法符号，直到遇到exp的follow集
            judge(0, follow_exp, MISSING, L"':='");
        }
        exp();
        if (cur_info)
            // 赋值的P代码，当前栈顶为计算出的表达式
            PCodeList::emit(store, cur_info->level, cur_info->offset / UNIT_SIZE + ACT_PRE_REC_SIZE + cur_info->level + 1);
    }
    // <statement> -> if <lexp> then <statement> [else <statement>]
    else if (sym == IF_SYM) {
        getWord();
        lexp();
        int entry_jpc = -1, entry_jmp = -1;

        // 当前栈顶为条件表达式的布尔值
        if (sym == THEN_SYM) {
            // 条件为假跳转，待回填else入口地址或if外地址
            entry_jpc = PCodeList::emit(jpc, 0, 0);
            getWord();
        } else {
            judge(0, first_stmt, MISSING, L"then");
        }
        // <statement> -> if <lexp> then <statement>
        statement();

        if (sym == ELSE_SYM) {
            // 待回填if外的入口地址
            entry_jmp = PCodeList::emit(jmp, 0, 0);
            getWord();
            // 将else入口地址回填至jpc
            PCodeList::backpatch(entry_jpc, PCodeList::code_list.size());
            statement();
            // 有else，则将if外入口地址回填至jmp
            PCodeList::backpatch(entry_jmp, PCodeList::code_list.size());
        } else
            // 没有else，则将if外入口地址回填至jpc
            PCodeList::backpatch(entry_jpc, PCodeList::code_list.size());
    }
    // <statement> -> while <lexp> do <statement>
    else if (sym == WHILE_SYM) {
        getWord();
        // FIRST(lexp)
        size_t condition = PCodeList::code_list.size();
        lexp();
        // 当前栈顶为条件表达式的布尔值
        // 条件为假跳转，待回填循环出口地址
        size_t loop = PCodeList::emit(jpc, 0, 0);
        if (sym == DO_SYM) {
            getWord();
            statement();
            // 无条件跳转至循环条件判断前
            PCodeList::emit(jmp, 0, condition);
        } else {
            judge(0, first_stmt, MISSING, L"do");
        }
        // 将下一条语句回填至jpc
        PCodeList::backpatch(loop, PCodeList::code_list.size());
    }
    // <statement> -> call id ([{<exp>{,<exp>}])
    else if (sym == CALL_SYM) {
        getWord();
        ProcInfo* cur_info = nullptr;
        // <statement> -> call id
        if (sym == IDENT) {
            // 查找过程的符号名
            int pos = SymTable::lookUp(strToken);
            // 未查找到过程名
            if (pos == -1)
                error(UNDECLARED_PROC, strToken.c_str());
            else if (SymTable::table[pos].info->cat == Category::PROCE)
                cur_info = (ProcInfo*)SymTable::table[pos].info;
            // 查找到重名的非过程符号
            else {
                // todo
            }
            // 若调用未定义的过程
            if (cur_info && !cur_info->isDefined)
                error(UNDEFINED_PROC, strToken.c_str());
            getWord();
        } else {
            judge(0, LPAREN, EXPECT_STH_FIND_ANTH, L"identifier", (L"'" + strToken + L"'").c_str());
        }
        // <statement> -> call id (
        if (sym == LPAREN) {
            getWord();
        } else {
            judge(0, first_exp | RPAREN, MISSING, L"'('");
        }
        // <statement> -> call id ([{<exp>
        if (sym & first_exp) {
            exp();
            // 将实参传入即将调用的子过程
            if (cur_info)
                PCodeList::emit(store, -1, ACT_PRE_REC_SIZE + cur_info->level + 1);
            size_t i = 1;
            // <statement> -> call id ([{<exp>{,<exp>}]
            while (sym == COMMA) {
                getWord();
                if (sym & first_exp) {
                    exp();
                    // 将实参传入即将调用的子过程
                    if (cur_info)
                        PCodeList::emit(store, -1, i + ACT_PRE_REC_SIZE + cur_info->level + 1);
                } else {
                    judge(0, first_exp, REDUNDENT, L"','");
                    exp();
                }
                i++;
            }
            // 若实参列表与形参列表变量数不兼容，报错
            if (cur_info && i != cur_info->form_var_list.size()) {
                error(INCOMPATIBLE_VAR_LIST);
            }
        }
        // <statement> -> call id ([{<exp>{,<exp>}])
        if (sym == RPAREN) {
            getWord();
            // 调用子过程
            if (cur_info)
                PCodeList::emit(call, cur_info->level, cur_info->entry);
        }
    }
    // <statement> -> <body>
    else if (sym == BEGIN_SYM) {
        body();
    }
    // <statement> -> read (id{,id})
    else if (sym == READ_SYM) {
        getWord();
        if (sym == LPAREN) {
            getWord();
        } else {
            judge(0, IDENT, MISSING, L"'('");
        }
        // <statement> -> read (id
        if (sym == IDENT) {
            int pos = SymTable::lookUp(strToken);
            // 未查找到符号
            VarInfo* cur_info = nullptr;
            if (pos == -1)
                error(UNDECLARED_IDENT, strToken.c_str());
            else if (SymTable::table[pos].info->cat != Category::PROCE)
                // 用临时变量记录当前查到的信息
                cur_info = (VarInfo*)SymTable::table[pos].info;
            // 查找到重名的过程符号
            else {
                // todo
            }
            // 右值不可被赋值
            if (cur_info) {
                if (cur_info->cat == Category::CST)
                    error(ILLEGAL_RVALUE_ASSIGN);
                // 从命令行读一个数据到栈顶
                PCodeList::emit(red, 0, 0);
                // 将栈顶值送入变量所在地址
                PCodeList::emit(store, cur_info->level, cur_info->offset / UNIT_SIZE + ACT_PRE_REC_SIZE + cur_info->level + 1);
            }
            getWord();
        } else {
            judge(0, COMMA | RPAREN, EXPECT_STH_FIND_ANTH, L"identifier", (L"'" + strToken + L"'").c_str());
        }
        while (sym == COMMA) {
            getWord();
            if (sym == IDENT) {
                int pos = SymTable::lookUp(strToken);
                // 未查找到符号
                VarInfo* cur_info = nullptr;
                if (pos == -1)
                    error(UNDECLARED_IDENT, strToken.c_str());
                else if (SymTable::table[pos].info->cat != Category::PROCE)
                    // 用临时变量记录当前查到的信息
                    cur_info = (VarInfo*)SymTable::table[pos].info;
                // 查找到重名的过程符号
                else {
                    // todo
                }
                // 右值不可被赋值
                if (cur_info && cur_info->cat == Category::CST) {
                    error(ILLEGAL_RVALUE_ASSIGN);
                    // 从命令行读一个数据到栈顶
                    PCodeList::emit(red, 0, 0);
                    // 将栈顶值送入变量所在地址
                    PCodeList::emit(store, cur_info->level, cur_info->offset / UNIT_SIZE + ACT_PRE_REC_SIZE + cur_info->level + 1);
                }
                getWord();
            } else {
                judge(0, IDENT, REDUNDENT, strToken.c_str());
            }
        }
        if (sym == RPAREN) {
            getWord();
        } else {
            judge(0, follow_stmt, MISSING, L"')'");
        }
    }
    // <statement> -> write(<exp> {,<exp>})
    else if (WRITE_SYM) {
        getWord();
        // <statement> -> write(
        if (sym == LPAREN) {
            getWord();
        } else {
            judge(0, first_exp, MISSING, L"'('");
        }
        // <statement> -> write(<exp>
        exp();
        PCodeList::emit(wrt, 0, 0);
        // <statement> -> write(<exp> {,<exp>}
        while (sym == COMMA) {
            getWord();
            if (sym == RPAREN)
                error(REDUNDENT, L"','");
            else
            {
                exp();
                PCodeList::emit(wrt, 0, 0);
            }
        }
        // <statement> -> write(<exp> {,<exp>})
        if (sym == RPAREN) {
            getWord();
        } else {
            judge(0, follow_stmt, MISSING, L"')'");
        }
    } else
        judge(0, follow_stmt, ILLEGAL_DEFINE, L"statement");
}

// <body> -> begin <statement> {;<statement>} end
void body()
{
    // 判断是否存在begin,是否仅缺少begin
    judge(BEGIN_SYM, first_stmt, MISSING, L"begin");
    // begin之后为新的作用域
    // level++;
    statement();
    // 这里如果end前多一个分号会多进行一次循环，并进入else分支
    while (sym & (SEMICOLON | COMMA | first_stmt)) {
        // 判断是否存在分号，是否仅缺少分号,是否错写为逗号
        if (sym == COMMA) {
            error(EXPECT_STH_FIND_ANTH, L";", L",");
            getWord();
        } else
            judge(SEMICOLON, first_stmt, MISSING, L"';'");
        if (sym & first_stmt) {
            statement();
        } else {
            error(REDUNDENT, L"';'");
        }
    }
    // 判断是否缺少end
    judge(END_SYM, 0, MISSING, L"end");
    // end之后作用域结束
    // level--;
}

// <block> -> [<condecl>][<vardecl>][<proc>]<body>
void block()
{
    // <block> -> [<condecl>]
    if (sym == CONST_SYM) {
        condecl();
    }
    // <block> -> [<condecl>][<vardecl>]
    if (sym == VAR_SYM) {
        vardecl();
    }
    // 将过程所需的内存大小写入符号表
    size_t cur_proc = SymTable::sp;
    ProcInfo* cur_info = (ProcInfo*)SymTable::table[cur_proc].info;
    SymTable::addWidth(
        cur_proc,
        glo_offset);
    // <block> -> [<condecl>][<vardecl>][<proc>]]
    if (sym == PROC_SYM) {
        proc();
    }
    // 为子过程开辟活动记录空间，其中为display开辟level + 1个单元
    size_t entry = PCodeList::emit(alloc, 0, cur_info->offset / UNIT_SIZE + ACT_PRE_REC_SIZE + level + 1);
    size_t target = cur_info->entry;
    // 将过程入口地址回填至过程的跳转语句
    PCodeList::backpatch(target, entry);
    // <block> -> [<condecl>][<vardecl>][<proc>]<body>
    // 过程体开始，过程已定义
    if (cur_proc)
        cur_info->isDefined = true;
    body();
}

// <prog> -> program id; <block>
void prog()
{
    // 找到第一次出现的program
    judge(PROGM_SYM, 0, MISSING, L"program");
    // <prog> -> program id
    if (sym == IDENT) {
        // 将过程名登入符号表
        SymTable::mkTable();
        SymTable::enterProgm(strToken);
        getWord();
    }
    // 缺失 <prog> -> program ;
    else if (sym == SEMICOLON) {
        error(MISSING, L"identifier");
    }
    //  <prog> -> program {~id} ;
    else {
        error(EXPECT_STH_FIND_ANTH, L"identifier", (L"'" + strToken + L"'").c_str());
        getWord();
    }
    // <prog> -> program id;
    if (sym == SEMICOLON) {
        getWord();
    }
    // <prog> -> program id {~';'} <block>
    else {
        // 判断是否仅仅是缺失分号
        judge(SEMICOLON, first_block, MISSING, L"';'");
    }
    // 主过程的入口地址登入符号表，待回填
    size_t entry = PCodeList::emit(jmp, 0, 0);
    SymTable::table[0].info->setEntry(entry);
    //<prog> -> program id; <block>
    block();
    //<prog> -> program id; <block>#
    // 执行返回，并弹栈
    // PCodeList::emit(opr, 0, 0);
    if (sym == NUL) {
        // 程序终止
        return;
    }
}

void analyze()
{
    getWord();
    prog();
    if (w_ch != L'#') {
        error(60);
    }
    over();
}