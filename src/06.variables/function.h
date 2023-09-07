#include "data.h"

#pragma once

// custom_error.c
void custom_error_line();
void custom_error_char(char *, char);
void custom_error_chars(char *, char *);
void custom_error_int(char *, int);

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
void generator_preamble();
void generator_postamble();

// code_generator.c
void free_all_registers();
void free_register(int);
int alloc_register();
void code_generator_preamble();
void code_generator_postamble();

// statements.c
void statements();
void statements_print();
void statements_assign();
void statements_variable();