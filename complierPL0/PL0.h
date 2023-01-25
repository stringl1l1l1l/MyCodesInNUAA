#ifndef _PL0_H
#define _PL0_H
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

#define PROGM_PATH \
    "E:\\Programming\\GitHub\\repository\\DataStruct\\complierPL0\\test.txt"
#define RSV_WORD_MAX 15 /* 保留字的数量 */
#define N_MAX 14 /* 数字允许的最长位数 */
#define PROGM_CH_MAX 5000 /*源程序的最大字符数*/
#define ID_MAX 20 /* 标识符最大长度 */
#define OPR_MAX 11 /* 运算符号表最大长度*/
#define VAR_WIDTH 4 /*变量大小*/
#define CST_WIDTH 4 /*常量大小*/
#define SYM_ITEMS_CNT 100 // 符号表项数
#define PROC_CNT 40 // 过程嵌套数
#define ERR_CNT 70 // 报错种数

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

#define UNDECLARED_IDENT 11
#define REDEFINED_IDENT 61
// 全局变量声明
extern wchar_t w_ch; // 最近一次从文件中读出的字符
extern unsigned long sym; // 最近一次识别出来的 token 的类型
extern wstring strToken; // 最近一次识别出来的token的名字
extern wstring err_str; // 最近一次识别出来的非法字符串的名字
extern size_t strToken_len; // 当前token长度
extern size_t num; // 最近一次识别出来的数字的值
extern size_t col_pos; // 列指针
extern size_t row_pos; // 行指针
extern size_t line_lenth; // 行缓冲区长度
extern size_t err; // 出错总次数
extern size_t offset; // 变量地址偏移值
extern size_t level; //  层差

extern string progm_str; // 源程序代码的string字符串形式
extern wstring progm_w_str; // 源程序代码的wchar字符串形式
extern size_t progm_lenth; // 源程序代码的字符数量
extern size_t ch_ptr; // 字符指针，指向词法分析当前读取的字符
extern wstring resv_table[RSV_WORD_MAX]; // 保留字表
extern wchar_t opr_table[OPR_MAX]; // 运算符号表
extern wstring err_msg[ERR_CNT]; // 错误信息表
extern unordered_map<unsigned long, wstring> sym_map; // 保留字编号与字符串的映射
// first集声明
extern unsigned long first_block;
extern unsigned long first_stmt;
extern unsigned long first_factor;
extern unsigned long first_term;
extern unsigned long first_exp;
extern unsigned long first_lexp;
extern unsigned long first_lop;
// follow集声明
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
    NIL, // 空
    ARR, // 数组
    VAR, // 变量
    PROCE, // 过程
    CST, // 常量
    FORM, // 形参
};
enum Type { INTERGER };
// 函数声明
void exp();
void factor();
void term();
void block();
void body();
void PL0Test();
void error(size_t n);
#endif