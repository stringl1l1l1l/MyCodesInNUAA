#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

#define PRGM_PATH \
    "E:\\Programming\\GitHub\\repository\\DataStruct\\complierPL0\\test.txt"
#define RSV_WORD_MAX 15 /* 保留字的数量 */
#define N_MAX 14 /* 数字允许的最长位数 */
#define ID_MAX 20 /* 标识符最大长度 */
#define OPR_MAX 11 /* 运算符号表最大长度*/

#define NUL 0x0 /* 空 */
#define IDENT 0x1 /* 标识符 */
#define NUMBER 0x2 /* 数值 */
#define PLUS 0x4 /* + */
#define MINUS 0x8 /* - */
#define MULTI 0x10 /* * */
#define DIVIS 0x20 /* / */
#define ODD_SYM 0x40 /* 奇数判断 */
#define EQL 0x80 /* = */
#define NEQ 0x100 /* <> */
#define LSS 0x200 /* < */
#define LEQ 0x400 /* <= */
#define GTR 0x800 /* > */
#define GEQ 0x1000 /* >= */
#define LPAREN 0x2000 /* ( */
#define RPAREN 0x4000 /* ) */
#define COMMA 0x8000 /* , */
#define SEMICOLON 0x10000 /* ; */
#define BECOMES 0x20000 /*:=*/

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

extern char ch; // 用于词法分析器，存放最近一次从文件中读出的字符
extern unsigned long sym; // 词法分析器输出结果之用，存放最近一次识别出来的 token 的类型
extern char strToken[ID_MAX + 1]; // 词法分析器输出结果之用，存放最近一次识别出来的标识符的名字
extern size_t strToken_len; // 当前token长度
extern long num; // 词法分析器输出结果之用，存放最近一次识别出来的数字的值
extern long col_pos; // 列指针
extern long row_pos; // 行指针
extern long line_lenth; // 行缓冲区长度
extern long err; /* 出错总次数 */
extern char line[81]; /* 行缓冲区，用于从文件读出一行，供词法分析获取单词时之用 */
extern char a[ID_MAX + 1]; /* 词法分析器中用于临时存放正在分析的词 */
extern fstream file;
extern string progm;
extern char* c_str_progm;
extern size_t progm_lenth;
extern size_t ch_ptr;
extern char resv_table[RSV_WORD_MAX][10]; // 保留字表
extern char opr_table[OPR_MAX]; // 运算符号表
extern char err_msg[65][100]; // 错误信息表
extern unordered_map<unsigned long, string> sym_map; // 保留字编号与字符串的映射
// first集
extern unsigned long first_block;
extern unsigned long first_stmt;
extern unsigned long first_factor;
extern unsigned long first_term;
extern unsigned long first_exp;
extern unsigned long first_lexp;
extern unsigned long first_lop;
// follow集
extern unsigned long follow_condecl;
extern unsigned long follow_vardecl;
extern unsigned long follow_body;
extern unsigned long follow_stmt;
extern unsigned long follow_lexp;
extern unsigned long follow_exp;
extern unsigned long follow_term;
extern unsigned long follow_factor;
extern unsigned long follow_ident;
extern unsigned long follow_block;
extern unsigned long follow_constdef;
extern unsigned long follow_proc;
enum Category {
    NIL,
    ARR,
    VAR,
    FUN
};
enum Type {};

// 函数声明
void exp();
void factor();
void term();
void block();
void body();
void test();