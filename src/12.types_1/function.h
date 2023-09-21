#include "data.h"

#pragma once

// custom_func.c
void custom_error_line();
void custom_error_char(char *t, char c);
void custom_error_chars(char *c1, char *c2);
void custom_error_int(char *c1, int i);
void match(AST_node_type t, char *str);
void match_semi();
void match_lbrace();
void match_rbrace();
void match_lparen();
void match_rparen();
void match_ident();
void match_int();
void match_assign();
void match_print();
void match_if();
void match_while();
void match_for();
void match_void();

// scan.c
int next();
int skip();
int scan(Token *);
int int_pos(char *str, int s);
int scanint(int c);
int scanident(char c, char *str, int strlen);
int scankey(char *s);

// gen.c
void genpreamble();
void genfreeregs();
void genprintint(int reg);
void genglobsym(int id);
int genAST(ASTnode *n, int reg, AST_node_type parentASTop);

// cg.c
void freeall_registers(void);
void cgpreamble();
static int alloc_register(void);
void cgfuncpreamble(char *name);
void cgfuncpostamble();
void cgprintint(int r);
void cgglobsym(int id);

// stm.c
ASTnode *function_declaration();
ASTnode *compound_statement();
static ASTnode *single_statement(void);
static ASTnode *print_statement(void);
void var_declaration();
static ASTnode *assignment_statement(void);
static ASTnode *if_statement(void);
static ASTnode *while_statement();
static ASTnode *for_statement();

// ast.c
ASTnode *mkAST_node(AST_node_type op, Primitive_type type, ASTnode *left, ASTnode *mid, ASTnode *right, int intv);
ASTnode *mkAST_leaf(AST_node_type op, Primitive_type type, int id);
ASTnode *mkAST_left(AST_node_type op, Primitive_type type, ASTnode *left, int v);
ASTnode *binexpr(int ptp);
static ASTnode *primary(void);
static int op_precedence(int tokentype);
static int arithop(int tokentype);

// sym.c
int findglob(char *s);
char *my_strdup(const char *source);
static int newglob();
int addglob(char *name, Primitive_type type, Structural_type stype)