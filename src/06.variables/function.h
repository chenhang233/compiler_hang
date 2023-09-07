#include "data.h"

#pragma once

// custom_error.c
int custom_error_line();
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