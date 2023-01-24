#include "PL0.h"
#include "SymbolTable.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <windows.h>
#include <tchar.h>

wchar_t w_ch = 32; // 用于词法分析器，存放最近一次从文件中读出的字符
unsigned long sym; // 最近一次识别出来的 token 的类型
wstring strToken; // 最近一次识别出来的标识符的名字
size_t strToken_len = 0; // 当前token长度
size_t num; // 词法分析器输出结果之用，存放最近一次识别出来的数字的值
size_t col_pos = 0; // 列指针
size_t row_pos = 1; // 行指针
size_t line_lenth; // 行缓冲区长度
size_t err = 0; /* 出错总次数 */
size_t offset = 0;
size_t level = 0; //  层差

string progm_str;
wstring progm_w_str; // 源程序代码的wchar字符串形式
size_t progm_lenth;
size_t ch_ptr;
unordered_map<unsigned long, wstring> sym_map; // 保留字编号与字符串的映射

// first集
unsigned long first_block = CONST_SYM | VAR_SYM | PROC_SYM | BEGIN_SYM;
unsigned long first_stmt = IDENT | IF_SYM | WHILE_SYM | CALL_SYM | BEGIN_SYM | READ_SYM | WRITE_SYM;
unsigned long first_factor = IDENT | NUMBER | LPAREN;
unsigned long first_term = first_factor;
unsigned long first_exp = first_term | PLUS | MINUS;
unsigned long first_lexp = first_exp | ODD_SYM;
unsigned long first_lop = EQL | NEQ | LSS | LEQ | GTR | GEQ;

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

// 错误信息表
wstring err_msg[ERR_CNT] = {
    L"Missing 'program'",
    L"Found '=' when expecting ':='.",
    L"There must be a number to follow '='.",
    L"There must be an '=' to follow the identifier.",
    L"There must be an identifier to follow 'const', 'var', or 'procedure'.",
    L"Missing ';'",
    L"Incorrect procedure name.",
    L"Statement expected.",
    L"Follow the statement is an incorrect symbol.",
    L"'.' expected.",
    L"';' expected.",
    L"Undeclared identifier.",
    L"Illegal assignment.",
    L"':=' expected.",
    L"There must be an identifier to follow the 'call'.",
    L"A constant or variable can not be called.",
    L"'then' expected.",
    L"';' or 'end' expected.",
    L"'do' expected.",
    L"Incorrect symbol.",
    L"Relative operators expected.",
    L"Procedure identifier can not be in an expression.",
    L"Missing ')'.",
    L"The symbol can not be followed by a factor.",
    L"The symbol can not be as the beginning of an expression.",
    L"Missing ",
    L"Redundent word",
    L"Expression expected",
    L"Missing identifier",
    L"Find number when identifier expected",
    L"Illegal ",
    L"The number is too great.",
    L"There are too many levels.",
    L"Illegal identifier",
    L"Missing '='",
    L"Illegal character",
    L"Missing 'begin'",
    L"Missing 'end'",
    L"Find ',' when expecting ';'",
    L"Missing ':='",
    L"Illegel word",
    L"Missing ','",
    L"Missing 'const'",
    L"Missing '('",
    L"Missing ')'",
    L"Missing ','",
    L"Illegal expression",
    L"Illegal factor",
    L"Illegal block",
    L"Redundent ';'",
    L"Illegal const definition",
    L"Illegal",
    L"Missing 'then'",
    L"Missing '<' or '<=' or '>' or '>=' or '<>' or '='",
    L"Missing 'do'",
    L"Illegal const declaration",
    L"Illegal procedure definition",
    L"Illegal term definition",
    L"Illegal lexp definition",
    L"Illegal statement definition",
    L"Redundent words when program should done"
};

