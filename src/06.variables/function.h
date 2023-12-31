#include "data.h"

#pragma once

// custom_error.c
void custom_error_line();
void custom_error_char(char *, char);
void custom_error_chars(char *, char *);
void custom_error_int(char *, int);
static void match(Token_type, char *);
void match_semi();
void match_print();
void match_int();
void match_ident();
void match_equals();
char *my_strdup(const char *);

// main.c
void usage(char *, const char *);
void init();

// scan.c
int scan(Token *);
int next();
int skip();
int scan_dight(int);
int dight_pos(char *, int);
int scan_identifier(int, char *, int);
Token_type scan_keys(char *);

// generator.c
int generator_AST(AST *, int);
void generator_preamble();
void generator_postamble();
int generator_load_int(int);
int generator_load_global(char *);
int generator_global_symbol(char *);
int generator_store_register(int, char *);
int generator_add(int, int);
int generator_sub(int, int);
int generator_mul(int, int);
int generator_div(int, int);
int generator_printint(int);
void generator_free_all_registers();

// code_generator.c
void free_all_registers();
void free_register(int);
int alloc_register();
void code_generator_preamble();
void code_generator_postamble();
int code_generator_load_int(int);
int code_generator_load_global(char *);
int code_generator_global_symbol(char *);
int code_generator_store_register(int, char *);
int code_generator_add(int, int);
int code_generator_sub(int, int);
int code_generator_mul(int, int);
int code_generator_div(int, int);
int code_generator_printint(int);

// statements.c
void statements();
void statements_print();
void statements_assign();
void statements_variable();

// tree.c
AST *mkast_node(AST_TYPE, AST *, AST *, AST_V);
AST *mkast_leaf(AST_TYPE, AST_V);

// ast.c
AST *ast_generator(int);
AST *primary();
int get_score(Token_type);
static AST_TYPE arithop(Token_type);

// symbol.c
int global_find(char *);
static int global_new();
int global_add(char *);
