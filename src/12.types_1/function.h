#include "data.h"

#pragma once

// custom_func.c
void custom_error_line();
void custom_error_char(char *t, char c);
void custom_error_chars(char *c1, char *c2);
void custom_error_int(char *c1, int i);

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