void init()
{
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
    sym_map[GTR] = L"GTR";
    sym_map[GEQ] = L"GEQ";
    sym_map[LPAREN] = L"LPAREN";
    sym_map[RPAREN] = L"RPAREN";
    sym_map[COMMA] = L"COMMA";
    sym_map[SEMICOLON] = L"SEMICOLON";
    sym_map[BECOMES] = L"BECOMES";
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
    fstream file(filename, ios::in);
    if (!file.is_open()) {
        cout << "cannot open file!" << endl;
        exit(0);
    }
    // 跳过 UTF8 BOM（0xEFBBBF）
    if (file.get() != 0xEF || file.get() != 0xBB || file.get() != 0xBF) {
        file.seekg(0, ios::beg);
    }

    byte B; // 1字节
    wchar_t wchar; // 2字节存储UCS-2码点
    wstring w_str; // 用于存储转换结果的 Unicode 码点序列
    int len; // 单个 UTF8 字符的编码长度

    while ((B = file.get()) && !file.eof()) {
        // 单字节编码 0xxxxxxx
        if (B < 0b10000000) {
            wchar = B;
        } // 多字节编码，获取编码长度
        else {
            // 超出可用 Unicode 范围
            if (B > 0b11110100) {
                cout << B << endl;
                cout << "Invalid unicode range" << endl;
                exit(0);
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
                cout << "Invalid utf8 leading code" << endl;
                exit(0);
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
                    cout << "Invalid utf8 tailing code" << endl;
                    exit(0);
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
    progm_w_str = w_str;
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
void error(size_t n)
{
    err++;
    // wcout << "\e[31m(" << row_pos << "," << col_pos - strToken_len + 1 << ")"
    //       << " Error: "
    //       << err_msg[n]
    //       << "\e[0m " << endl;
}

// 格式化输出分析结果
void over()
{
    if (err == 0) {
        cout << "\e[32mNo error. Congratulations!\e[0m" << endl;
    } else {
        cout << "\e[31mTotol: " << err << " errors\e[0m" << endl;
    }
    exit(0);
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
    while (progm_w_str[ch_ptr] == L'0' && progm_w_str[ch_ptr] == L' ') {
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
            error(ILLEGAL_ID);
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
            sym = BECOMES;
        } else {
            error(MISSING_EQL);
            strToken.clear();
            sym = NUL;
        }
    } else if (w_ch == L'<') {
        contract();
        getCh();
        if (w_ch == L'=') {
            contract();
            sym = LEQ;
        } else if (w_ch == L'>') {
            contract();
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
            sym = GEQ;
        } else {
            sym = GTR;
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
            //     sym = GTR;
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
            error(ILLEGAL_CHAR);
            strToken.clear();
            sym = NUL;
        }
    }
    wcout << "(" << row_pos << "," << col_pos << ")\t" << setw(15) << strToken
          << setw(20) << sym_map[sym] << endl;
}

// 该函数是用来查follow集中有没有合适的符号
void judge(unsigned long s1, unsigned long s2, int n)
{
    if (!(sym & s1)) // 当前符号不在s1中
    {
        error(n);
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
        // 将常量登入符号表
        SymTable::enter(strToken, offset, Type::INTERGER);
        offset += CST_WIDTH;
        getWord();
        // const -> id:=
        if (sym & (BECOMES | EQL)) {
            if (sym == EQL) {
                error(EXPECT_BECOMES_NOT_EQL);
            }
            getWord();
            // const -> id:=number
            if (sym == NUMBER) {
                getWord();
            } else {
                error(EXPECT_NUMEBR_AFTER_BECOMES);
            }
        }
    } else {
        // 没有找到id则跳转到第一个constDef的follow集中的符号
        judge(0, follow_constdef, ILLEGAL_CONSTDEF);
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
            judge(COMMA, IDENT, MISSING_COMMA);
            if (sym == IDENT) // FIRST(<constDef>)
            {
                constDef();
            } else {
                judge(0, IDENT, REDUNDENT_WORD); // todo expect常量定义式
                constDef();
            }
        }
        // const <constDef> {,<constDef>};
        if (sym == SEMICOLON) {
            getWord();
            return;
        } else {
            judge(0, follow_condecl, MISSING_SEMICOLON); // todo expect ;
        }
    } else {
        judge(0, follow_condecl, ILLEGAL_CONDECL);
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
            SymTable::enter(strToken, VAR_WIDTH, Type::INTERGER);
            offset += VAR_WIDTH;
            getWord();
        } else {
            judge(0, COMMA, MISSING_IDENT);
        }
        // var <id>{,<id>}
        while (sym == COMMA) {
            getWord();
            if (sym == IDENT) {
                // 将标识符登入到符号表
                SymTable::enter(strToken, VAR_WIDTH, Type::INTERGER);
                offset += VAR_WIDTH;
                getWord();
            } else {
                error(REDUNDENT_COMMA); // todo expect 标识符
            }
        }
        // var <id>{,<id>};
        if (sym == SEMICOLON) {
            getWord();
        } else {
            judge(0, follow_vardecl, MISSING_SEMICOLON);
        }
    } else {
        judge(0, follow_vardecl, ILLEGAL_VARDECL);
    }
}

// <proc> -> procedure id ([id {,id}]);<block> {;<proc>}
void proc()
{
    level++;
    if (sym == PROC_SYM) {
        getWord();
        // <proc> -> procedure id
        if (sym == IDENT) {
            // 将过程名登入符号表
            SymTable::enterProc(strToken);
            getWord();
        } else {
            judge(0, LPAREN, MISSING_IDENT);
        }
        // <proc> -> procedure id (
        if (sym == LPAREN) {
            getWord();
        } else {
            judge(0, IDENT | RPAREN, MISSING_LPAREN);
        }
        // <proc> -> procedure id ([id {,id}]
        if (sym == IDENT) {
            getWord();
            while (sym == COMMA) {
                getWord();
                if (sym == IDENT) {
                    getWord();
                } else {
                    error(REDUNDENT_COMMA);
                }
            }
        }
        // <proc> -> procedure id ([id {,id}])
        if (sym == RPAREN) {
            getWord();
        } else {
            judge(0, SEMICOLON, MISSING_RPAREN);
        }
        // <proc> -> procedure id ([id {,id}]);
        if (sym == SEMICOLON) {
            getWord();
        } else {
            judge(0, first_block, MISSING_SEMICOLON);
        }
        // <proc> -> procedure id ([id {,id}]);<block> {;<proc>}
        if (sym & first_block) {
            block();
            while (sym == SEMICOLON) {
                getWord();
                // FIRST(proc)
                if (sym == PROC_SYM) {
                    proc();
                } else {
                    error(REDUNDENT_SEMICOLON);
                }
            }
        } else {
            judge(0, follow_block, ILLEGAL_BLOCK);
        }
    } else {
        judge(0, follow_proc, ILLEGAL_PROC);
    }
    level--;
}

// <exp> -> [+|-] <term>{<aop><term>}
void exp()
{
    // <exp> -> [+|-]
    if (sym == PLUS || sym == MINUS) {
        getWord();
    }
    // <exp> -> <term>{<aop><term>}
    if (sym & first_term) // FIRST(term) 、 FIRST(factor)
    {
        term();
        while (sym == PLUS || sym == MINUS) {
            getWord();
            // FIRST(term)
            if (sym & first_term) {
                term();
            } else {
                error(REDUNDENT_WORD);
            }
        }
    } else {
        judge(0, follow_exp, ILLEGAL_EXP);
    }
}

// <factor> -> id|number|(<exp>)
void factor()
{
    if (sym == IDENT) {
        getWord();
        return;
    } else if (sym == NUMBER) {
        getWord();
        return;
    } else if (sym == LPAREN) {
        getWord();
        exp();
        if (sym == RPAREN) {
            getWord();
        } else {
            judge(0, follow_factor, MISSING_RPAREN);
        }
    } else {
        judge(0, follow_factor, ILLEGAL_FACTOR);
    }
}

// <term> -> <factor>{<mop><factor>}
void term()
{
    if (sym & first_term) {
        factor();
        while (sym == MULTI || sym == DIVIS) {
            getWord();
            if (sym & first_term) {
                factor();
            } else {
                error(REDUNDENT_WORD);
            }
        }
    } else {
        judge(0, follow_term, ILLEGAL_TERM);
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
        } else {
            error(EXPECT_EXPRESSION);
        }
    }
    // <lexp> -> <exp> <lop> <exp>
    else if (sym & first_exp) {
        exp();
        if (sym & first_lop) {
            getWord();
            exp();
        } else {
            judge(0, first_exp, MISSING_LOP);
            exp();
        }
    } else {
        judge(0, follow_lexp, ILLEGAL_LEXP);
    }
}

