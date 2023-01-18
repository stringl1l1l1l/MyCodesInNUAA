#include <stdio.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <string>
#include <unordered_map>
using namespace std;

#define PRGM_PATH "E:\\Programming\\GitHub\\repository\\DataStruct\\complierPL0\\test.txt"
#define RSV_WORD_MAX 15 /* 保留字的数量 */
#define N_MAX 14        /* 数字允许的最长位数 */
#define ID_MAX 10       /* 标识符最大长度 */
#define OPR_MAX 11      /* 运算符号表最大长度*/

#define NUL 0x0           /* 空 */
#define IDENT 0x1         /* 标识符 */
#define NUMBER 0x2        /* 数值 */
#define PLUS 0x4          /* + */
#define MINUS 0x8         /* - */
#define MULTI 0x10        /* * */
#define DIVIS 0x20        /* / */
#define ODD_SYM 0x40      /* 奇数判断 */
#define EQL 0x80          /* = */
#define NEQ 0x100         /* <> */
#define LSS 0x200         /* < */
#define LEQ 0x400         /* <= */
#define GTR 0x800         /* > */
#define GEQ 0x1000        /* >= */
#define LPAREN 0x2000     /* ( */
#define RPAREN 0x4000     /* ) */
#define COMMA 0x8000      /* , */
#define SEMICOLON 0x10000 /* ; */
#define BECOMES 0x20000   /*:=*/

#define BEGIN_SYM 0x40000
#define END_SYM 0x80000
#define IF_SYM 0x100000
#define THEN_SYM 0x200000
#define WHILE_SYM 0x400000
#define DO_SYM 0x800000
#define CALL_SYM 0x1000000
#define CONST_SYM 0x2000000
#define VAR_SYM 0x4000000
#define PROC_SYM 0x8000000
#define WRITE_SYM 010000000
#define READ_SYM 0x20000000
#define PROGM_SYM 0x40000000
#define ELSE_SYM 0x80000000

#define EXPECT_BECOMES_NOT_EQL 1
#define EXPECT_NUMEBR_AFTER_BECOMES 2
#define EXPECT_STATEMENT 7
#define EXPECT_EXPRESSION 27
#define EXPECT_IDENT_FIND_NUM 29
#define EXPECT_SEMICOLON_FIND_COMMA 38

#define ILLEGAL 30
#define ILLEGAL_ID 33
#define ILLEGAL_CHAR 35
#define INCORRECT_PROC_NAME 6
#define ILLEGAL_WORD 40
#define ILLEGAL_EXP 46
#define ILLEGAL_FACTOR 47
#define ILLEGAL_BLOCK 48
#define ILLEGAL_CONSTDEF 50
#define ILLEGAL_VARDECL 51
#define ILLEGAL_CONDECL 55
#define ILLEGAL_PROC 56
#define ILLEGAL_TERM 57
#define ILLEGAL_LEXP 58
#define ILLEGAL_STMT 59

#define MISSING 25
#define MISSING_EQL 34
#define MISSING_PROGM 0
#define MISSING_SEMICOLON 5
#define MISSING_IDENT 28
#define MISSING_BEGIN 36
#define MISSING_END 37
#define MISSING_BEACOMES 39
#define MISSING_COMMA 41
#define MISSING_CONST 42
#define MISSING_LPAREN 43
#define MISSING_RPAREN 44
#define MISSING_THEN 52
#define MISSING_LOP 53
#define MISSING_DO 54

#define REDUNDENT_WORD 26
#define REDUNDENT_COMMA 45
#define REDUNDENT_SEMICOLON 49

char ch = 32;                    // 用于词法分析器，存放最近一次从文件中读出的字符
unsigned long sym;               // 词法分析器输出结果之用，存放最近一次识别出来的 token 的类型
char strToken[ID_MAX + 1] = {0}; // 词法分析器输出结果之用，存放最近一次识别出来的标识符的名字
size_t strToken_len = 0;
long num;           // 词法分析器输出结果之用，存放最近一次识别出来的数字的值
long col_pos = 0;   // 列指针
long row_pos = 1;   // 行指针
long line_lenth;    // 行缓冲区长度
long err = 0;       /* 出错总次数 */
char line[81];      /* 行缓冲区，用于从文件读出一行，供词法分析获取单词时之用 */
char a[ID_MAX + 1]; /* 词法分析器中用于临时存放正在分析的词 */

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

