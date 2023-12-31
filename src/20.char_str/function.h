#include "data.h"

#pragma once

// custom_func.c
void custom_error_line();
void custom_error_char(char *t, char c);
void custom_error_chars(char *c1, char *c2);
void custom_error_int(char *c1, int i);
void match(Token_type t, char *str);
void match_semi();
void match_lbrace();
void match_rbrace();
void match_lparen();
void match_rparen();
void match_ident();
void match_int();
void match_assign();
void match_if();
void match_while();
void match_for();
void match_void();
void match_return();
void match_lbracket();
void match_rbracket();

// scan.c
int next();
int skip();
int scan(Token *);
int int_pos(char *str, int s);
int scanint(int c);
static int scanident(char c, char *str, int strlen);
int scankey(char *s);
void reject_token(Token *t);
static int scan_ch(void);
static int scanstr(char *buf);

// gen.c
void genpreamble();
void genfreeregs();
void genprintint(int reg);
void genglobsym(int id);
int genAST(ASTnode *n, int label, AST_node_type parentASTop);
int genIf(ASTnode *n);
int genWhile(ASTnode *n);
int label();
int genprimsize(int type);
void genpostamble();
int genlabel(void);
int genglobstr(char *strvalue);

// cg.c
void freeall_registers(void);
static void free_register(int reg);
void cgpreamble();
static int alloc_register(void);
void cgfuncpreamble(int id);
void cgfuncpostamble(int id);
void cgprintint(int r);
void cgglobsym(int id);
void cgglobstr(int l, char *strvalue);
int cgloadint(int value, int type);
int cgloadglob(int id);
int cgloadglobstr(int id);
int cgadd(int r1, int r2);
int cgsub(int r1, int r2);
int cgmul(int r1, int r2);
int cgdiv(int r1, int r2);
void cgprintint(int r);
int cgstorglob(int r, int id);
int cgcompare_and_set(AST_node_type ASTop, int r1, int r2);
void cglabel(int l);
void cgjump(int l);
int cgcompare_and_jump(AST_node_type ASTop, int r1, int r2, int label);
int cgwiden(int r, int oldtype, int newtype);
int cgprimsize(Primitive_type type);
void cgreturn(int reg, int id);
int cgcall(int r, int id);
int cgaddress(int id);
int cgderef(int r, int type);
void cgpostamble();
int cgshlconst(int r, int val);
int cgstorderef(int r1, int r2, int type);

// stm.c
ASTnode *function_declaration(Primitive_type type);
ASTnode *compound_statement();
static ASTnode *single_statement(void);
static ASTnode *print_statement(void);
void var_declaration(Primitive_type type);
static ASTnode *assignment_statement(void);
static ASTnode *if_statement(void);
static ASTnode *while_statement();
static ASTnode *for_statement();
static ASTnode *return_statement(void);
Primitive_type parse_type();
void global_declarations();

// ast.c
ASTnode *mkAST_node(AST_node_type op, Primitive_type type, ASTnode *left, ASTnode *mid, ASTnode *right, int intv);
ASTnode *mkAST_leaf(AST_node_type op, Primitive_type type, int id);
ASTnode *mkAST_left(AST_node_type op, Primitive_type type, ASTnode *left, int v);
ASTnode *binexpr(int ptp);
static ASTnode *primary(void);
static ASTnode *array_access(void);
static int op_precedence(Token_type tokentype);
static int rightassoc(Token_type tokentype);
static AST_node_type binastop(Token_type tokentype);
static int arithop(int tokentype);
ASTnode *funccall(void);
ASTnode *prefix(void);

static int gendumplabel(void);
void dumpAST(ASTnode *n, int label, int level);

// sym.c
int findglob(char *s);
char *my_strdup(const char *source);
static int newglob();
int addglob(char *name, Primitive_type type, Structural_type stype,
            int endlabel, int size);

// types.c
int type_compatible(Primitive_type *left, Primitive_type *right, int onlyright);
Primitive_type pointer_to(Primitive_type type);
Primitive_type value_at(Primitive_type type);
ASTnode *modify_type(ASTnode *tree, Primitive_type rtype, AST_node_type op);
int inttype(Primitive_type type);
int ptrtype(Primitive_type type);