void statement()
{
    switch (sym) {
    // <statement> -> id := <exp>
    case IDENT:
        getWord();
        if (sym == BECOMES) {
            getWord();
        } else if (sym == EQL) {
            error(EXPECT_BECOMES_NOT_EQL);
            getWord();
        } else {
            judge(0, follow_exp, MISSING_BEACOMES);
        }
        exp();
        break;
    // <statement> -> if <lexp> then <statement> [else <statement>]
    case IF_SYM:
        getWord();
        lexp();
        if (sym == THEN_SYM) {
            getWord();
        } else {
            judge(0, first_stmt, MISSING_THEN);
        }

        statement();
        if (sym == ELSE_SYM) {
            getWord();
            statement();
        }
        break;
    // <statement> -> while <lexp> do <statement>
    case WHILE_SYM:
        getWord();
        // FIRST(lexp)
        lexp();
        if (sym == DO_SYM) {
            getWord();
            statement();
        } else {
            judge(0, first_stmt, MISSING_DO);
        }
        break;
    // <statement> -> call id ([{<exp>{,<exp>}])
    case CALL_SYM:
        getWord();
        if (sym == IDENT) {
            getWord();
        } else {
            judge(0, LPAREN, MISSING_IDENT);
        }
        if (sym == LPAREN) {
            getWord();
        } else {
            judge(0, first_exp | RPAREN, MISSING_LPAREN);
        }
        if (sym & first_exp) {
            exp();
            while (sym == COMMA) {
                getWord();
                if (sym & first_exp) {
                    exp();
                } else {
                    judge(0, first_exp, REDUNDENT_COMMA);
                    exp();
                }
            }
        }
        if (sym == RPAREN) {
            getWord();
        }

        break;
    // <statement> -> <body>
    case BEGIN_SYM:
        body();
        break;
    // <statement> -> read (id{,id})
    case READ_SYM:
        getWord();
        if (sym == LPAREN) {
            getWord();
        } else {
            judge(0, IDENT, MISSING_LPAREN);
        }
        if (sym == IDENT) {
            getWord();
        } else {
            judge(0, COMMA | RPAREN, MISSING_IDENT);
        }
        while (sym == COMMA) {
            getWord();
            if (sym == IDENT) {
                getWord();
            } else {
                judge(0, IDENT, REDUNDENT_WORD);
            }
        }
        if (sym == RPAREN) {
            getWord();
        } else {
            judge(0, follow_stmt, MISSING_RPAREN);
        }
        break;
    // <statement> -> write(<exp> {,<exp>})
    case WRITE_SYM:
        getWord();
        if (sym == LPAREN) {
            getWord();
        } else {
            judge(0, first_exp, MISSING_LPAREN);
        }
        exp();
        while (sym == COMMA) {
            getWord();
            if (sym == RPAREN)
                error(REDUNDENT_COMMA);
            else
                exp();
        }
        if (sym == RPAREN) {
            getWord();
        } else {
            judge(0, follow_stmt, MISSING_RPAREN);
        }

        break;
    default:
        judge(0, follow_stmt, ILLEGAL_STMT);
        break;
    }
}

