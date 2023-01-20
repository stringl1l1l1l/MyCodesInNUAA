#include "PL0.h"
#include <sstream>
#include <windows.h>
#include <iomanip>

void exp();
void factor();
void term();
void block();
void body();

void init()
{
    symMap[NUL] = "NUL";
    symMap[IDENT] = "IDENT";
    symMap[NUMBER] = "NUMBER";
    symMap[PLUS] = "PLUS";
    symMap[MINUS] = "MINUS";
    symMap[MULTI] = "MULTI";
    symMap[DIVIS] = "DIVIS";
    symMap[ODD_SYM] = "ODD_SYM";
    symMap[EQL] = "EQL";
    symMap[NEQ] = "NEQ";
    symMap[LSS] = "LSS";
    symMap[LEQ] = "LEQ";
    symMap[GTR] = "GTR";
    symMap[GEQ] = "GEQ";
    symMap[LPAREN] = "LPAREN";
    symMap[RPAREN] = "RPAREN";
    symMap[COMMA] = "COMMA";
    symMap[SEMICOLON] = "SEMICOLON";
    symMap[BECOMES] = "BECOMES";
    symMap[BEGIN_SYM] = "BEGIN_SYM";
    symMap[END_SYM] = "END_SYM";
    symMap[IF_SYM] = "IF_SYM";
    symMap[THEN_SYM] = "THEN_SYM";
    symMap[WHILE_SYM] = "WHILE_SYM";
    symMap[DO_SYM] = "DO_SYM";
    symMap[CALL_SYM] = "CALL_SYM";
    symMap[CONST_SYM] = "CONST_SYM";
    symMap[VAR_SYM] = "VAR_SYM";
    symMap[PROC_SYM] = "PROC_SYM";
    symMap[WRITE_SYM] = "WRITE_SYM";
    symMap[READ_SYM] = "READ_SYM";
    symMap[PROGM_SYM] = "PROGM_SYM";
    symMap[ELSE_SYM] = "ELSE_SYM";
}


// 读取待编译源代码文件
void readFile()
{
    file.open(PRGM_PATH, ios::in);
    stringstream stream;
    if (!file.is_open())
    {
        cout << "cannot open file!" << endl;
        exit(0);
    }
    stream << file.rdbuf() << '#';
    progm = stream.str();
    progm_lenth = progm.length();
    c_str_progm = new char[progm_lenth];
    strcpy(c_str_progm, progm.c_str());
}

// 打印错误信息
void error(int n)
{
    err++;
    cout << "\e[31m(" << row_pos << "," << col_pos - strToken_len + 1 << ")"
         << " Error: " << err_msg[n] << "\e[0m " << endl;
}

// 格式化输出分析结果
void over()
{
    if (err == 0)
    {
        cout << "\e[32mNo error. Congratulations!\e[0m" << endl;
    }
    else
    {
        cout << "\e[31mTotol: " << err << " errors\e[0m" << endl;
    }
    exit(0);
}

bool isDigit(char ch)
{
    if (ch >= '0' && ch <= '9')
        return true;
    else
        return false;
}

bool isLetter(char ch)
{
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
        return true;
    else
        return false;
}

bool isTerminate(char ch)
{
    return ch == ' ' || ch == '\t' || ch == '\n' || ch == '#' || ch == 0 || ch == ';' || ch == ',';
}

// 判断是否为运算符，是则返回在表中位置，否则返回-1
int isOprator(char ch)
{
    char *p = opr_table;
    while (p - opr_table != OPR_MAX)
    {
        if (*p == ch)
        {
            return p - opr_table;
        }
        p++;
    }
    return -1;
}

// 跳过所有空白符，并将读指针置于空白符下一个位置，但并不装载下一个字符
void skipBlank()
{
    while (c_str_progm[ch_ptr] && c_str_progm[ch_ptr] == ' ')
    {
        col_pos++;
        ch_ptr++;
    }
}

// 获取ch_ptr对应位置的字符，并移动读指针到下一位置。若当前字符为回车符，则跳过当前字符
void getCh()
{
    ch = c_str_progm[ch_ptr++];
    col_pos++;
}

// 退一个字符
void retract()
{
    ch = c_str_progm[--ch_ptr];
    col_pos--;
}

// 将当前字符追加到token中
void contract()
{
    strcat(strToken, &ch);
    strToken_len++;
}

// 查询str是否为保留字，是保留字返回保留字在表中位置，否则返回-1
int reserve(char *str)
{
    char(*p)[10] = resv_table;
    while (p - resv_table != RSV_WORD_MAX)
    {
        if (strcmp(*p, str) == 0)
        {
            return p - resv_table;
        }
        p++;
    }
    return -1;
}