fstream file;
string progm;
char *c_str_progm;
size_t progm_lenth;
size_t ch_ptr;

// 保留字编号与字符串的映射
unordered_map<unsigned long, string> symMap;

// 保留字表
char resv_table[RSV_WORD_MAX][10] = {"odd", "begin", "end", "if", "then", "while", "do", "call", "const",
                                     "var", "procedure", "write", "read", "program", "else"};

// 运算符号表
char opr_table[OPR_MAX] = {
    '+',
    '-',
    '*',
    '/',
    '=',
    '<',
    '>',
    '(',
    ')',
    ',',
    ';'};

// 错误信息表
char err_msg[65][100] =
    {
        /*  0 */ "Missing 'program'",
        /*  1 */ "Found '=' when expecting ':='.",
        /*  2 */ "There must be a number to follow '='.",
        /*  3 */ "There must be an '=' to follow the identifier.",
        /*  4 */ "There must be an identifier to follow 'const', 'var', or 'procedure'.",
        /*  5 */ "Missing ';'",
        /*  6 */ "Incorrect procedure name.",
        /*  7 */ "Statement expected.",
        /*  8 */ "Follow the statement is an incorrect symbol.",
        /*  9 */ "'.' expected.",
        /* 10 */ "';' expected.",
        /* 11 */ "Undeclared identifier.",
        /* 12 */ "Illegal assignment.",
        /* 13 */ "':=' expected.",
        /* 14 */ "There must be an identifier to follow the 'call'.",
        /* 15 */ "A constant or variable can not be called.",
        /* 16 */ "'then' expected.",
        /* 17 */ "';' or 'end' expected.",
        /* 18 */ "'do' expected.",
        /* 19 */ "Incorrect symbol.",
        /* 20 */ "Relative operators expected.",
        /* 21 */ "Procedure identifier can not be in an expression.",
        /* 22 */ "Missing ')'.",
        /* 23 */ "The symbol can not be followed by a factor.",
        /* 24 */ "The symbol can not be as the beginning of an expression.",
        /* 25 */ "Missing ",
        /* 26 */ "Redundent word",
        /* 27 */ "Expression expected",
        /* 28 */ "Missing identifier",
        /* 29 */ "Find number when identifier expected",
        /* 30 */ "Illegal ",
        /* 31 */ "The number is too great.",
        /* 32 */ "There are too many levels.",
        /* 33 */ "Illegal identifier",
        /* 34 */ "Missing '='",
        /* 35 */ "Illegal character",
        /* 36 */ "Missing 'begin'",
        /* 37 */ "Missing 'end'",
        /* 38 */ "Find ',' when expecting ';'",
        /* 39 */ "Missing ':='",
        /* 40 */ "Illegel word",
        /* 41 */ "Missing ','",
        /* 42 */ "Missing 'const'",
        /* 43 */ "Missing '('",
        /* 44 */ "Missing ')'",
        /* 45 */ "Missing ','",
        /* 46 */ "Illegal expression",
        /* 47 */ "Illegal factor",
        /* 48 */ "Illegal block",
        /* 49 */ "Redundent ';'",
        /* 50 */ "Illegal const definition",
        /* 51 */ "Illegal",
        /* 52 */ "Missing 'then'",
        /* 53 */ "Missing '<' or '<=' or '>' or '>=' or '<>' or '='",
        /* 54 */ "Missing 'do'",
        /* 55 */ "Illegal const declaration",
        /* 56 */ "Illegal procedure definition",
        /* 57 */ "Illegal term definition",
        /* 58 */ "Illegal lexp definition",
        /* 59 */ "Illegal statement definition",
        /* 60 */ "Redundent words when program should done"};