// <body> -> begin <statement> {;<statement>} end
void body()
{
    // 判断是否存在begin,是否仅缺少begin
    judge(BEGIN_SYM, first_stmt, MISSING_BEGIN);
    statement();
    // 这里如果end前多一个分号会多进行一次循环，并进入else分支
    while (sym & (SEMICOLON | COMMA | first_stmt)) {
        // 判断是否存在分号，是否仅缺少分号,是否错写为逗号
        if (sym == COMMA) {
            error(EXPECT_SEMICOLON_FIND_COMMA);
            getWord();
        } else
            judge(SEMICOLON, first_stmt, MISSING_SEMICOLON);
        if (sym & first_stmt) {
            statement();
        } else {
            error(REDUNDENT_WORD);
        }
    }
    // 判断是否缺少end
    judge(END_SYM, 0, MISSING_END);
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
    // <block> -> [<condecl>][<vardecl>][<proc>]]
    if (sym == PROC_SYM) {
        proc();
    }
    // <block> -> [<condecl>][<vardecl>][<proc>]<body>
    body();
}

// <prog> -> program id; <block>
void prog()
{
    // 找到第一次出现的program
    judge(PROGM_SYM, 0, MISSING_PROGM);

    // <prog> -> program id
    if (sym == IDENT) {
        getWord();
    }
    // 错误 <prog> -> program number
    else if (sym == NUMBER) {
        error(EXPECT_IDENT_FIND_NUM);
        getWord();
    }
    // 缺失 <prog> -> program ;
    else if (sym == SEMICOLON) {
        error(MISSING_IDENT);
    }
    //  <prog> -> program {~id} ;
    else {
        error(ILLEGAL_WORD);
        getWord();
    }
    // <prog> -> program id;
    if (sym == SEMICOLON) {
        getWord();
    }
    // <prog> -> program id {~';'} <block>
    else {
        // 判断是否仅仅是缺失分号
        judge(SEMICOLON, first_block, MISSING_SEMICOLON);
    }
    //<prog> -> program id; <block>
    block();
    //<prog> -> program id; <block>#
    if (sym == NUL) {
        // 程序终止
        return;
    }
}

void analyze()
{
    getWord();
    prog();
    if (w_ch != '#') {
        error(60);
    }
    over();
}

void test()
{
    init();
    readFile2USC2(PROGM_PATH);
    // analyze();
}