// 词法分析器
void getWord()
{
    strToken[0] = 0;
    strToken_len = 0;
    skipBlank();
    getCh();
    if (ch == 0)
        return;
    // 跳过连续的回车符
    if (ch == '\n')
    {
        col_pos = 0;
        row_pos++;
        getWord();
        return;
    }
    else if (ch == '#')
    {
        contract();
        sym = NUL;
    }
    // 纯字母
    else if (isLetter(ch))
    {
        while (isLetter(ch) || isDigit(ch))
        {
            contract();
            getCh();
        }
        // 查表，判断是否为保留字
        int code = reserve(strToken);
        switch (code)
        {
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
    else if (isDigit(ch))
    {
        while (isDigit(ch))
        {
            contract();
            getCh();
        }
        // 遇到字母
        if (isLetter(ch))
        {
            error(ILLEGAL_ID);
            // 跳过错误至下一个终止符
            while (!isTerminate(ch))
                getCh();
            retract();
            strToken[0] = 0;
            sym = NUL;
        }
        // 遇到其他字符
        else
        {
            sym = NUMBER;
            retract();
        }
    } // 遇到:判断是否为赋值符号
    else if (ch == ':')
    {
        contract();
        getCh();
        if (ch == '=')
        {
            contract();
            sym = BECOMES;
        }
        else
        {
            error(MISSING_EQL);
            strToken[0] = 0;
            sym = NUL;
        }
    }
    else if (ch == '<')
    {
        contract();
        getCh();
        if (ch == '=')
        {
            contract();
            sym = LEQ;
        }
        else if (ch == '>')
        {
            contract();
            sym = NEQ;
        }
        else
        {
            sym = LSS;
            retract();
        }
    }
    else if (ch == '>')
    {
        contract();
        getCh();
        if (ch == '=')
        {
            contract();
            sym = GEQ;
        }
        else
        {
            sym = GTR;
            retract();
        }
    }
    else
    {
        int code = isOprator(ch);
        if (code != -1)
        {
            contract();
            switch (code)
            {
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
        }
        else
        {
            error(ILLEGAL_CHAR);
            strToken[0] = 0;
            sym = NUL;
        }
    }
    cout << "(" << row_pos << "," << col_pos << ")\t"
         << setw(15) << strToken << setw(20) << symMap[sym]
         << endl;
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
            if (ch == 0)
                over();
            getWord(); // 继续词法分析
        }
        if (sym & s1)
            getWord();
    }
    else
        getWord();
}

// 开始语法分析
// const -> id:=number
void constDef()
{
    if (sym == IDENT)
    {
        getWord();
        if (sym == BECOMES || sym == EQL)
        {
            if (sym == EQL)
            {
                error(EXPECT_BECOMES_NOT_EQL);
            }
            getWord();
            if (sym == NUMBER)
            {
                getWord();
            }
            else
            {
                error(EXPECT_NUMEBR_AFTER_BECOMES);
            }
        }
    }
    else
    {
        judge(0, follow_constdef, ILLEGAL_CONSTDEF);
    }
}

// <condecl> -> const <constDef>{,<constDef>};
void condecl()
{
    // const
    if (sym == CONST_SYM)
    {
        // const <constDef>
        getWord();

        constDef();
        // const <constDef> {,<constDef>}
        while (sym & (COMMA | IDENT))
        {
            judge(COMMA, IDENT, MISSING_COMMA);
            if (sym == IDENT) // FIRST(<constDef>)
            {
                constDef();
            }
            else
            {
                judge(0, IDENT, REDUNDENT_WORD); // todo expect常量定义式
                constDef();
            }
        }
        // const <constDef> {,<constDef>};
        if (sym == SEMICOLON)
        {
            getWord();
            return;
        }
        else
        {
            judge(0, follow_condecl, MISSING_SEMICOLON); // todo expect ;
        }
    }
    else
    {
        judge(0, follow_condecl, ILLEGAL_CONDECL);
    }
}

// <vardecl> -> var <id>{,<id>};
void vardecl()
{
    // var
    if (sym == VAR_SYM)
    {
        getWord();
        // var <id>
        if (sym == IDENT)
        {
            getWord();
        }
        else
        {
            judge(0, COMMA, MISSING_IDENT);
        }
        // var <id>{,<id>}
        while (sym == COMMA)
        {
            getWord();
            if (sym == IDENT)
            {
                getWord();
            }
            else
            {
                error(REDUNDENT_COMMA); // todo expect 标识符
            }
        }
        // var <id>{,<id>};
        if (sym == SEMICOLON)
        {
            getWord();
        }
        else
        {
            judge(0, follow_vardecl, MISSING_SEMICOLON);
        }
    }
    else
    {
        judge(0, follow_vardecl, ILLEGAL_VARDECL);
    }
}

// <proc> -> procedure id ([id {,id}]);<block> {;<proc>}
void proc()
{
    if (sym == PROC_SYM)
    {
        getWord();
        // <proc> -> procedure id
        if (sym == IDENT)
        {
            getWord();
        }
        else
        {
            judge(0, LPAREN, MISSING_IDENT);
        }
        // <proc> -> procedure id (
        if (sym == LPAREN)
        {
            getWord();
        }
        else
        {
            judge(0, IDENT | RPAREN, MISSING_LPAREN);
        }
        // <proc> -> procedure id ([id {,id}]
        if (sym == IDENT)
        {
            getWord();
            while (sym == COMMA)
            {
                getWord();
                if (sym == IDENT)
                {
                    getWord();
                }
                else
                {
                    error(REDUNDENT_COMMA);
                }
            }
        }
        // <proc> -> procedure id ([id {,id}])
        if (sym == RPAREN)
        {
            getWord();
        }
        else
        {
            judge(0, SEMICOLON, MISSING_RPAREN);
        }
        // <proc> -> procedure id ([id {,id}]);
        if (sym == SEMICOLON)
        {
            getWord();
        }
        else
        {
            judge(0, first_block, MISSING_SEMICOLON);
        }
        // <proc> -> procedure id ([id {,id}]);<block> {;<proc>}
        if (sym & first_block)
        {
            block();
            while (sym == SEMICOLON)
            {
                getWord();
                // FIRST(proc)
                if (sym == PROC_SYM)
                {
                    proc();
                }
                else
                {
                    error(REDUNDENT_SEMICOLON);
                }
            }
        }
        else
        {
            judge(0, follow_block, ILLEGAL_BLOCK);
        }
    }
    else
    {
        judge(0, follow_proc, ILLEGAL_PROC);
    }
}

// <exp> -> [+|-] <term>{<aop><term>}
void exp()
{
    // <exp> -> [+|-]
    if (sym == PLUS || sym == MINUS)
    {
        getWord();
    }
    // <exp> -> <term>{<aop><term>}
    if (sym & first_term) // FIRST(term) 、 FIRST(factor)
    {
        term();
        while (sym == PLUS || sym == MINUS)
        {
            getWord();
            // FIRST(term)
            if (sym & first_term)
            {
                term();
            }
            else
            {
                error(REDUNDENT_WORD);
            }
        }
    }
    else
    {
        judge(0, follow_exp, ILLEGAL_EXP);
    }
}

// <factor> -> id|number|(<exp>)
void factor()
{
    if (sym == IDENT)
    {
        getWord();
        return;
    }
    else if (sym == NUMBER)
    {
        getWord();
        return;
    }
    else if (sym == LPAREN)
    {
        getWord();
        exp();
        if (sym == RPAREN)
        {
            getWord();
        }
        else
        {
            judge(0, follow_factor, MISSING_RPAREN);
        }
    }
    else
    {
        judge(0, follow_factor, ILLEGAL_FACTOR);
    }
}

// <term> -> <factor>{<mop><factor>}
void term()
{
    if (sym & first_term)
    {
        factor();
        while (sym == MULTI || sym == DIVIS)
        {
            getWord();
            if (sym & first_term)
            {
                factor();
            }
            else
            {
                error(REDUNDENT_WORD);
            }
        }
    }
    else
    {
        judge(0, follow_term, ILLEGAL_TERM);
    }
}

// <lexp> -> <exp> <lop> <exp> | odd <exp>
void lexp()
{
    // <lexp> -> odd <exp>
    if (sym == ODD_SYM)
    {
        getWord();
        if (sym & first_exp)
        {
            exp();
        }
        else
        {
            error(EXPECT_EXPRESSION);
        }
    }
    // <lexp> -> <exp> <lop> <exp>
    else if (sym & first_exp)
    {
        exp();
        if (sym & first_lop)
        {
            getWord();
            exp();
        }
        else
        {
            judge(0, first_exp, MISSING_LOP);
            exp();
        }
    }
    else
    {
        judge(0, follow_lexp, ILLEGAL_LEXP);
    }
}

void statement()
{
    switch (sym)
    {
    // <statement> -> id := <exp>
    case IDENT:
        getWord();
        if (sym == BECOMES)
        {
            getWord();
        }
        else if (sym == EQL)
        {
            error(EXPECT_BECOMES_NOT_EQL);
            getWord();
        }
        else
        {
            judge(0, follow_exp, MISSING_BEACOMES);
        }
        exp();
        break;
    // <statement> -> if <lexp> then <statement> [else <statement>]
    case IF_SYM:
        getWord();
        lexp();
        if (sym == THEN_SYM)
        {
            getWord();
        }
        else
        {
            judge(0, first_stmt, MISSING_THEN);
        }

        statement();
        if (sym == ELSE_SYM)
        {
            getWord();
            statement();
        }
        break;
    // <statement> -> while <lexp> do <statement>
    case WHILE_SYM:
        getWord();
        // FIRST(lexp)
        lexp();
        if (sym == DO_SYM)
        {
            getWord();
            statement();
        }
        else
        {
            judge(0, first_stmt, MISSING_DO);
        }
        break;
    // <statement> -> call id ([{<exp>{,<exp>}])
    case CALL_SYM:
        getWord();
        if (sym == IDENT)
        {
            getWord();
        }
        else
        {
            judge(0, LPAREN, MISSING_IDENT);
        }
        if (sym == LPAREN)
        {
            getWord();
        }
        else
        {
            judge(0, first_exp | RPAREN, MISSING_LPAREN);
        }
        if (sym & first_exp)
        {
            exp();
            while (sym == COMMA)
            {
                getWord();
                if (sym & first_exp)
                {
                    exp();
                }
                else
                {
                    judge(0, first_exp, REDUNDENT_COMMA);
                    exp();
                }
            }
        }
        if (sym == RPAREN)
        {
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
        if (sym == LPAREN)
        {
            getWord();
        }
        else
        {
            judge(0, IDENT, MISSING_LPAREN);
        }
        if (sym == IDENT)
        {
            getWord();
        }
        else
        {
            judge(0, COMMA | RPAREN, MISSING_IDENT);
        }
        while (sym == COMMA)
        {
            getWord();
            if (sym == IDENT)
            {
                getWord();
            }
            else
            {
                judge(0, IDENT, REDUNDENT_WORD);
            }
        }
        if (sym == RPAREN)
        {
            getWord();
        }
        else
        {
            judge(0, follow_stmt, MISSING_RPAREN);
        }
        break;
    // <statement> -> write(<exp> {,<exp>})
    case WRITE_SYM:
        getWord();
        if (sym == LPAREN)
        {
            getWord();
        }
        else
        {
            judge(0, first_exp, MISSING_LPAREN);
        }
        exp();
        while (sym == COMMA)
        {
            getWord();
            if (sym == RPAREN)
                error(REDUNDENT_COMMA);
            else
                exp();
        }
        if (sym == RPAREN)
        {
            getWord();
        }
        else
        {
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
    while (sym & (SEMICOLON | COMMA | first_stmt))
    {
        // 判断是否存在分号，是否仅缺少分号,是否错写为逗号
        if (sym == COMMA)
        {
            error(EXPECT_SEMICOLON_FIND_COMMA);
            getWord();
        }
        else
            judge(SEMICOLON, first_stmt, MISSING_SEMICOLON);
        if (sym & first_stmt)
        {
            statement();
        }
        else
        {
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
    if (sym == CONST_SYM)
    {
        condecl();
    }
    // <block> -> [<condecl>][<vardecl>]
    if (sym == VAR_SYM)
    {
        vardecl();
    }
    // <block> -> [<condecl>][<vardecl>][<proc>]]
    if (sym == PROC_SYM)
    {
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
    if (sym == IDENT)
    {
        getWord();
    }
    // 错误 <prog> -> program number
    else if (sym == NUMBER)
    {
        error(EXPECT_IDENT_FIND_NUM);
        getWord();
    }
    // 缺失 <prog> -> program ;
    else if (sym == SEMICOLON)
    {
        error(MISSING_IDENT);
    }
    //  <prog> -> program {~id} ;
    else
    {
        error(ILLEGAL_WORD);
        getWord();
    }
    // <prog> -> program id;
    if (sym == SEMICOLON)
    {
        getWord();
    }
    // <prog> -> program id {~';'} <block>
    else
    {
        // 判断是否仅仅是缺失分号
        judge(SEMICOLON, first_block, MISSING_SEMICOLON);
    }
    //<prog> -> program id; <block>
    block();
    //<prog> -> program id; <block>#
    if (sym == NUL)
    {
        // 程序终止
        return;
    }
}

void grammaAnalysis()
{
    getWord();
    prog();
    if (ch != '#')
    {
        error(60);
    }
    over();
}

void test()
{
    init();
    readFile();
    grammaAnalysis();
}

int main()
{
    test();
}